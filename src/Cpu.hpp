#pragma once

#include "Registers.hpp"

class Cpu
{
    public:
        virtual ~Cpu() = default;

        virtual u8 fetchOpcode() = 0;

        virtual Registers& getRegisters() = 0;

        virtual bool isHalted() = 0;

        virtual bool interruptsEnabled() = 0;

        virtual void executeInstruction(u8 opcode) = 0;
};