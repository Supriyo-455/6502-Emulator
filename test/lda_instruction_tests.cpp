#include "test_defines.h"

TEST_F(Test6502Cpu1, lda_immediate_can_load_a_value_to_the_a_register)
{
    CPU copyCpu = cpu;
    
    i32 cycles = Test_Load_Register_Immediate(&cpu, &mem, 2, INS_LDA_IM, 0x84);
    
    EXPECT_EQ(cycles, 0); // NOTE: Exact amount of cpu cycles consumed
    EXPECT_EQ(cpu.A, 0x84);
    EXPECT_FALSE(cpu.PS.ZeroFlag);
    EXPECT_FALSE(cpu.PS.NegativeFlag);
    
    LOAD_REGISTER_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
}

TEST_F(Test6502Cpu1, lda_immediate_affect_the_zero_flag)
{
    cpu.A = 0x44;
    
    CPU copyCpu = cpu;
    
    i32 cycles = Test_Load_Register_Immediate(&cpu, &mem, 2, INS_LDA_IM, 0x00);
    
    EXPECT_EQ(cycles, 0); // NOTE: Exact amount of cpu cycles consumed
    EXPECT_EQ(cpu.A, 0x00);
    EXPECT_TRUE(cpu.PS.ZeroFlag);
    EXPECT_FALSE(cpu.PS.NegativeFlag);
    
    LOAD_REGISTER_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
}

TEST_F(Test6502Cpu1, lda_zero_page_can_load_a_value_to_the_a_register)
{
    // Start - Inline program
    mem.Data[0xFFFC] = INS_LDA_ZP;
    mem.Data[0xFFFD] = 0x42;
    mem.Data[0x0042] = 0x37;
    // End - Inline program
    
    CPU copyCpu = cpu;
    i32 cycles = execute_instruction(&cpu, &mem, 3);
    
    EXPECT_EQ(cycles, 0); // NOTE: Exact amount of cpu cycles consumed
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_FALSE(cpu.PS.ZeroFlag);
    EXPECT_FALSE(cpu.PS.NegativeFlag);
    
    LOAD_REGISTER_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
}

TEST_F(Test6502Cpu1, lda_zero_page_X_can_load_a_value_to_the_a_register)
{
    cpu.X = 0x05;
    // Start - Inline program
    mem.Data[0xFFFC] = INS_LDA_ZP_X;
    mem.Data[0xFFFD] = 0x42;
    mem.Data[0x0047] = 0x37;
    // End - Inline program
    
    CPU copyCpu = cpu;
    i32 cycles = execute_instruction(&cpu, &mem, 4);
    
    EXPECT_EQ(cycles, 0); // NOTE: Exact amount of cpu cycles consumed
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_FALSE(cpu.PS.ZeroFlag);
    EXPECT_FALSE(cpu.PS.NegativeFlag);
    
    LOAD_REGISTER_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
}

TEST_F(Test6502Cpu1, lda_zero_page_X_can_load_a_value_to_the_a_register_when_it_wraps)
{
    cpu.X = 0xFF;
    // Start - Inline program
    mem.Data[0xFFFC] = INS_LDA_ZP_X;
    mem.Data[0xFFFD] = 0x80;
    mem.Data[0x007F] = 0x37;
    // End - Inline program
    
    CPU copyCpu = cpu;
    i32 cycles = execute_instruction(&cpu, &mem, 4);
    
    EXPECT_EQ(cycles, 0); // NOTE: Exact amount of cpu cycles consumed
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_FALSE(cpu.PS.ZeroFlag);
    EXPECT_FALSE(cpu.PS.NegativeFlag);
    
    LOAD_REGISTER_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
}

TEST_F(Test6502Cpu1, lda_absolute_can_load_a_value_to_the_a_register)
{
    // Start - Inline program
    mem.Data[0xFFFC] = INS_LDA_ABS;
    mem.Data[0xFFFD] = 0x88;
    mem.Data[0xFFFE] = 0x44; // NOTE: Should load from 0x4488 as little endian
    mem.Data[0x4488] = 0x37;
    // End - Inline program
    
    CPU copyCpu = cpu;
    i32 cycles = execute_instruction(&cpu, &mem, 4);
    
    EXPECT_EQ(cycles, 0); // NOTE: Exact amount of cpu cycles consumed
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_FALSE(cpu.PS.ZeroFlag);
    EXPECT_FALSE(cpu.PS.NegativeFlag);
    
    LOAD_REGISTER_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
}

TEST_F(Test6502Cpu1, lda_absolute_x_can_load_a_value_to_the_a_register)
{
    // Start - Inline program
    cpu.X = 0x01;
    
    mem.Data[0xFFFC] = INS_LDA_ABS_X;
    mem.Data[0xFFFD] = 0x88;
    mem.Data[0xFFFE] = 0x44; // NOTE: Should load from 0x4488 as little endian
    mem.Data[0x4489] = 0x37; // NOTE: 0x4488 + 0x0001 -> 0x4489
    // End - Inline program
    
    
    CPU copyCpu = cpu;
    i32 cycles = execute_instruction(&cpu, &mem, 4);
    
    EXPECT_EQ(cycles, 0); // NOTE: Exact amount of cpu cycles consumed
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_FALSE(cpu.PS.ZeroFlag);
    EXPECT_FALSE(cpu.PS.NegativeFlag);
    
    LOAD_REGISTER_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
}

