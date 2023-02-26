#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mocks/BusMock.hpp"

#include "../src/CpuImpl.hpp"

using testing::Eq;
using testing::Return;

class CpuImplTest : public testing::Test
{
    protected:
        void SetUp() override
        {
            bus = std::make_shared<BusMock>();
            testedCpu = std::make_unique<CpuImpl>(bus);
        }

        std::unique_ptr<CpuImpl> testedCpu;
        std::shared_ptr<BusMock> bus;

        const u8 OPCODE_NOP = 0x00;

        const u8 OPCODE_LXI_BC = 0x01;
        const u8 OPCODE_LXI_DE = 0x11;
        const u8 OPCODE_LXI_HL = 0x21;
        const u8 OPCODE_LXI_SP = 0x31;

        const u8 OPCODE_DAD_BC = 0x09;
        const u8 OPCODE_DAD_DE = 0x19;
        const u8 OPCODE_DAD_HL = 0x29;
        const u8 OPCODE_DAD_SP = 0x39;

        const u8 OPCODE_STAX_BC = 0x02;
        const u8 OPCODE_STAX_DE = 0x12;

        const u8 OPCODE_SHLD = 0x22;

        const u8 OPCODE_STA = 0x32;

        const u8 OPCODE_LDAX_BC = 0x0A;
        const u8 OPCODE_LDAX_DE = 0x1A;

        const u8 OPCODE_LHLD = 0x2A;

        const u8 OPCODE_LDA = 0x3A;

        const u8 OPCODE_INX_BC = 0x03;
        const u8 OPCODE_INX_DE = 0x13;
        const u8 OPCODE_INX_HL = 0x23;
        const u8 OPCODE_INX_SP = 0x33;

        const u8 OPCODE_DCX_BC = 0x0B;
        const u8 OPCODE_DCX_DE = 0x1B;
        const u8 OPCODE_DCX_HL = 0x2B;
        const u8 OPCODE_DCX_SP = 0x3B;

        const u8 OPCODE_INR_B = 0x04;
        const u8 OPCODE_INR_C = 0x0C;
        const u8 OPCODE_INR_D = 0x14;
        const u8 OPCODE_INR_E = 0x1C;
        const u8 OPCODE_INR_H = 0x24;
        const u8 OPCODE_INR_L = 0x2C;
        const u8 OPCODE_INR_M = 0x34;
        const u8 OPCODE_INR_A = 0x3C;

        const u8 OPCODE_DCR_B = 0x05;
        const u8 OPCODE_DCR_C = 0x0D;
        const u8 OPCODE_DCR_D = 0x15;
        const u8 OPCODE_DCR_E = 0x1D;
        const u8 OPCODE_DCR_H = 0x25;
        const u8 OPCODE_DCR_L = 0x2D;
        const u8 OPCODE_DCR_M = 0x35;
        const u8 OPCODE_DCR_A = 0x3D;

        const u8 OPCODE_MVI_B = 0x06;
        const u8 OPCODE_MVI_C = 0x0E;
        const u8 OPCODE_MVI_D = 0x16;
        const u8 OPCODE_MVI_E = 0x1E;
        const u8 OPCODE_MVI_H = 0x26;
        const u8 OPCODE_MVI_L = 0x2E;
        const u8 OPCODE_MVI_M = 0x36;
        const u8 OPCODE_MVI_A = 0x3E;

        const u8 OPCODE_RLC = 0x07;
        const u8 OPCODE_RRC = 0x0F;
        const u8 OPCODE_RAL = 0x17;
        const u8 OPCODE_RAR = 0x1F;
        const u8 OPCODE_DAA = 0x27;
        const u8 OPCODE_CMA = 0x2F;
        const u8 OPCODE_STC = 0x37;
        const u8 OPCODE_CMC = 0x3F;

        const u8 OPCODE_MOV_B_B = 0x40;
        const u8 OPCODE_MOV_B_C = 0x41;
        const u8 OPCODE_MOV_B_D = 0x42;
        const u8 OPCODE_MOV_B_E = 0x43;
        const u8 OPCODE_MOV_B_H = 0x44;
        const u8 OPCODE_MOV_B_L = 0x45;
        const u8 OPCODE_MOV_B_M = 0x46;
        const u8 OPCODE_MOV_B_A = 0x47;

        const u8 OPCODE_MOV_C_B = 0x48;
        const u8 OPCODE_MOV_C_C = 0x49;
        const u8 OPCODE_MOV_C_D = 0x4A;
        const u8 OPCODE_MOV_C_E = 0x4B;
        const u8 OPCODE_MOV_C_H = 0x4C;
        const u8 OPCODE_MOV_C_L = 0x4D;
        const u8 OPCODE_MOV_C_M = 0x4E;
        const u8 OPCODE_MOV_C_A = 0x4F;

        const u8 OPCODE_MOV_D_B = 0x50;
        const u8 OPCODE_MOV_D_C = 0x51;
        const u8 OPCODE_MOV_D_D = 0x52;
        const u8 OPCODE_MOV_D_E = 0x53;
        const u8 OPCODE_MOV_D_H = 0x54;
        const u8 OPCODE_MOV_D_L = 0x55;
        const u8 OPCODE_MOV_D_M = 0x56;
        const u8 OPCODE_MOV_D_A = 0x57;

