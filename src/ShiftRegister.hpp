#pragma once

#include "Types.hpp"

class ShiftRegister
{
    public:
        virtual ~ShiftRegister() = default;

        virtual u8 readShiftResult() const = 0;

        virtual void writeShiftOffset(u8 value) = 0;

        virtual void writeShiftData(u8 value) = 0;
};