#include "Emulator.hpp"

#include <string>
#include <fstream>
#include <utility>
#include <SDL2/SDL.h>
#include <emscripten.h>

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
}

void Emulator::reset()
{
    cpu->reset();
    apu->reset();
    memory->reset();
    inputs->reset();
    shiftRegister->reset();

    shouldRun = initializeSdlResources(!shouldRun) && loadRoms();
}

void Emulator::run()
{
    uint64_t currentTime = 0;
    uint64_t lastTime = 0;
    uint64_t deltaTime = 0;
    while(shouldRun) {
        currentTime = SDL_GetTicks64();
        deltaTime = currentTime - lastTime;

        handleInput();
        update(deltaTime);
        render();

        emscripten_sleep(16 - currentTime);

        lastTime = currentTime;
    }
}

u16 Emulator::exportHiScoreData()
{
    const u16 hiScoreAddress = 0x20F4;
    return static_cast<u16>(memory->read(hiScoreAddress)) |
        static_cast<u16>(memory->read(hiScoreAddress + 1)) << 8;
}

void Emulator::importHiScoreData(u16 hiScore)
{
    const u16 hiScoreAddress = 0x1BF4;
    memory->write(hiScoreAddress, hiScore & 0xFF);
    memory->write(hiScoreAddress + 1, (hiScore >> 8) & 0xFF);
}

bool Emulator::initializeSdlResources(bool initSdlResources)
{
    if(!initSdlResources)
        return true;

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        return false;
    }

    if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) != 0) {
        return false;
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
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    if(!window) {
        return false;
    }
    
    renderer = make_sdl_resource(
        SDL_CreateRenderer,
        SDL_DestroyRenderer,
        window.get(),
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if(!renderer) {
        return false;
    }

    SDL_RenderSetLogicalSize(renderer.get(), DISPLAY_WIDTH, DISPLAY_HEIGHT);

    texture = make_sdl_resource(
        SDL_CreateTexture,
        SDL_DestroyTexture,
        renderer.get(),
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        DISPLAY_WIDTH,
        DISPLAY_HEIGHT);

    if(!texture) {
        return false;
    }

    return true;
}

bool Emulator::loadRoms()
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
        
        for(const auto& byte : data) {
            this->memory->write(startAddr++, byte);
        }
    };

    sounds = {
        make_sdl_resource(Mix_LoadWAV_RW, Mix_FreeChunk, SDL_RWFromFile("roms/ufo_highpitch.wav", "rb"), 1),
        make_sdl_resource(Mix_LoadWAV_RW, Mix_FreeChunk, SDL_RWFromFile("roms/shoot.wav", "rb"), 1),
        make_sdl_resource(Mix_LoadWAV_RW, Mix_FreeChunk, SDL_RWFromFile("roms/explosion.wav", "rb"), 1),
        make_sdl_resource(Mix_LoadWAV_RW, Mix_FreeChunk, SDL_RWFromFile("roms/invaderkilled.wav", "rb"), 1),
        make_sdl_resource(Mix_LoadWAV_RW, Mix_FreeChunk, SDL_RWFromFile("roms/fastinvader1.wav", "rb"), 1),
        make_sdl_resource(Mix_LoadWAV_RW, Mix_FreeChunk, SDL_RWFromFile("roms/fastinvader2.wav", "rb"), 1),
        make_sdl_resource(Mix_LoadWAV_RW, Mix_FreeChunk, SDL_RWFromFile("roms/fastinvader3.wav", "rb"), 1),
        make_sdl_resource(Mix_LoadWAV_RW, Mix_FreeChunk, SDL_RWFromFile("roms/fastinvader4.wav", "rb"), 1),
        make_sdl_resource(Mix_LoadWAV_RW, Mix_FreeChunk, SDL_RWFromFile("roms/ufo_lowpitch.wav", "rb"), 1)
    };

    for(const auto& sound : sounds) {
        if(!sound) {
            return false;
        }
    }

    for(const auto& rom : romInfoArray) {
        fileReader(rom.first, rom.second);
    }

    return true;
}

void Emulator::handleInput()
{
    static SDL_Event event;
    while(SDL_PollEvent(&event) != 0) {
        if(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
            handleKeyEvent(event);
    }
}

void Emulator::update(uint32_t dt)
{
    static const u8 RST_10 = 0xD7;
    static const u8 RST_8 = 0xCF;
    static u8 currentInterrupt = RST_8;
    unsigned cycleCount = 0;
    while(cycleCount < dt * CYCLES_PER_SECOND / 1000) {
        cycleCount += cpu->step();
        if(cpu->cycleCount() >= CYCLES_PER_FRAME / 2) {
            cpu->subtractFromCycleCount(CYCLES_PER_FRAME / 2);
            cpu->interrupt(currentInterrupt);
            if(currentInterrupt == RST_10) {
                // VBLANK Interrupt
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
    for(unsigned byte = 0; byte < vram.size(); byte++) {
        const unsigned y = (byte * 8) / DISPLAY_HEIGHT;
        const unsigned x = (byte * 8) % DISPLAY_HEIGHT;
        auto data = vram[byte];
        for(unsigned bit = 0; bit < 8; bit++) {
            Uint32 color = SDL_MapRGB(pixelFormat.get(), 0, 0, 0);
            unsigned px = x + bit;
            unsigned py = y;
            if((data >> bit) & 1) {
                if(px < 16) {
                    color = py < 16 || py > 134 
                        ? SDL_MapRGB(pixelFormat.get(), 255, 255, 255) 
                        : SDL_MapRGB(pixelFormat.get(), 0, 255, 0);
                } else if(px >= 16 && px <= 72) {
                    color = SDL_MapRGB(pixelFormat.get(), 0, 255, 0);
                } else if(px >= 192 && px < 224) {
                    color = SDL_MapRGB(pixelFormat.get(), 255, 0, 0);
                } else {
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
    switch (key) {
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
        default:
            break;
    }
}