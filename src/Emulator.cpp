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
    auto currentTime = std::chrono::steady_clock::now();
    auto lastTime = currentTime;
    while(shouldRun)
    {
        currentTime = std::chrono::steady_clock::now();
        auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime);

        handleInput();
        update(deltaTime);
        render();

        #ifdef __EMSCRIPTEN__
        emscripten_sleep(0);
        #endif

        lastTime = std::chrono::steady_clock::now();
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

}

void Emulator::update(std::chrono::milliseconds dt)
{

}

void Emulator::render()
{
    
}