#include <array>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include <iomanip>

#include "../../src/CpuImpl.hpp"
#include "../../src/Bus.hpp"

#define NOP  case 0x00:
#define LXI  case 0x01: case 0x11: case 0x21: case 0x31:
#define DAD  case 0x09: case 0x19: case 0x29: case 0x39:
#define STAX case 0x02: case 0x12:
#define SHLD case 0x22:
#define STA  case 0x32:
#define LDAX case 0x0A: case 0x1A:
#define LHLD case 0x2A:
#define LDA  case 0x3A:
#define INX  case 0x03: case 0x13: case 0x23: case 0x33:
#define DCX  case 0x0B: case 0x1B: case 0x2B: case 0x3B:
#define INR  case 0x04: case 0x0C: case 0x14: case 0x1C: \
             case 0x24: case 0x2C: case 0x34: case 0x3C:
#define DCR  case 0x05: case 0x0D: case 0x15: case 0x1D: \
             case 0x25: case 0x2D: case 0x35: case 0x3D:
#define MVI  case 0x06: case 0x0E: case 0x16: case 0x1E: \
             case 0x26: case 0x2E: case 0x36: case 0x3E:
#define BIT_INSTRUCTIONS \
             case 0x07: case 0x0F: case 0x17: case 0x1F: \
             case 0x27: case 0x2F: case 0x37: case 0x3F:
#define MOV  case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47: \
             case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x4F: \
             case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57: \
             case 0x58: case 0x59: case 0x5A: case 0x5B: case 0x5C: case 0x5D: case 0x5E: case 0x5F: \
             case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67: \
             case 0x68: case 0x69: case 0x6A: case 0x6B: case 0x6C: case 0x6D: case 0x6E: case 0x6F: \
             case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x77: case 0x78: \
             case 0x79: case 0x7A: case 0x7B: case 0x7C: case 0x7D: case 0x7E: case 0x7F: 
#define ARITHMETIC_INSTRUCTIONS \
             case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87: \
             case 0x88: case 0x89: case 0x8A: case 0x8B: case 0x8C: case 0x8D: case 0x8E: case 0x8F: \
             case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97: \
             case 0x98: case 0x99: case 0x9A: case 0x9B: case 0x9C: case 0x9D: case 0x9E: case 0x9F: \
             case 0xA0: case 0xA1: case 0xA2: case 0xA3: case 0xA4: case 0xA5: case 0xA6: case 0xA7: \
             case 0xA8: case 0xA9: case 0xAA: case 0xAB: case 0xAC: case 0xAD: case 0xAE: case 0xAF: \
             case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB6: case 0xB7: \
             case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC: case 0xBD: case 0xBE: case 0xBF:
#define HLT  case 0x76:
#define RETURNS \
             case 0xC0: case 0xC8: case 0xD0: case 0xD8: case 0xE0: case 0xE8: case 0xF0: case 0xF8: \
             case 0xC9:
#define POP  case 0xC1: case 0xD1: case 0xE1: case 0xF1:
#define PCHL case 0xE9:
#define SPHL case 0xF9:
#define JUMPS \
             case 0xC2: case 0xCA: case 0xD2: case 0xDA: case 0xE2: case 0xEA: case 0xF2: case 0xFA: \
             case 0xC3:
#define OUT  case 0xD3:
#define XTHL case 0xE3:
#define DI   case 0xF3:
#define IN   case 0xDB:
#define XCHG case 0xEB:
#define EI   case 0xFB:
#define CALLS \
             case 0xC4: case 0xCC: case 0xD4: case 0xDC: case 0xE4: case 0xEC: case 0xF4: case 0xFC: \
             case 0xCD:
#define PUSH case 0xC5: case 0xD5: case 0xE5: case 0xF5:
#define IMMEDATE_ARITHMETIC_INSTRUCTIONS \
             case 0xC6: case 0xCE: case 0xD6: case 0xDE: case 0xE6: case 0xEE: case 0xF6: case 0xFE:
#define RST  case 0xC7: case 0xCF: case 0xD7: case 0xDF: case 0xE7: case 0xEF: case 0xF7: case 0xFF:
             

class DiagnosticBusImpl
    : public Bus
{
    public:
        DiagnosticBusImpl() = default;

        ~DiagnosticBusImpl() = default;

        u8 readFromMemory(u16 addr) const override { return memory[addr & 0xFFFF]; }
        void writeIntoMemory(u16 addr, u8 value) override { memory[addr & 0xFFFF] = value; }

        u8 readFromInputPort(u8 port) const override { return 0; }
        void writeIntoOutputPort(u8 port, u8 value) override { }

        u8& getMemoryLocationRef(u16 addr) override { return memory[addr & 0xFFFF]; }

        void clearMemory() { for(auto& byte : memory) byte = 0; }

    private:
        std::array<u8, 0x10000> memory;
};

