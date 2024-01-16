#include "ApuImpl.hpp"

#include <array>

ApuImpl::ApuImpl(AudioPlayCallback audioPlayCallback)
    : audioPlayCallback(audioPlayCallback)
{
    reset();
}

void ApuImpl::reset()
{
    sound1.raw = 0x00;
    sound2.raw = 0x00;
}

void ApuImpl::write(unsigned bank, u8 value)
{
    static const u8 BITS[5] = { 0x01, 0x02, 0x04, 0x08, 0x10 };
    unsigned soundToPlay = 0xF;
    if(bank == 1)
    {
        if(value & BITS[0] && (sound1.ufo = ~sound1.ufo))
            soundToPlay = 0;
        else if(value & BITS[1] && (sound1.shot = ~sound1.shot))
            soundToPlay = 1;
        else if(value & BITS[2] && (sound1.playerDie = ~sound1.playerDie))
            soundToPlay = 2;
        else if(value & BITS[3] && (sound1.invaderDie = ~sound1.invaderDie))
            soundToPlay = 3;
    }
    else if (bank == 2)
    {
        if(value & BITS[0] && (sound2.fleetMovement1 = ~sound2.fleetMovement1))
            soundToPlay = 4;
        else if(value & BITS[1] && (sound2.fleetMovement2 = ~sound2.fleetMovement2))
            soundToPlay = 5;
        else if(value & BITS[2] && (sound2.fleetMovement3 = ~sound2.fleetMovement3))
            soundToPlay = 6;
        else if(value & BITS[3] && (sound2.fleetMovement4 = ~sound2.fleetMovement4))
            soundToPlay = 7;
        else if(value & BITS[4] && (sound2.ufoHit = ~sound2.ufoHit))
            soundToPlay = 8;    
    }

    if (soundToPlay != 0xF)
        audioPlayCallback(soundToPlay);
}