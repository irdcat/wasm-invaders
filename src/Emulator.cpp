#include "Emulator.hpp"

#include <string>
#include <fstream>
#include <utility>
#include <system_error>
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
{
    shiftRegister = std::make_shared<ShiftRegisterImpl>();
    inputs = std::make_shared<InputsImpl>();
    memory = std::make_shared<MemoryImpl>();
    apu = std::make_shared<ApuImpl>([](unsigned index){
        //TODO: Audio play callback
    });
    bus = std::make_shared<BusImpl>(memory, inputs, apu, shiftRegister);
    cpu = std::make_shared<CpuImpl>(bus);

    initializeSdlResources();
    loadRoms();
    shouldRun = true;
}

Emulator::~Emulator()
{
    SDL_Quit();
}

void Emulator::run()
{
    uint32_t currentTime = 0;
    uint32_t lastTime = 0;
    uint32_t deltaTime = 0;
    while(shouldRun)
    {
        currentTime = SDL_GetTicks();
        deltaTime = currentTime - lastTime;

        handleInput();
        update(deltaTime);
        render();

        emscripten_sleep(0);

        lastTime = currentTime;
    }
}

void Emulator::initializeSdlResources()
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) 
        throw std::system_error(
            errno, 
            std::generic_category(), 
            std::string("Unable to initialize SDL: ") + std::string(SDL_GetError())
            );

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
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    SDL_RenderSetLogicalSize(renderer.get(), DISPLAY_WIDTH, DISPLAY_HEIGHT);

    texture = make_sdl_resource(
        SDL_CreateTexture,
        SDL_DestroyTexture,
        renderer,
        SDL_PIXELFORMAT_ARGB32,
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
    // TODO: Update buffer and emulated environment
}

void Emulator::render()
{
    // TODO: Rendering
}

void Emulator::handleKeyEvent(SDL_Event &event)
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
        default:
            break;
    }
}