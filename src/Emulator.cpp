#include "Emulator.hpp"

#include <string>
#include <fstream>
#include <utility>
#include <system_error>
#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "ApuImpl.hpp"
#include "BusImpl.hpp"
#include "CpuImpl.hpp"
#include "InputsImpl.hpp"
#include "MemoryImpl.hpp"
#include "ShiftRegisterImpl.hpp"
#include "SdlResource.hpp"

Emulator::Emulator()
    : shouldRun(false)
    , shiftRegister(std::make_shared<ShiftRegisterImpl>())
    , inputs(std::make_shared<InputsImpl>())
    , memory(std::make_shared<MemoryImpl>())
{
    apu = std::make_shared<ApuImpl>([this](unsigned index){
        // Prevent same sounds from overlapping
        if(!Mix_Playing(index)) {
            Mix_PlayChannel(index, this->sounds[index].get(), 0);
        }
    });
    bus = std::make_shared<BusImpl>(memory, inputs, apu, shiftRegister);
    cpu = std::make_shared<CpuImpl>(bus);

    initializeSdlResources();
    loadRoms();
    shouldRun = true;
}

Emulator::~Emulator()
{
    Mix_CloseAudio();
    SDL_Quit();
}

void Emulator::run()
{
    uint64_t currentTime = 0;
    uint64_t lastTime = 0;
    uint64_t deltaTime = 0;
    while(shouldRun)
    {
        currentTime = SDL_GetTicks64();
        deltaTime = currentTime - lastTime;

        handleInput();
        update(deltaTime);
        render();

        emscripten_sleep(16 - currentTime);

        lastTime = currentTime;
    }
}

void Emulator::initializeSdlResources()
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        throw std::system_error(
            errno, 
            std::generic_category(), 
            std::string("Unable to initialize SDL: ") + std::string(SDL_GetError())
            );
    }

    if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) != 0) {
        throw std::system_error(
            errno,
            std::generic_category(),
            std::string("Unable to initialize SDL_mixer: " + std::string(SDL_GetError()))
            );
    }

    Mix_Volume(-1, AUDIO_VOLUME);

    window = make_sdl_resource(
        SDL_CreateWindow, 
        SDL_DestroyWindow, 
        "Space Invaders", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        DISPLAY_WIDTH * PIXEL_SIZE, 
        DISPLAY_HEIGHT * PIXEL_SIZE, 
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
        );
    
    renderer = make_sdl_resource(
        SDL_CreateRenderer,
        SDL_DestroyRenderer,
        window.get(),
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    SDL_RenderSetLogicalSize(renderer.get(), DISPLAY_WIDTH, DISPLAY_HEIGHT);

    texture = make_sdl_resource(
        SDL_CreateTexture,
        SDL_DestroyTexture,
        renderer.get(),
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT
    );
}

void Emulator::loadRoms()
{
    using RomInfoArray = std::array<std::pair<std::string, u16>, 4>;
    const RomInfoArray romInfoArray = {{
        std::make_pair("roms/invaders.h", 0x0000),
        std::make_pair("roms/invaders.g", 0x0800),
        std::make_pair("roms/invaders.f", 0x1000),
        std::make_pair("roms/invaders.e", 0x1800)
    }};

    auto fileReader = [this](auto filename, auto startAddr){
        
        auto file = std::ifstream(filename, std::ios::binary);

        if(!file.is_open()) {
            file.exceptions(file.failbit);
        }
        
        file.seekg(0, std::ios::end);
        auto fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        
        std::vector<u8> data(fileSize);
        file.read((char *) &data[0], fileSize);
        
        for(const auto& byte : data)
            this->memory->write(startAddr++, byte);
    };

    sounds = {
        make_sdl_resource(Mix_LoadWAV_RW, Mix_FreeChunk, SDL_RWFromFile("roms/ufo_lowpitch.wav", "rb"), 1),
        make_sdl_resource(Mix_LoadWAV_RW, Mix_FreeChunk, SDL_RWFromFile("roms/shoot.wav", "rb"), 1),
        make_sdl_resource(Mix_LoadWAV_RW, Mix_FreeChunk, SDL_RWFromFile("roms/explosion.wav", "rb"), 1),
        make_sdl_resource(Mix_LoadWAV_RW, Mix_FreeChunk, SDL_RWFromFile("roms/invaderkilled.wav", "rb"), 1),
        make_sdl_resource(Mix_LoadWAV_RW, Mix_FreeChunk, SDL_RWFromFile("roms/fastinvader1.wav", "rb"), 1),
        make_sdl_resource(Mix_LoadWAV_RW, Mix_FreeChunk, SDL_RWFromFile("roms/fastinvader2.wav", "rb"), 1),
        make_sdl_resource(Mix_LoadWAV_RW, Mix_FreeChunk, SDL_RWFromFile("roms/fastinvader3.wav", "rb"), 1),
        make_sdl_resource(Mix_LoadWAV_RW, Mix_FreeChunk, SDL_RWFromFile("roms/fastinvader4.wav", "rb"), 1),
        make_sdl_resource(Mix_LoadWAV_RW, Mix_FreeChunk, SDL_RWFromFile("roms/ufo_highpitch.wav", "rb"), 1)
    };

    for(const auto& rom : romInfoArray)
        fileReader(rom.first, rom.second);
}

