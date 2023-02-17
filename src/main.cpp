#include "Emulator.hpp"

#include <iostream>
#include <stdexcept>
#include <SDL2/SDL.h>

int main()
{
    try 
    {
        Emulator emulator;
        emulator.run();
    } 
    catch(const std::runtime_error& ex)
    {
        SDL_Log("Something went wrong: %s", ex.what());
    }
    catch(const std::ios_base::failure& ex)
    {
        SDL_Log("Something went wrong while reading roms: %s", ex.what());
    }
}