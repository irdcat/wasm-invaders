#pragma once

#include <functional>
#include "Apu.hpp"

class ApuImpl : public Apu
{
    public:
        using AudioPlayCallback = std::function<void(unsigned index)>;

        explicit ApuImpl(AudioPlayCallback audioPlayCallback);

        void reset() override;

        ~ApuImpl() = default;

        void write(unsigned bank, u8 value) override;

    private:
        union 
        {
            struct
            {
                u8 ufo : 1;
                u8 shot : 1;
                u8 playerDie : 1;
                u8 invaderDie : 1;
                u8 extendedPlay : 1;
                u8 ampEnable : 1;
                u8 : 2;
            };
            u8 raw;
        } sound1;

        union 
        {
            struct 
            {
                u8 fleetMovement1 : 1;
                u8 fleetMovement2 : 1;
                u8 fleetMovement3 : 1;
                u8 fleetMovement4 : 1;
                u8 ufoHit : 1;
                u8 : 3;
            };
            u8 raw;
        } sound2;

        AudioPlayCallback audioPlayCallback;
};