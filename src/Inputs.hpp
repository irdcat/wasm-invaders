#pragma once

#include "Types.hpp"

class Inputs
{
    public:
        virtual ~Inputs() = default;

        virtual void reset() = 0;

        virtual u8 getInput(unsigned index) = 0;

        virtual void setCredit(bool deposit) = 0;
        virtual void setShips(unsigned amount) = 0;
        virtual void setExtraShip(bool value) = 0;
        virtual void setDisplayCoinInfo(bool display) = 0;

        virtual void setP1Start(bool pressed) = 0;
        virtual void setP2Start(bool pressed) = 0;

        virtual void setP1Shoot(bool pressed) = 0;
        virtual void setP1Left(bool pressed) = 0;
        virtual void setP1Right(bool pressed) = 0;

        virtual void setP2Shoot(bool pressed) = 0;
        virtual void setP2Left(bool pressed) = 0;
        virtual void setP2Right(bool presse) = 0;
};