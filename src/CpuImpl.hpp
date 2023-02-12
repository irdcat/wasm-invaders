#pragma once

#include <memory>

#include "Cpu.hpp"
#include "Bus.hpp"
#include "Condition.hpp"

class CpuImpl : public Cpu
{
    public:
        CpuImpl(const std::shared_ptr<Bus>& busPtr);

        ~CpuImpl() = default;

        u8 fetchOpcode() override;

        Registers& getRegisters() override;

        bool isHalted() override;

        bool interruptsEnabled() override;

        void executeInstruction(u8 opcode) override;

    private:
        bool interrupt_enable;
        bool halted;
        Registers registers;
        std::shared_ptr<Bus> bus;

        void executeFirstGroupInstruction(u8 y, u8 z, u8 p, u8 q);
        void executeSecondGroupInstruction(u8 y, u8 z);
        void executeThirdGroupInstruction(u8 y, u8 z);
        void executeFourthGroupInstruction(u8 y, u8 z, u8 p, u8 q);

        u8& resolveRegister(unsigned index);
        u16& resolveRegisterPair1(unsigned index);
        u16& resolveRegisterPair2(unsigned index);
        u16& resolveRegisterPair(unsigned index, unsigned table);
        Condition resolveCondition(unsigned index);
        bool evaluateCondition(Condition c);

        u16 fetchImmedate16();
        u8 fetchImmedate8();
        u8 popFromStack();
        void pushIntoStack(u8 value);
        u16 popFromStack16();
        void pushIntoStack16(u16 value);

        bool checkParity(u8 value);

        // Instructions
        void nop();                         // NOP  - No operation
        void lxi(u16& reg16, u16 immedate); // LXI  - Load extended register with immedate
        void dad(u16& reg16);               // DAD  - Double Add
        void stax(u16& reg16);              // STAX - Store Accumulator at address stored in extended register
        void shld(u16 addr);                // SHLD - Store HL at immedate address
        void sta(u16 addr);                 // STA  - Store Accumulator at immedate address
        void ldax(u16& reg16);              // LDAX - Load Accumulator from address stored in extended register
        void lhld(u16 addr);                // LHLD - Load HL from immedate address
        void lda(u16 addr);                 // LDA  - Load Accumulator from immedate address
        void inx(u16& reg16);               // INX  - Increment extended register
        void dcx(u16& reg16);               // DCX  - Decrement extended register
        void inr(u8& reg8);                 // INR  - Increment register
        void dcr(u8& reg8);                 // DCR  - Decrement register
        void mvi(u8& reg8, u8 immedate);    // MVI  - Move immedate value to register
        void rlc();                         // RLC  - Rotate Accumulator Left
        void rrc();                         // RRC  - Rotate Accumulator Right
        void ral();                         // RAL  - Rotate Accumulator Left trough Carry
        void rar();                         // RAR  - Rotate Accumulator Right through Carry
        void daa();                         // DAA  - Decimal adjust accumulator
        void cma();                         // CMA  - Complement accumulator
        void stc();                         // STC  - Set carry
        void cmc();                         // CMC  - Complement carry
        void mov(u8& dest, u8& src);        // MOV  - Move one register value to another    
        void halt();                        // HALT - Halt cpu
        void add(u8& src);                  // ADD  - Add register value to accumulator
        void adc(u8& src);                  // ADC  - Add register value to accumulator with carry
        void sub(u8& src);                  // SUB  - Subtract register value from accumulator
        void sbb(u8& src);                  // SBB  - Subtract register value from accumulator with borrow
        void ana(u8& src);                  // ANA  - Logical AND register value with accumulator
        void xra(u8& src);                  // XRA  - Logical XOR register value with accumulator
        void ora(u8& src);                  // ORA  - Logical OR register value with accumulator
        void cmp(u8& src);                  // CMP  - Compare register value with accumulator (subtract without storing result)
        void ret(Condition c);              // Rcc  - Conditional return
        void pop(u16& reg16);               // POP  - Pop 16 bit value from stack and store in extended register
        void ret();                         // RET  - Unconditional return
        void pchl();                        // PCHL - Load PC from HL
        void sphl();                        // SPHL - Load SP from HL
        void jmp(Condition c, u16 addr);    // Jcc  - Conditional Jump
        void jmp(u16 addr);                 // JMP  - Unconditional Jump
        void out(u8 port);                  // OUT  - Write accumulator value to output port
        void xthl();                        // XTHL - Exchange HL with memory contents pointed by SP
        void di();                          // DI   - Disable interrupts
        void in(u8 port);                   // IN   - Read value from input port and store in accumulator
        void xchg();                        // XCHG - Exchange HL with DE
        void ei();                          // EI   - Enable interrupts
        void call(Condition c, u16 addr);   // Ccc  - Conditional call
        void push(u16& reg);                // PUSH - Push extended register value into stack
        void call(u16 addr);                // CALL - Unconditional call
        void adi(u8 immedate);              // ADI  - Add immedate to accumulator
        void aci(u8 immedate);              // ACI  - Add immedate to accumulator with carry
        void sui(u8 immedate);              // SUI  - Subtract immedate from accumulator
        void sbi(u8 immedate);              // SBI  - Subtract immedate from accumulator with borrow
        void ani(u8 immedate);              // ANI  - Logical AND accumulator with immedate
        void xri(u8 immedate);              // XRI  - Logical XOR accumulator with immedate
        void ori(u8 immedate);              // ORI  - Logical OR accumulator with immedate
        void cpi(u8 immedate);              // CPI  - Compare accumulator with immedate
        void rst(u8 vector);                // RST  - Jump to reset vector
};