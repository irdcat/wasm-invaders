#pragma once

template <typename TRaw, typename THigh, typename TLow>
class RegisterPair
{
    union
    {
        struct
        {
            TLow low;
            THigh high;
        };
        TRaw raw;
    };

    public:
        RegisterPair() = default;

        ~RegisterPair() = default;

        operator TRaw() const;

        RegisterPair& operator=(TRaw value);

        TRaw& getRaw();

        THigh& getHigh();

        TLow& getLow();
};

#include "RegisterPair.inl"