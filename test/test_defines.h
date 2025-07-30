/* date = June 16th 2025 7:43 am */

#ifndef TEST_DEFINES_H
#define TEST_DEFINES_H

#include "gtest/gtest.h"
#include "cpu.h"

struct Test6502Cpu1 : public testing::Test
{
    MEM mem = {};
    CPU cpu = {};
    
    virtual void SetUp()
    {
        reset_cpu(&cpu, &mem);
    }
    
    virtual void TearDown()
    {
        
    }
};

void LOAD_REGISTER_CHECK_UNCHANGED_CPU_FLAGS(CPU cpu, CPU copyCpu) 
{
    EXPECT_EQ(cpu.PS.CarryFlag, copyCpu.PS.CarryFlag);
    EXPECT_EQ(cpu.PS.InterruptDisableFlag, copyCpu.PS.InterruptDisableFlag);
    EXPECT_EQ(cpu.PS.DecimalModeFlag, copyCpu.PS.DecimalModeFlag);
    EXPECT_EQ(cpu.PS.BreakCommandFlag, copyCpu.PS.BreakCommandFlag);
    EXPECT_EQ(cpu.PS.OverflowFlag, copyCpu.PS.OverflowFlag);
}

i32 Test_Load_Register_Immediate(CPU* cpu, MEM* mem, i32 cycles, Byte OpCode, Byte Value)
{
    // Start - Inline program
    mem->Data[0xFFFC] = OpCode;
    mem->Data[0xFFFD] = Value;
    // End - Inline program
    
    return execute_instruction(cpu, mem, cycles);
}

#endif //TEST_DEFINES_H