TEST_F(Test6502Cpu1, lda_absolute_x_can_load_a_value_to_the_a_register_when_page_boundary_crossed)
{
    // Start - Inline program
    cpu.X = 0xFF;
    
    mem.Data[0xFFFC] = INS_LDA_ABS_X;
    mem.Data[0xFFFD] = 0x02;
    mem.Data[0xFFFE] = 0x44; // NOTE: Should load from 0x4488 as little endian
    mem.Data[0x4501] = 0x37; // NOTE: 0x4402 + 0xFF -> 0x4501 (crossed the page boundary )
    // End - Inline program
    
    
    CPU copyCpu = cpu;
    i32 cycles = execute_instruction(&cpu, &mem, 5);
    
    EXPECT_EQ(cycles, 0); // NOTE: Exact amount of cpu cycles consumed
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_FALSE(cpu.PS.ZeroFlag);
    EXPECT_FALSE(cpu.PS.NegativeFlag);
    
    LOAD_REGISTER_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
}

TEST_F(Test6502Cpu1, lda_absolute_y_can_load_a_value_to_the_a_register)
{
    // Start - Inline program
    cpu.Y = 0x01;
    
    mem.Data[0xFFFC] = INS_LDA_ABS_Y;
    mem.Data[0xFFFD] = 0x88;
    mem.Data[0xFFFE] = 0x44; // NOTE: Should load from 0x4488 as little endian
    mem.Data[0x4489] = 0x37; // NOTE: 0x4488 + 0x0001 -> 0x4489
    // End - Inline program
    
    
    CPU copyCpu = cpu;
    i32 cycles = execute_instruction(&cpu, &mem, 4);
    
    EXPECT_EQ(cycles, 0); // NOTE: Exact amount of cpu cycles consumed
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_FALSE(cpu.PS.ZeroFlag);
    EXPECT_FALSE(cpu.PS.NegativeFlag);
    
    LOAD_REGISTER_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
}

TEST_F(Test6502Cpu1, lda_absolute_y_can_load_a_value_to_the_a_register_when_page_boundary_crossed)
{
    // Start - Inline program
    cpu.Y = 0xFF;
    
    mem.Data[0xFFFC] = INS_LDA_ABS_Y;
    mem.Data[0xFFFD] = 0x02;
    mem.Data[0xFFFE] = 0x44; // NOTE: Should load from 0x4488 as little endian
    mem.Data[0x4501] = 0x37; // NOTE: 0x4402 + 0xFF -> 0x4501 (crossed the page boundary )
    // End - Inline program
    
    
    CPU copyCpu = cpu;
    i32 cycles = execute_instruction(&cpu, &mem, 5);
    
    EXPECT_EQ(cycles, 0); // NOTE: Exact amount of cpu cycles consumed
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_FALSE(cpu.PS.ZeroFlag);
    EXPECT_FALSE(cpu.PS.NegativeFlag);
    
    LOAD_REGISTER_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
}

TEST_F(Test6502Cpu1, lda_indirect_x_can_load_a_value_to_the_a_register)
{
    // Start - Inline program
    cpu.X = 0x04;
    
    mem.Data[0xFFFC] = INS_LDA_IND_X;
    mem.Data[0xFFFD] = 0x02;
    mem.Data[0x0006] = 0x00; // NOTE: As 0x04 + 0x02 -> 0x06
    mem.Data[0x0007] = 0x80;
    mem.Data[0x8000] = 0x37;
    // End - Inline program
    
    CPU copyCpu = cpu;
    i32 cycles = execute_instruction(&cpu, &mem, 6);
    
    EXPECT_EQ(cycles, 0); // NOTE: Exact amount of cpu cycles consumed
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_FALSE(cpu.PS.ZeroFlag);
    EXPECT_FALSE(cpu.PS.NegativeFlag);
    
    LOAD_REGISTER_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
}

TEST_F(Test6502Cpu1, lda_indirect_y_can_load_a_value_to_the_a_register)
{
    // Start - Inline program
    cpu.Y = 0x04;
    
    mem.Data[0xFFFC] = INS_LDA_IND_Y;
    mem.Data[0xFFFD] = 0x02;
    mem.Data[0x0002] = 0x00; 
    mem.Data[0x0003] = 0x80;
    mem.Data[0x8004] = 0x37; // NOTE: As 0x8000 + 0x04 -> 0x8004
    // End - Inline program
    
    CPU copyCpu = cpu;
    i32 cycles = execute_instruction(&cpu, &mem, 5);
    
    EXPECT_EQ(cycles, 0); // NOTE: Exact amount of cpu cycles consumed
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_FALSE(cpu.PS.ZeroFlag);
    EXPECT_FALSE(cpu.PS.NegativeFlag);
    
    LOAD_REGISTER_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
}

TEST_F(Test6502Cpu1, lda_indirect_y_can_load_a_value_to_the_a_register_when_it_crosses_a_page)
{
    // Start - Inline program
    cpu.Y = 0x02;
    
    mem.Data[0xFFFC] = INS_LDA_IND_Y;
    mem.Data[0xFFFD] = 0x02;
    mem.Data[0x0002] = 0xFF; 
    mem.Data[0x0003] = 0x80;
    mem.Data[0x8101] = 0x37; // NOTE: As 0x80FF + 0x02 -> 0x8101
    // End - Inline program
    
    CPU copyCpu = cpu;
    i32 cycles = execute_instruction(&cpu, &mem, 6);
    
    EXPECT_EQ(cycles, 0); // NOTE: Exact amount of cpu cycles consumed
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_FALSE(cpu.PS.ZeroFlag);
    EXPECT_FALSE(cpu.PS.NegativeFlag);
    
    LOAD_REGISTER_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
}