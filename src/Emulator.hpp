#pragma once

#include <memory>
#include <SDL2/SDL_mixer.h>

#include "Apu.hpp"
#include "Bus.hpp"
#include "Cpu.hpp"
#include "Inputs.hpp"
#include "Memory.hpp"
#include "ShiftRegister.hpp"
#include "SdlResource.hpp"

class Emulator
{
    public:
        Emulator();

        ~Emulator();

        void run();

    private:
        void initializeSdlResources();
        void loadRoms();

        void handleInput();
        void update(uint32_t ms);
        void render();

        void updateScreen();

        void handleKeyEvent(const SDL_Event &event);

        bool shouldRun;

        std::shared_ptr<Cpu> cpu;
        std::shared_ptr<Bus> bus;
        std::shared_ptr<Memory> memory;
        std::shared_ptr<Inputs> inputs;
        std::shared_ptr<Apu> apu;
        std::shared_ptr<ShiftRegister> shiftRegister;

        SdlResource<SDL_Texture> texture;
        SdlResource<SDL_Renderer> renderer;
        SdlResource<SDL_Window> window;

        std::array<SdlResource<Mix_Chunk>, 9> sounds;

        static const constexpr int AUDIO_VOLUME = MIX_MAX_VOLUME / 2;
        static const constexpr unsigned DISPLAY_WIDTH = 224;
        static const constexpr unsigned DISPLAY_HEIGHT = 256;
        static const constexpr unsigned PIXEL_SIZE = 2;
        static const constexpr unsigned FRAMES_PER_SECOND = 60;
        static const constexpr unsigned CYCLES_PER_SECOND = 1996800;
        static const constexpr unsigned CYCLES_PER_FRAME = CYCLES_PER_SECOND / FRAMES_PER_SECOND;
};