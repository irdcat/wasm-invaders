#include "Registers.hpp"

Registers::Registers()
    : af(0x0000)
    , bc(0x0000)
    , de(0x0000)
    , hl(0x0000)
    , pc(0x0000)
    , sp(0x0000)
{
}

RegisterPair<u16, u8, FlagRegister>& Registers::getAf()
{
    return af;
}

RegisterPair<u16, u8, u8>& Registers::getBc()
{
    return bc;
}

RegisterPair<u16, u8, u8>& Registers::getDe()
{
    return de;
}

RegisterPair<u16, u8, u8>& Registers::getHl()
{
    return hl;
}

u16& Registers::getPc() 
{
    return pc;
}

u16& Registers::getSp()
{
    return sp;
}