class DiagnosticCpuImpl
    : public CpuImpl
{
    public:
        ~DiagnosticCpuImpl() = default;

        DiagnosticCpuImpl(const std::shared_ptr<Bus>& bus)
            : CpuImpl(bus)
        {
            executionLogFile = std::ofstream("execution-log.txt", std::ios::trunc);
            if(!executionLogFile.is_open())
            {
                std::cout << "Unable to open file execution-log.txt" << std::endl;
                executionLogFile.exceptions(executionLogFile.failbit);
            }
        }

        unsigned executeInstruction(u8 opcode) override
        {
            // Already fetched the opcode and incremented program counter
            auto pc = getRegisters().getPc() - 1;
            u8 * mem = &bus->getMemoryLocationRef(pc);

            static const std::array<std::string, 8> registerTable = { 
                "B", "C", "D", "E", "H", "L", "(HL)", "A" 
                };
            static const std::array<std::string, 4> registerPairTable1 = { 
                "BC", "DE", "HL", "SP" 
                };
            static const std::array<std::string, 4> registerPairTable2 = { 
                "BC", "DE", "HL", "AF" 
                };
            static const std::array<std::string, 8> arithmeticInstructionsTable = {
                "ADD", "ADC", "SUB", "SBB", "ANA", "XRA", "ORA", "CMP"
                };
            static const std::array<std::string, 8> immedateArithmeticInstructionsTable = {
                "ADI", "ACI", "SUI", "SBI", "ANI", "XRI", "ORI", "CPI"
                };
            static const std::array<std::string, 8> conditionalJumpsTable = {
                "JNZ", "JZ", "JNC", "JC", "JPO", "JPE", "JP", "JM"
                };
            static const std::array<std::string, 8> conditionalCallsTable = {
                "CNZ", "CZ", "CNC", "CC", "CPO", "CPE", "CP", "CM"
                };
            static const std::array<std::string, 8> conditionalReturnsTable = {
                "RNZ", "RZ", "RNC", "RC", "RPO", "RPE", "RP", "RM"
                };
            static const std::array<std::string, 8> bitInstructionsTable = {
                "RLC", "RRC", "RAL", "RAR", "DAA", "CMA", "STC", "CMC"
                };

            executionLogFile
                << "0x" 
                << std::setfill('0') << std::setw(4) << std::uppercase << std::hex << pc 
                << " : ";

            switch(opcode)
            {
                NOP
                    executionLogFile << "NOP";
                    break;
                LXI
                {
                    u16 operand = (*(mem + 1) << 8) | (*(mem + 2));
                    executionLogFile << "LXI " << registerPairTable1[(opcode >> 4) & 0x3] << ", " << operand;
                    break;
                }
                DAD
                    executionLogFile << "DAD " << registerPairTable1[(opcode >> 4) & 0x3];
                    break;
                STAX
                    executionLogFile << "STAX " << registerPairTable1[(opcode >> 4) & 0x3];
                    break;
                SHLD
                {
                    u16 destinationAddr = (*(mem + 1) << 8) | (*(mem + 2));
                    executionLogFile << "SHLD " << destinationAddr;
                    break;
                }
                STA
                {
                    u16 destinationAddr = (*(mem + 1) << 8) | (*(mem + 2));
                    executionLogFile << "STA " << destinationAddr;
                    break;
                }
                LDAX
                    executionLogFile << "LDAX " << registerPairTable1[(opcode >> 4) & 0x3];
                    break;
                LHLD
                {
                    u16 destinationAddr = (*(mem + 1) << 8) | (*(mem + 2));
                    executionLogFile << "LHLD " << destinationAddr;
                    break;
                }
                LDA
                {
                    u16 destinationAddr = (*(mem + 1) << 8) | (*(mem + 2));
                    executionLogFile << "LDA " << destinationAddr;
                    break;
                }
                INX
                    executionLogFile << "INX " << registerPairTable1[(opcode >> 4) & 0x3];
                    break;
                DCX
                    executionLogFile << "DCX " << registerPairTable1[(opcode >> 4) & 0x3];
                    break;
                INR
                    executionLogFile << "INR " << registerTable[(opcode >> 3 & 0x7)];
                    break;
                DCR
                    executionLogFile << "DCR " << registerTable[(opcode >> 3 & 0x7)];
                    break;
                MVI
                    executionLogFile << "MVI " << registerTable[(opcode >> 3 & 0x7)] << ", " << (unsigned)*(mem + 1);
                    break;
                BIT_INSTRUCTIONS
                    executionLogFile << bitInstructionsTable[(opcode >> 3 & 0x7)];
                    break;
                MOV
                    executionLogFile << "MOV " << registerTable[(opcode >> 3 & 0x7)] << ", " << registerTable[opcode & 0x7];
                    break;
                HLT
                    executionLogFile << "HLT";
                    break;    
                ARITHMETIC_INSTRUCTIONS
                    executionLogFile << arithmeticInstructionsTable[(opcode >> 3) & 0x7] << " " << registerTable[opcode & 0x7];
                    break;
                RETURNS
                    executionLogFile << (opcode & 0x1 ? "RET " : conditionalReturnsTable[(opcode >> 3) & 0x7]);
                    break;
                POP
                    executionLogFile << "POP " << registerPairTable2[(opcode >> 4) & 0x3];
                    break;
                PCHL
                    executionLogFile << "PCHL";
                    break;
                SPHL
                    executionLogFile << "SPHL";
                    break;
                JUMPS
                {
                    u16 destinationAddr = (*(mem + 1) << 8) | (*(mem + 2));
                    if(opcode & 0x1)
                    {
                        executionLogFile << "JMP " << destinationAddr;
                    }
                    else
                    {
                        executionLogFile << conditionalJumpsTable[(opcode >> 3) & 0x7] << " " << destinationAddr;
                    }
                    break;
                }
                OUT
                    executionLogFile << "OUT " << (unsigned)*(mem + 1);
                    break;
                XTHL
                    executionLogFile << "XTHL";
                    break;
                DI
                    executionLogFile << "DI";
                    break;
                IN
                    executionLogFile << "IN " << (unsigned)*(mem + 1);
                    break;
                XCHG
                    executionLogFile << "XCHG";
                    break;
                EI
                    executionLogFile << "EI";
                    break;
                CALLS
                {
                    u16 destinationAddr = (*(mem + 1) << 8) | (*(mem + 2));
                    if(opcode & 0x1)
                    {
                        executionLogFile << "CALL " << destinationAddr;
                    }
                    else
                    {
                        executionLogFile << conditionalCallsTable[(opcode >> 3) & 0x7] << " " << destinationAddr;
                    }
                    break;
                }
                PUSH
                    executionLogFile << "PUSH " << registerPairTable2[(opcode >> 4) & 0x3];
                    break;
                IMMEDATE_ARITHMETIC_INSTRUCTIONS
                    executionLogFile 
                        << immedateArithmeticInstructionsTable[(opcode >> 3) & 0x7]
                        << " " << registerTable[opcode & 0x7] << ", " << (unsigned)*(mem + 1);
                    break;
                RST
                    executionLogFile << "RST " << (unsigned)((opcode >> 3) & 0x7);
                    break;
                default:
                    executionLogFile << "Unknown instruction " << (unsigned) opcode;
            }

            executionLogFile << std::endl;

            return CpuImpl::executeInstruction(opcode);
        }

    private:
        std::ofstream executionLogFile;
};

