#include "MemoryImpl.hpp"
#include <algorithm>

void MemoryImpl::reset()
{
    auto clear = [](auto& n){ n = 0x00; };

    std::for_each(rom.begin(), rom.end(), clear);
    std::for_each(ram.begin(), ram.end(), clear);
    std::for_each(vram.begin(), vram.end(), clear);
}

u8 MemoryImpl::read(u16 addr) const
{
    if (addr < ROM_START_ADDR + ROM_SIZE)
        return rom[addr - ROM_START_ADDR];
    else if (addr < RAM_START_ADDR + RAM_SIZE)
        return ram[addr - RAM_START_ADDR];
    else if (addr < VRAM_START_ADDR + VRAM_SIZE)
        return vram[addr - VRAM_START_ADDR];
    else if (addr >= 0x6000)
        return 0;

    // RAM/VRAM Mirror
    return read(addr - 0x2000);
}

void MemoryImpl::write(u16 addr, u8 value)
{
    u8& ref = memoryRef(addr);
    ref = value;
}

u8& MemoryImpl::memoryRef(u16 addr)
{
    static u8 nullByte = 0;

    if (addr < ROM_START_ADDR + ROM_SIZE)
        return rom[addr - ROM_START_ADDR];
    else if (addr < RAM_START_ADDR + RAM_SIZE)
        return ram[addr - RAM_START_ADDR];
    else if (addr < VRAM_START_ADDR + VRAM_SIZE)
        return vram[addr - VRAM_START_ADDR];
    else if (addr >= 0x6000)
        return nullByte;
    
    // RAM/VRAM Mirror
    return memoryRef(addr - 0x2000);
}

const std::array<u8, 0x1C00>& MemoryImpl::getVRam() const
{
    return vram;
}