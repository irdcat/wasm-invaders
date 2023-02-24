#pragma once

#include <gmock/gmock.h>
#include "../../src/Bus.hpp"

class BusMock : public Bus
{
    public:
        MOCK_METHOD(u8, readFromMemory, (u16 addr), (const, override));
        MOCK_METHOD(void, writeIntoMemory, (u16 addr, u8 value), (override));
        MOCK_METHOD(u8, readFromInputPort, (u8 port), (const, override));
        MOCK_METHOD(void, writeIntoOutputPort, (u8 port, u8 value), (override));
        MOCK_METHOD(u8&, getMemoryLocationRef, (u16 addr), (override));
};