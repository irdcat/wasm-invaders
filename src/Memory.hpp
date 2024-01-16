#pragma once

#include <array>
#include "Types.hpp"

constexpr const unsigned ROM_SIZE = 0x2000;
constexpr const unsigned RAM_SIZE = 0x0400;
constexpr const unsigned VRAM_SIZE = 0x1C00;

using Rom = std::array<u8, ROM_SIZE>;
using Ram = std::array<u8, RAM_SIZE>;
using VRam = std::array<u8, VRAM_SIZE>;

class Memory
{
    public:
        virtual ~Memory() = default;

        virtual void reset() = 0;

        virtual u8 read(u16 addr) const = 0;

        virtual void write(u16 addr, u8 value) = 0;

        virtual u8& memoryRef(u16 addr) = 0;

        virtual const VRam& getVRam() const = 0;
};