        const u8 OPCODE_MOV_E_B = 0x58;
        const u8 OPCODE_MOV_E_C = 0x59;
        const u8 OPCODE_MOV_E_D = 0x5A;
        const u8 OPCODE_MOV_E_E = 0x5B;
        const u8 OPCODE_MOV_E_H = 0x5C;
        const u8 OPCODE_MOV_E_L = 0x5D;
        const u8 OPCODE_MOV_E_M = 0x5E;
        const u8 OPCODE_MOV_E_A = 0x5F;

        const u8 OPCODE_MOV_H_B = 0x60;
        const u8 OPCODE_MOV_H_C = 0x61;
        const u8 OPCODE_MOV_H_D = 0x62;
        const u8 OPCODE_MOV_H_E = 0x63;
        const u8 OPCODE_MOV_H_H = 0x64;
        const u8 OPCODE_MOV_H_L = 0x65;
        const u8 OPCODE_MOV_H_M = 0x66;
        const u8 OPCODE_MOV_H_A = 0x67;

        const u8 OPCODE_MOV_L_B = 0x68;
        const u8 OPCODE_MOV_L_C = 0x69;
        const u8 OPCODE_MOV_L_D = 0x6A;
        const u8 OPCODE_MOV_L_E = 0x6B;
        const u8 OPCODE_MOV_L_H = 0x6C;
        const u8 OPCODE_MOV_L_L = 0x6D;
        const u8 OPCODE_MOV_L_M = 0x6E;
        const u8 OPCODE_MOV_L_A = 0x6F;

        const u8 OPCODE_MOV_M_B = 0x70;
        const u8 OPCODE_MOV_M_C = 0x71;
        const u8 OPCODE_MOV_M_D = 0x72;
        const u8 OPCODE_MOV_M_E = 0x73;
        const u8 OPCODE_MOV_M_H = 0x74;
        const u8 OPCODE_MOV_M_L = 0x75;
        const u8 OPCODE_MOV_M_A = 0x77;

        const u8 OPCODE_MOV_A_B = 0x78;
        const u8 OPCODE_MOV_A_C = 0x79;
        const u8 OPCODE_MOV_A_D = 0x7A;
        const u8 OPCODE_MOV_A_E = 0x7B;
        const u8 OPCODE_MOV_A_H = 0x7C;
        const u8 OPCODE_MOV_A_L = 0x7D;
        const u8 OPCODE_MOV_A_M = 0x7E;
        const u8 OPCODE_MOV_A_A = 0x7F;

        const u8 OPCODE_HLT = 0x76;

        const u8 OPCODE_ADD_B = 0x80;
        const u8 OPCODE_ADD_C = 0x81;
        const u8 OPCODE_ADD_D = 0x82;
        const u8 OPCODE_ADD_E = 0x83;
        const u8 OPCODE_ADD_H = 0x84;
        const u8 OPCODE_ADD_L = 0x85;
        const u8 OPCODE_ADD_M = 0x86;
        const u8 OPCODE_ADD_A = 0x87;

        const u8 OPCODE_ADC_B = 0x88;
        const u8 OPCODE_ADC_C = 0x89;
        const u8 OPCODE_ADC_D = 0x8A;
        const u8 OPCODE_ADC_E = 0x8B;
        const u8 OPCODE_ADC_H = 0x8C;
        const u8 OPCODE_ADC_L = 0x8D;
        const u8 OPCODE_ADC_M = 0x8E;
        const u8 OPCODE_ADC_A = 0x8F;

        const u8 OPCODE_SUB_B = 0x90;
        const u8 OPCODE_SUB_C = 0x91;
        const u8 OPCODE_SUB_D = 0x92;
        const u8 OPCODE_SUB_E = 0x93;
        const u8 OPCODE_SUB_H = 0x94;
        const u8 OPCODE_SUB_L = 0x95;
        const u8 OPCODE_SUB_M = 0x96;
        const u8 OPCODE_SUB_A = 0x97;

        const u8 OPCODE_SBB_B = 0x98;
        const u8 OPCODE_SBB_C = 0x99;
        const u8 OPCODE_SBB_D = 0x9A;
        const u8 OPCODE_SBB_E = 0x9B;
        const u8 OPCODE_SBB_H = 0x9C;
        const u8 OPCODE_SBB_L = 0x9D;
        const u8 OPCODE_SBB_M = 0x9E;
        const u8 OPCODE_SBB_A = 0x9F;

        const u8 OPCODE_ANA_B = 0xA0;
        const u8 OPCODE_ANA_C = 0xA1;
        const u8 OPCODE_ANA_D = 0xA2;
        const u8 OPCODE_ANA_E = 0xA3;
        const u8 OPCODE_ANA_H = 0xA4;
        const u8 OPCODE_ANA_L = 0xA5;
        const u8 OPCODE_ANA_M = 0xA6;
        const u8 OPCODE_ANA_A = 0xA7;

