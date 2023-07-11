#include <emscripten.h>

#include "Emulator.hpp"

extern "C"
{
    EMSCRIPTEN_KEEPALIVE int run()
    {
        Emulator emulator;
        emulator.run();
        Mix_CloseAudio();
        SDL_Quit();
        return 0;
    }
}