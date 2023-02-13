#pragma once

#include "Types.hpp"

template<unsigned BitNo, unsigned BitCount, typename T>
struct RegisterBit
{
    T raw;

    enum : unsigned
    {
        mask = 1 << BitCount - 1;
    };

    template <typename U>
    RegisterBit& operator=(U value)
    {
        raw = (raw & ~(mask << BitNo)) | ((val & mask) << BitNo);
        return *this;
    }

    operator unsigned() const
    {
        return (raw >> BitNo) & mask;
    }
};