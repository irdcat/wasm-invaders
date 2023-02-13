#pragma once

#include "Types.hpp"

class Memory
{
    public:
        virtual ~Memory() = default;

        virtual u8 read(u16 addr) const = 0;

        virtual void write(u16 addr, u8 value) = 0;

        virtual u8& memoryRef(u16 addr) = 0;
};