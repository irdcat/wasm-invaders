#include "InputsImpl.hpp"

void InputsImpl::reset()
{
    input1.raw = 1 << 3;
    input2.raw = 0;
}

u8 InputsImpl::getInput(unsigned index)
{
    if(index > 2) index = 2;

    if(index == 1) return input1.raw;
    if(index == 2) return input2.raw;

    return 0xFF;
}

void InputsImpl::setCredit(bool deposit)
{
    input1.credit = deposit;
}

void InputsImpl::setShips(unsigned amount)
{
    if(amount > 6) amount = 6;
    if(amount < 3) amount = 3;

    input2.ships = amount - 3;
}

void InputsImpl::setExtraShip(bool value)
{
    input2.extraShip = value;
}

void InputsImpl::setDisplayCoinInfo(bool display)
{
    input2.displayCoinInfo = !display;
}

void InputsImpl::setP1Start(bool pressed)
{
    input1.p1start = pressed;
}

void InputsImpl::setP2Start(bool pressed)
{
    input1.p2start = pressed;
}

void InputsImpl::setP1Shoot(bool pressed)
{
    input1.p1shoot = pressed;
}

void InputsImpl::setP1Left(bool pressed)
{
    input1.p1left = pressed;
}

void InputsImpl::setP1Right(bool pressed)
{
    input1.p1right = pressed;
}

void InputsImpl::setP2Shoot(bool pressed)
{
    input2.p2shoot = pressed;
}

void InputsImpl::setP2Left(bool pressed)
{
    input2.p2left = pressed;
}

void InputsImpl::setP2Right(bool pressed)
{
    input2.p2right = pressed;
}