int main()
{
    auto bus = std::make_shared<DiagnosticBusImpl>();
    auto cpu = std::make_unique<DiagnosticCpuImpl>(bus);

    auto loadRom = [&bus](auto filename, auto startAddr) {
        auto file = std::ifstream(filename, std::ios::binary);
        if(!file.is_open())
        {
            std::cout << "Unable to open file " << filename << std::endl;
            file.exceptions(file.failbit);
        }

        file.seekg(0, std::ios::end);
        auto fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<u8> data(fileSize);
        file.read((char *) &data[0], fileSize);

        for(const auto& byte : data)
            bus->writeIntoMemory(startAddr++, byte);

        std::cout << std::endl << "-----------------------------------" << std::endl;
        std::cout << "File " << filename << " loaded, size " << fileSize << std::endl;
    };

    auto executeTest = [&cpu, &bus, loadRom](auto filename) {
        static const u8 RET_OPCODE = 0xC9;
        static const u16 ORIGIN = 0x0100;
        static const u16 CPM_WBOOT_SECT_ADDR = 0x0000;
        static const u16 CPM_PRINT_PROC_ADDR = 0x0005;

        bus->clearMemory();

        loadRom(filename, ORIGIN);
        bus->writeIntoMemory(CPM_PRINT_PROC_ADDR, RET_OPCODE); // Inject RET in place of CP/M printing subroutine
        auto& pc = cpu->getRegisters().getPc();
        pc = ORIGIN; // Start execution at address 0x0100

        while(true)
        {
            if(cpu->getRegisters().getPc() == CPM_PRINT_PROC_ADDR)
            {
                u8 c = cpu->getRegisters().getBc().getLow();
                u16 de = cpu->getRegisters().getDe();
                u8 e = cpu->getRegisters().getDe().getLow();

                if(c == 0x9)
                {
                    auto addr = de;
                    char * data = reinterpret_cast<char*>(&bus->getMemoryLocationRef(addr + 3));
                    while(*data != '$')
                    {
                        std::cout << *data++;
                    }
                    std::cout << std::endl;
                }
                else if(c == 0x2)
                {
                    std::cout << (char) e;
                }
            }
            cpu->step();
            if(cpu->isHalted())
            {
                std::cout << "Cpu has beed HALTED!" << std::endl;
                std::exit(1);
            }
            if(cpu->getRegisters().getPc() == CPM_WBOOT_SECT_ADDR)
            {
                std::cout << std::endl << "Jumped to 0000" << std::endl;
                return;
            }
        }
    };

    executeTest("TEST.COM");

    return 0;
}