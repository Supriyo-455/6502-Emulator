#include "test_defines.h"

TEST_F(Test6502Cpu1, ldx_immediate_can_load_a_value_to_the_x_register)
{
    CPU copyCpu = cpu;
    
    i32 cycles = Test_Load_Register_Immediate(&cpu, &mem, 2, INS_LDX_IM, 0x84);
    
    EXPECT_EQ(cycles, 0); // NOTE: Exact amount of cpu cycles consumed
    EXPECT_EQ(cpu.X, 0x84);
    EXPECT_FALSE(cpu.PS.ZeroFlag);
    EXPECT_FALSE(cpu.PS.NegativeFlag);
    
    LOAD_REGISTER_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
}

TEST_F(Test6502Cpu1, ldx_immediate_can_load_zero_value_to_the_x_register)
{
    CPU copyCpu = cpu;
    
    i32 cycles = Test_Load_Register_Immediate(&cpu, &mem, 2, INS_LDX_IM, 0x0);
    
    EXPECT_EQ(cycles, 0); // NOTE: Exact amount of cpu cycles consumed
    EXPECT_EQ(cpu.X, 0x0);
    EXPECT_TRUE(cpu.PS.ZeroFlag);
    EXPECT_FALSE(cpu.PS.NegativeFlag);
    
    LOAD_REGISTER_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
}

TEST_F(Test6502Cpu1, ldx_immediate_can_load_negative_value_to_the_x_register)
{
    CPU copyCpu = cpu;
    
    i32 cycles = Test_Load_Register_Immediate(&cpu, &mem, 2, INS_LDX_IM, -0x12);
    
    EXPECT_EQ(cycles, 0); // NOTE: Exact amount of cpu cycles consumed
    EXPECT_EQ(cpu.X, -0x12);
    EXPECT_FALSE(cpu.PS.ZeroFlag);
    EXPECT_TRUE(cpu.PS.NegativeFlag);
    
    LOAD_REGISTER_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
}