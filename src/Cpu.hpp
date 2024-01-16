#pragma once

#include "Registers.hpp"

class Cpu
{
    public:
        virtual ~Cpu() = default;

        virtual void reset() = 0;

        virtual u8 fetchOpcode() = 0;

        virtual Registers& getRegisters() = 0;

        virtual unsigned cycleCount() = 0;

        virtual void subtractFromCycleCount(unsigned value) = 0;

        virtual unsigned step() = 0;

        virtual unsigned executeInstruction(u8 opcode) = 0;

        virtual void interrupt(u8 interrupt_source) = 0;

        virtual bool isHalted() const = 0;
};