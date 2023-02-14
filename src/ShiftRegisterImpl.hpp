#pragma once

#include "ShiftRegister.hpp"

class ShiftRegisterImpl : public ShiftRegister
{
    public:
        ShiftRegisterImpl();

        ~ShiftRegisterImpl() = default;

        u8 readShiftResult() const override;

        void writeShiftOffset(u8 value) override;

        void writeShiftData(u8 value) override;

    private:
        u8 shift_low;
        u8 shift_high;
        u8 shift_offset;
};