#pragma once

#include <array>
#include "Memory.hpp"

class MemoryImpl : public Memory
{
    public:
        u8 read(u16 addr) const override;

        void write(u16 addr, u8 value) override;

        u8& memoryRef(u16 addr) override;

        const std::array<u8, 0x1C00>& getVRam() const override; 

    private:
        Rom rom;
        Ram ram;
        VRam vram;
};