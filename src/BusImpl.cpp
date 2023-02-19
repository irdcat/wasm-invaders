#include "BusImpl.hpp"

BusImpl::BusImpl(const std::shared_ptr<Memory>& memoryPtr,
                const std::shared_ptr<Inputs>& inputsPtr,
                const std::shared_ptr<Apu>& apuPtr,
                const std::shared_ptr<ShiftRegister>& shiftRegisterPtr)
    : memory(memoryPtr)
    , inputs(inputsPtr)
    , apu(apuPtr)
    , shiftRegister(shiftRegisterPtr)
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
    switch(port)
    {
        case 0x0: // INPUT0 - Never used by invaders code despite being mapped in the hardware
        case 0x1: // INPUT1
        case 0x2: // INPUT2
            return inputs->getInput(port);
        case 0x3: // SHFTIN
            return shiftRegister->readShiftResult();
    }
    return u8();
}

void BusImpl::writeIntoOutputPort(u8 port, u8 value)
{
    switch (port)
    {
        case 0x2: // SHFTAMNT
            shiftRegister->writeShiftOffset(value);
            break;
        case 0x3: // SOUND1
            apu->write(0x1, value);
            break;
        case 0x4: // SHFTDATA
            shiftRegister->writeShiftData(value);
            break;
        case 0x5: // SOUND2
            apu->write(0x2, value);
            break;
        case 0x6: // WATCHDOG
            // Original hardware had wired reset of watchdog timer in this port
            // Not neccessary here
            break;
    }
}

u8& BusImpl::getMemoryLocationRef(u16 addr)
{
    return memory->memoryRef(addr);
}
