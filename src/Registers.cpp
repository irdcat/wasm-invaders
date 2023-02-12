#include "Registers.hpp"

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