void Emulator::handleInput()
{
    static SDL_Event event;
    while(SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
            shouldRun = false;
        else if(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
            handleKeyEvent(event);
    }
}

void Emulator::update(uint32_t dt)
{
    static const u8 RST_10 = 0xD7;
    static const u8 RST_8 = 0xCF;
    static u8 currentInterrupt = RST_8;
    unsigned cycleCount = 0;
    while(cycleCount < dt * CYCLES_PER_SECOND / 1000)
    {
        cycleCount += cpu->step();
        if(cpu->cycleCount() >= CYCLES_PER_FRAME / 2)
        {
            cpu->subtractFromCycleCount(CYCLES_PER_FRAME / 2);
            cpu->interrupt(currentInterrupt);
            if(currentInterrupt == RST_10) // VBLANK
            {
                updateScreen();
            }
            currentInterrupt = currentInterrupt == RST_8 ? RST_10 : RST_8;
        }
    }
}

void Emulator::render()
{
    SDL_RenderClear(renderer.get());
    SDL_RenderCopy(renderer.get(), texture.get(), nullptr, nullptr);
    SDL_RenderPresent(renderer.get());
}

void Emulator::updateScreen()
{
    auto vram = memory->getVRam();
    Uint32 * renderBuffer = nullptr;
    Uint32 format = 0;
    int pitch = 0;
    SDL_QueryTexture(texture.get(), &format, nullptr, nullptr, nullptr);
    auto pixelFormat = make_sdl_resource(SDL_AllocFormat, SDL_FreeFormat, format);
    SDL_LockTexture(texture.get(), nullptr, reinterpret_cast<void**>(&renderBuffer), &pitch);
    for(unsigned byte = 0; byte < vram.size(); byte++)
    {
        const unsigned y = (byte * 8) / DISPLAY_HEIGHT;
        const unsigned x = (byte * 8) % DISPLAY_HEIGHT;
        auto data = vram[byte];
        for(unsigned bit = 0; bit < 8; bit++)
        {
            Uint32 color = SDL_MapRGB(pixelFormat.get(), 0, 0, 0);
            unsigned px = x + bit;
            unsigned py = y;
            if((data >> bit) & 1)
            {
                if(px < 16)
                {
                    color = py < 16 || py > 134 
                        ? SDL_MapRGB(pixelFormat.get(), 255, 255, 255) 
                        : SDL_MapRGB(pixelFormat.get(), 0, 255, 0);
                }
                else if(px >= 16 && px <= 72)
                {
                    color = SDL_MapRGB(pixelFormat.get(), 0, 255, 0);
                }
                else if(px >= 192 && px < 224)
                {
                    color = SDL_MapRGB(pixelFormat.get(), 255, 0, 0);
                }
                else
                {
                    color = SDL_MapRGB(pixelFormat.get(), 255, 255, 255);
                }
            }

            const unsigned renderBufferX = py;
            const unsigned renderBufferY = -px + DISPLAY_HEIGHT - 1;
            const unsigned renderBufferIndex = renderBufferY * (pitch / sizeof(unsigned int)) + renderBufferX;
            renderBuffer[renderBufferIndex] = color;
        }
    }
    SDL_UnlockTexture(texture.get());
}

void Emulator::handleKeyEvent(const SDL_Event &event)
{
    auto key = event.key.keysym.scancode;
    auto pressed = event.type == SDL_KEYDOWN;
    switch (key)
    {
        case SDL_SCANCODE_C:
            inputs->setCredit(pressed);
            break;
        case SDL_SCANCODE_1:
            inputs->setP1Start(pressed);
            break;
        case SDL_SCANCODE_2:
            inputs->setP2Start(pressed);
            break;
        case SDL_SCANCODE_A:
            inputs->setP1Left(pressed);
            break;
        case SDL_SCANCODE_D:
            inputs->setP1Right(pressed);
            break;
        case SDL_SCANCODE_W:
            inputs->setP1Shoot(pressed);
            break;
        case SDL_SCANCODE_LEFT:
            inputs->setP2Left(pressed);
            break;
        case SDL_SCANCODE_RIGHT:
            inputs->setP2Right(pressed);
            break;
        case SDL_SCANCODE_UP:
            inputs->setP2Shoot(pressed);
            break;
        case SDL_SCANCODE_ESCAPE:
            {
                SDL_Event e;
                e.type = SDL_QUIT;
                SDL_PushEvent(&e);
            }
            break;
        default:
            break;
    }
}