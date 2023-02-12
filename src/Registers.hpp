#pragma once

#include "RegisterPair.hpp"
#include "FlagRegister.hpp"

class Registers
{
    RegisterPair<u16, u8, FlagRegister> af;
    RegisterPair<u16, u8, u8> bc;
    RegisterPair<u16, u8, u8> de;
    RegisterPair<u16, u8, u8> hl;

    u16 sp;
    u16 pc;

    public:
        Registers() = default;

        ~Registers() = default;

        RegisterPair<u16, u8, FlagRegister>& getAf();

        RegisterPair<u16, u8, u8>& getBc();

        RegisterPair<u16, u8, u8>& getDe();

        RegisterPair<u16, u8, u8>& getHl();

        u16& getPc();

        u16& getSp();
};