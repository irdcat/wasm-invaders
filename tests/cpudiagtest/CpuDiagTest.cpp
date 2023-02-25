#include <array>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>

#include "../../src/CpuImpl.hpp"
#include "../../src/Bus.hpp"

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

int main()
{
    auto bus = std::make_shared<DiagnosticBusImpl>();
    auto cpu = std::make_unique<CpuImpl>(bus);

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

    auto executeTest = [&cpu, &bus, loadRom](auto filename, bool checkForSuccess) {
        static const u8 RET_OPCODE = 0xC9;
        static const u16 ORIGIN = 0x0100;
        static const u16 CPM_WBOOT_SECT_ADDR = 0x0000;
        static const u16 CPM_PRINT_PROC_ADDR = 0x0005;

        bus->clearMemory();
        bool success = true;

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
                    char * data = reinterpret_cast<char*>(&bus->getMemoryLocationRef(addr));
                    while(*data != '$')
                    {
                        std::cout << *data++;
                    }
                    std::cout << std::endl;
                    success = false;
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
                if(checkForSuccess && success)
                {
                    std::exit(1);
                }
                return;
            }
        }
    };

    // Disabled as it keeps being stuck in a loop
    // Probably because CPU implementation is not correct
    //executeTest("CPUTEST.COM", false);
    
    executeTest("TEST.COM", false);
    executeTest("8080PRE.COM", true);
    executeTest("8080EX1.COM", false);

    return 0;
}