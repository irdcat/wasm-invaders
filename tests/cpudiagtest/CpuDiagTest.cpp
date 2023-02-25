#include <array>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <memory>
#include <cstdlib>

#include "../../src/CpuImpl.hpp"
#include "../../src/Bus.hpp"

template <typename Container, typename T>
bool contains(const Container &c, T value) 
{
    return std::any_of(
        c.begin(), 
        c.end(), 
        [value](auto element){ 
            return element == value;
        }
    );
}

class DiagnosticBusImpl
    : public Bus
{
    public:
        DiagnosticBusImpl() = default;

        ~DiagnosticBusImpl() = default;

        u8 readFromMemory(u16 addr) const override
        {
            return memory[addr % 0x4000];
        }

        void writeIntoMemory(u16 addr, u8 value) override
        {
            memory[addr % 0x4000] = value;
        }

        u8 readFromInputPort(u8 port) const override
        {
            return 0;
        }

        void writeIntoOutputPort(u8 port, u8 value) override
        {
        }

        u8& getMemoryLocationRef(u16 addr) override
        {
            return memory[addr % 0x4000];
        }

    private:
        std::array<u8, 0x4000> memory;
};

class DiagnosticCpuImpl
    : public CpuImpl
{
    public:
        DiagnosticCpuImpl(const std::shared_ptr<Bus>& bus)
            : CpuImpl(bus)
        {
            auto& pc = CpuImpl::getRegisters().getPc();
            pc = 0x100; // Set origin to addr 0x100
        }

        ~DiagnosticCpuImpl() = default;

        unsigned executeInstruction(u8 opcode) override 
        {
            const std::array<u8, 4> CALL_OPCODES = { 0xCD, 0xDD, 0xED, 0xFD };
            if(contains(CALL_OPCODES, opcode))
            {
                auto addr = peekAddress();
                if(addr == 0x0005) // CP/M internal printing procedure
                {
                    auto& c = CpuImpl::getRegisters().getBc().getLow();

                    if(c == 0x9)
                    {
                        u16 offset = CpuImpl::getRegisters().getDe();
                        char* str = reinterpret_cast<char *>(&bus->getMemoryLocationRef(offset + 3));
                        while(*str != '$')
                        {
                            std::cout << *str++;
                        }
                        std::cout << std::endl;
                    }
                }
                else if(addr == 0x0000)
                {
                    std::exit(0);
                }

                return 11;
            }
            if(CpuImpl::getRegisters().getPc() == 0x1)
            {
                std::exit(0);
            }

            return CpuImpl::executeInstruction(opcode);
        }

    private:
        u16 peekAddress()
        {
            return bus->readFromMemory(CpuImpl::getRegisters().getPc())
                | (bus->readFromMemory(CpuImpl::getRegisters().getPc() + 1) << 8);
        }
};

int main()
{
    auto bus = std::make_shared<DiagnosticBusImpl>();
    auto cpu = std::make_unique<DiagnosticCpuImpl>(bus);

    auto file = std::ifstream("cpudiag.bin", std::ios::binary);

    if(!file.is_open())
    {
        file.exceptions(file.failbit);
    }

    file.seekg(0, std::ios::end);
    auto fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<u8> data(fileSize);
    file.read((char *) &data[0], fileSize);

    auto startAddr = 0x100;
    for(const auto& byte : data)
    {
        bus->writeIntoMemory(startAddr++, byte);
    }
    bus->writeIntoMemory(0x5, 0xC9);

    while(true)
    {
        cpu->step();
    }

    return 0;
}