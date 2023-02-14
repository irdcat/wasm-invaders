#pragma once

#include "Types.hpp"

class Apu
{
    public:
        virtual ~Apu() = default;

        virtual void write(unsigned bank, u8 value) = 0;
};