#include "MemoryImpl.hpp"

u8 MemoryImpl::read(u16 addr) const
{
    if (addr < ROM_START_ADDR + ROM_SIZE)
        return rom[addr - ROM_START_ADDR];
    else if (addr < RAM_START_ADDR + RAM_SIZE)
        return ram[addr - RAM_START_ADDR];
    
    return ram[addr - VRAM_START_ADDR];
}

void MemoryImpl::write(u16 addr, u8 value)
{
    u8& ref = memoryRef(addr);
    ref = value;
}

u8& MemoryImpl::memoryRef(u16 addr)
{
    if (addr < ROM_START_ADDR + ROM_SIZE)
        return rom[addr - ROM_START_ADDR];
    else if (addr < RAM_START_ADDR + RAM_SIZE)
        return ram[addr - RAM_START_ADDR];
    
    return ram[addr - VRAM_START_ADDR];
}