#include <emscripten.h>

#include "Emulator.hpp"

Emulator emulator;

extern "C"
{
    EMSCRIPTEN_KEEPALIVE int run(u16 hiScore = 0)
    {
        emulator.reset();
        emulator.importHiScoreData(hiScore);
        emulator.run();
        Mix_CloseAudio();
        SDL_Quit();
        return 0;
    }

    EMSCRIPTEN_KEEPALIVE void reset()
    {
        u16 hiScore = emulator.exportHiScoreData();
        emulator.reset();
        emulator.importHiScoreData(hiScore);
    }

    EMSCRIPTEN_KEEPALIVE u16 exportHiScoreData()
    {
        return emulator.exportHiScoreData();
    }
}