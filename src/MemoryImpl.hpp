#pragma once

#include <array>
#include "Memory.hpp"

class MemoryImpl : public Memory
{
    public:
        u8 read(u16 addr) const override;

        void write(u16 addr, u8 value) override;

        u8& memoryRef(u16 addr) override;

    private:
        static constexpr const unsigned ROM_SIZE = 0x2000;
        static constexpr const unsigned RAM_SIZE = 0x0400;
        static constexpr const unsigned VRAM_SIZE = 0x1C00;

        static constexpr const unsigned ROM_START_ADDR = 0x0000;
        static constexpr const unsigned RAM_START_ADDR = 0x2000;
        static constexpr const unsigned VRAM_START_ADDR = 0x2400;

        std::array<u8, ROM_SIZE> rom;
        std::array<u8, RAM_SIZE> ram;
        std::array<u8, VRAM_SIZE> vram;
};