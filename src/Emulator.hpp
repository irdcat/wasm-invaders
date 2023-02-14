#pragma once

#include <memory>

#include "Apu.hpp"
#include "Bus.hpp"
#include "Cpu.hpp"
#include "Inputs.hpp"
#include "Memory.hpp"
#include "ShiftRegister.hpp"

class Emulator
{
    public:
        Emulator();

        ~Emulator() = default;

    private:
        std::shared_ptr<Cpu> cpu;
        std::shared_ptr<Bus> bus;
        std::shared_ptr<Memory> memory;
        std::shared_ptr<Inputs> inputs;
        std::shared_ptr<Apu> apu;
        std::shared_ptr<ShiftRegister> shiftRegister;
};