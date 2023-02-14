#pragma once

#include <memory>
#include "Bus.hpp"
#include "Apu.hpp"
#include "Memory.hpp"
#include "Inputs.hpp"
#include "ShiftRegister.hpp"

class BusImpl : public Bus
{
    public:
        BusImpl(const std::shared_ptr<Memory>& memoryPtr,
                const std::shared_ptr<Inputs>& inputsPtr, 
                const std::shared_ptr<Apu>& apuPtr,
                const std::shared_ptr<ShiftRegister>& shiftRegisterPtr);

        ~BusImpl() = default;

        u8 readFromMemory(u16 addr) const override;

        void writeIntoMemory(u16 addr, u8 value) override;

        u8 readFromInputPort(u8 port) const override;

        void writeIntoOutputPort(u8 port, u8 value) override;

        u8& getMemoryLocationRef(u16 addr) override;

    private:
        std::shared_ptr<Memory> memory;
        std::shared_ptr<Inputs> inputs;
        std::shared_ptr<Apu> apu;
        std::shared_ptr<ShiftRegister> shiftRegister;
};