#pragma once

#include <array>
#include "Memory.hpp"

class MemoryImpl : public Memory
{
    public:
        MemoryImpl() = default;

        ~MemoryImpl() = default;

        void reset() override;

        u8 read(u16 addr) const override;

        void write(u16 addr, u8 value) override;

        u8& memoryRef(u16 addr) override;

        const std::array<u8, 0x1C00>& getVRam() const override; 

    private:
        Rom rom;
        Ram ram;
        VRam vram;

        static constexpr const unsigned ROM_START_ADDR = 0x0000;
        static constexpr const unsigned RAM_START_ADDR = 0x2000;
        static constexpr const unsigned VRAM_START_ADDR = 0x2400;
        static constexpr const unsigned RAM_MIRROR_START_ADDR = 0x4000;
};