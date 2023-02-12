#pragma once

#include "Types.hpp"

union FlagRegister
{
    struct
    {
        u8 C : 1; // Carry
        u8 : 1;
        u8 P : 1; // Parity
        u8 : 1;
        u8 AC : 1; // Aux-Carry
        u8 : 1;
        u8 Z : 1; // Zero
        u8 S : 1; // Sign
    };
    u8 raw;
};