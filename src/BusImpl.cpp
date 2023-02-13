#include "BusImpl.hpp"

BusImpl::BusImpl(const std::shared_ptr<Memory>& memoryPtr)
    : memory(memoryPtr)
{
}

u8 BusImpl::readFromMemory(u16 addr) const
{
    return memory->read(addr);
}

void BusImpl::writeIntoMemory(u16 addr, u8 value)
{
    memory->write(addr, value);
}

u8 BusImpl::readFromInputPort(u8 port) const
{
    return u8();
}

void BusImpl::writeIntoOutputPort(u8 port, u8 value)
{
}

u8& BusImpl::getMemoryLocationRef(u16 addr)
{
    memory->memoryRef(addr);
}
