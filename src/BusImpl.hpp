#pragma once

#include <memory>
#include "Bus.hpp"
#include "Memory.hpp"

class BusImpl : public Bus
{
    public:
        BusImpl(const std::shared_ptr<Memory>& memoryPtr);

        ~BusImpl() = default;

        u8 readFromMemory(u16 addr) const override;

        void writeIntoMemory(u16 addr, u8 value) override;

        u8 readFromInputPort(u8 port) const override;

        void writeIntoOutputPort(u8 port, u8 value) override;

        u8& getMemoryLocationRef(u16 addr) override;

    private:
        std::shared_ptr<Memory> memory;
};