#include "Emulator.hpp"

#include <iostream>
#include <stdio.h>
#include <stdexcept>
#include <SDL2/SDL.h>

int main()
{
    std::cout << "Starting emulator..." << std::endl;
    Emulator emulator;
    emulator.run();
}