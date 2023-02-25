#include <gtest/gtest.h>

#include "../src/ShiftRegisterImpl.hpp"

class ShiftRegisterImplTest : public testing::Test
{
    protected:
        void SetUp() override
        {
            shiftRegister = std::make_unique<ShiftRegisterImpl>();
        }

        std::unique_ptr<ShiftRegisterImpl> shiftRegister;
};

TEST_F(ShiftRegisterImplTest, shiftsCorrectlyWithoutOffset)
{
    shiftRegister->writeShiftData(0x99);
    shiftRegister->writeShiftData(0x88);

    auto result = shiftRegister->readShiftResult();

    EXPECT_EQ(0x88, result);
}

TEST_F(ShiftRegisterImplTest, shiftsCorrectlyWithOffset)
{
    shiftRegister->writeShiftData(0x99);
    shiftRegister->writeShiftData(0x88);
    shiftRegister->writeShiftOffset(2);

    auto result = shiftRegister->readShiftResult();

    EXPECT_EQ(0x22, result);
}

TEST_F(ShiftRegisterImplTest, usesOnlyThreeBitsOfOffset)
{
    shiftRegister->writeShiftData(0x99);
    shiftRegister->writeShiftData(0x88);
    shiftRegister->writeShiftOffset(15);

    auto result = shiftRegister->readShiftResult();

    EXPECT_EQ(0x4C, result);
}