        const u8 OPCODE_XRA_B = 0xA8;
        const u8 OPCODE_XRA_C = 0xA9;
        const u8 OPCODE_XRA_D = 0xAA;
        const u8 OPCODE_XRA_E = 0xAB;
        const u8 OPCODE_XRA_H = 0xAC;
        const u8 OPCODE_XRA_L = 0xAD;
        const u8 OPCODE_XRA_M = 0xAE;
        const u8 OPCODE_XRA_A = 0xAF;

        const u8 OPCODE_ORA_B = 0xB0;
        const u8 OPCODE_ORA_C = 0xB1;
        const u8 OPCODE_ORA_D = 0xB2;
        const u8 OPCODE_ORA_E = 0xB3;
        const u8 OPCODE_ORA_H = 0xB4;
        const u8 OPCODE_ORA_L = 0xB5;
        const u8 OPCODE_ORA_M = 0xB6;
        const u8 OPCODE_ORA_A = 0xB7;

        const u8 OPCODE_CMP_B = 0xB8;
        const u8 OPCODE_CMP_C = 0xB9;
        const u8 OPCODE_CMP_D = 0xBA;
        const u8 OPCODE_CMP_E = 0xBB;
        const u8 OPCODE_CMP_H = 0xBC;
        const u8 OPCODE_CMP_L = 0xBD;
        const u8 OPCODE_CMP_M = 0xBE;
        const u8 OPCODE_CMP_A = 0xBF;

        const u8 OPCODE_RNZ = 0xC0;
        const u8 OPCODE_RZ = 0xC8;
        const u8 OPCODE_RNC = 0xD0;
        const u8 OPCODE_RC = 0xD8;
        const u8 OPCODE_RPO = 0xE0;
        const u8 OPCODE_RPE = 0xE8;
        const u8 OPCODE_RP = 0xF0;
        const u8 OPCODE_RM = 0xF8;

        const u8 OPCODE_RET = 0xC9;

        const u8 OPCODE_POP_BC = 0xC1;
        const u8 OPCODE_POP_DE = 0xD1;
        const u8 OPCODE_POP_HL = 0xE1;
        const u8 OPCODE_POP_AF = 0xF1;

        const u8 OPCODE_PUSH_BC = 0xC5;
        const u8 OPCODE_PUSH_DE = 0xD5;
        const u8 OPCODE_PUSH_HL = 0xE5;
        const u8 OPCODE_PUSH_AF = 0xF5;

        const u8 OPCODE_JNZ = 0xC2;
        const u8 OPCODE_JZ = 0xCA;
        const u8 OPCODE_JNC = 0xD2;
        const u8 OPCODE_JC = 0xDA;
        const u8 OPCODE_JPO = 0xE2;
        const u8 OPCODE_JPE = 0xEA;
        const u8 OPCODE_JP = 0xF2;
        const u8 OPCODE_JM = 0xFA;

        const u8 OPCODE_JMP = 0xC3;

        const u8 OPCODE_CNZ = 0xC4;
        const u8 OPCODE_CZ = 0xCC;
        const u8 OPCODE_CNC = 0xD4;
        const u8 OPCODE_CC = 0xDC;
        const u8 OPCODE_CPO = 0xE4;
        const u8 OPCODE_CPE = 0xEC;
        const u8 OPCODE_CP = 0xF4;
        const u8 OPCODE_CM = 0xFC;

        const u8 OPCODE_CALL = 0xCD;

        const u8 OPCODE_OUT = 0xD3;
        const u8 OPCODE_IN = 0xDB;

        const u8 OPCODE_XTHL = 0xE3;
        const u8 OPCODE_XCHG = 0xEB;

        const u8 OPCODE_DI = 0xF3;
        const u8 OPCODE_EI = 0xFB;

        const u8 OPCODE_PCHL = 0xE9;
        const u8 OPCODE_SPHL = 0xF9;

        const u8 OPCODE_ADI = 0xC6;
        const u8 OPCODE_ACI = 0xCE;
        const u8 OPCODE_SUI = 0xD6;
        const u8 OPCODE_SBI = 0xDE;
        const u8 OPCODE_ANI = 0xE6;
        const u8 OPCODE_XRI = 0xEE;
        const u8 OPCODE_ORI = 0xF6;
        const u8 OPCODE_CPI = 0xFE;

        const u8 OPCODE_RST_0 = 0xC7;
        const u8 OPCODE_RST_1 = 0xCF;
        const u8 OPCODE_RST_2 = 0xD7;
        const u8 OPCODE_RST_3 = 0xDF;
        const u8 OPCODE_RST_4 = 0xE7;
        const u8 OPCODE_RST_5 = 0xEF;
        const u8 OPCODE_RST_6 = 0xF7;
        const u8 OPCODE_RST_7 = 0xFF; 
};

TEST_F(CpuImplTest, testFetchOpcode)
{
    auto& pc = testedCpu->getRegisters().getPc();
    pc = 0x100;
    EXPECT_CALL(*bus, readFromMemory(Eq(0x100))).Times(1).WillOnce(Return(0x77));

    auto result = testedCpu->fetchOpcode();
    EXPECT_EQ(0x77, result);
}