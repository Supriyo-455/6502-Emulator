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


TEST_F(Test6502Cpu1, RunALittleInLineProgram)
{
    mem.Data[0xFFFC] = INS_JSR_ABS;
    mem.Data[0xFFFD] = 0x42;
    mem.Data[0xFFFE] = 0x42;
    mem.Data[0x4242] = INS_LDA_IM;
    mem.Data[0x4243] = 0x82;
    
    execute_instruction(&cpu, &mem, 9);
    
    EXPECT_EQ(cpu.A, 0x82);
}