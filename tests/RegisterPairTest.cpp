#include <gtest/gtest.h>

#include "../src/Types.hpp"
#include "../src/RegisterPair.hpp"

class RegisterPairTest : public testing::Test
{
    protected:
        void SetUp() override 
        {
        }
};

TEST_F(RegisterPairTest, alignmentOfRegistersInPairIsCorrect)
{
    auto registerPair = RegisterPair<u16, u8, u8>();
    registerPair = 0x9988;

    EXPECT_EQ(0x88, registerPair.getLow());
    EXPECT_EQ(0x99, registerPair.getHigh());
}