#pragma once

#include "Inputs.hpp"

class InputsImpl : public Inputs
{
    public:
        InputsImpl() = default;

        ~InputsImpl() = default;

        void reset() override;

        u8 getInput(unsigned index) override;

        void setCredit(bool deposit) override;
        void setShips(unsigned amount) override;
        void setExtraShip(bool value) override;
        void setDisplayCoinInfo(bool display) override;

        void setP1Start(bool pressed) override;
        void setP2Start(bool pressed) override;

        void setP1Shoot(bool pressed) override;
        void setP1Left(bool pressed) override;
        void setP1Right(bool pressed) override;

        void setP2Shoot(bool pressed) override;
        void setP2Left(bool pressed) override;
        void setP2Right(bool presse) override;

    private:
        union
        {
            struct
            {
                u8 credit : 1;
                u8 p2start : 1;
                u8 p1start : 1;
                u8 : 1;
                u8 p1shoot : 1;
                u8 p1left : 1;
                u8 p1right : 1;
                u8 : 1;
            };
            u8 raw;
        } input1;

        union
        {
            struct 
            {
                u8 ships : 2;
                u8 tilt : 1;
                u8 extraShip : 1;
                u8 p2shoot : 1;
                u8 p2left : 1;
                u8 p2right : 1;
                u8 displayCoinInfo : 1;
            };
            u8 raw;
        } input2;
};