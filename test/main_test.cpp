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

void LDA_CHECK_UNCHANGED_CPU_FLAGS(CPU cpu, CPU copyCpu) 
{
    EXPECT_EQ(BIT_MASK(cpu.PS, 0), BIT_MASK(copyCpu.PS, 0));
    EXPECT_EQ(BIT_MASK(cpu.PS, 2), BIT_MASK(copyCpu.PS, 2));
    EXPECT_EQ(BIT_MASK(cpu.PS, 3), BIT_MASK(copyCpu.PS, 3));
    EXPECT_EQ(BIT_MASK(cpu.PS, 4), BIT_MASK(copyCpu.PS, 4));
    EXPECT_EQ(BIT_MASK(cpu.PS, 5), BIT_MASK(copyCpu.PS, 5));
}

TEST_F(Test6502Cpu1, the_cpu_does_nothing_when_we_execute_zero_cycles)
{
    const i32 NUM_CYCLES = 0;
    
    i32 cyclesUsed = execute_instruction(&cpu, &mem, 0);
    
    EXPECT_EQ(cyclesUsed, 0);
}

TEST_F(Test6502Cpu1, the_cpu_can_execute_more_cycles_than_requested_if_required)
{
    mem.Data[0xFFFC] = INS_LDA_IM;
    mem.Data[0xFFFD] = 0x84;
    
    i32 cycles = execute_instruction(&cpu, &mem, 1);
    
    // NOTE: We have given 1 cycle instead of 2,
    // thats why we get -1 returned.
    EXPECT_EQ(cycles, -1);
}

TEST_F(Test6502Cpu1, executing_a_bad_instruction_does_not_put_the_cpu_in_an_infinite_loop)
{
    mem.Data[0xFFFC] = 0x00; // NOTE: Invalid instruction
    
    i32 cycles = execute_instruction(&cpu, &mem, 1);
    
    EXPECT_EQ(cycles, 0);
}

TEST_F(Test6502Cpu1, lda_immediate_can_load_a_value_to_the_a_register)
{
    // Start - Inline program
    mem.Data[0xFFFC] = INS_LDA_IM;
    mem.Data[0xFFFD] = 0x84;
    // End - Inline program
    
    CPU copyCpu = cpu;
    i32 cycles = execute_instruction(&cpu, &mem, 2);
    
    EXPECT_EQ(cycles, 0); // NOTE: Exact amount of cpu cycles consumed
    EXPECT_EQ(cpu.A, 0x84);
    EXPECT_FALSE(IS_SET(cpu.PS, 1));
    EXPECT_TRUE(IS_SET(cpu.PS, 6));
    
    LDA_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
}

TEST_F(Test6502Cpu1, lda_immediate_affect_the_zero_flag)
{
    cpu.A = 0x44;
    // Start - Inline program
    mem.Data[0xFFFC] = INS_LDA_IM;
    mem.Data[0xFFFD] = 0x00;
    // End - Inline program
    
    CPU copyCpu = cpu;
    i32 cycles = execute_instruction(&cpu, &mem, 2);
    
    EXPECT_EQ(cycles, 0); // NOTE: Exact amount of cpu cycles consumed
    EXPECT_EQ(cpu.A, 0x00);
    EXPECT_TRUE(IS_SET(cpu.PS, 1));
    EXPECT_FALSE(IS_SET(cpu.PS, 6));
    
    LDA_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
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
    EXPECT_FALSE(IS_SET(cpu.PS, 1));
    EXPECT_FALSE(IS_SET(cpu.PS, 6));
    
    LDA_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
}

TEST_F(Test6502Cpu1, lda_zero_page_X_can_load_a_value_to_the_a_register)
{
    cpu.X = 0x05;
    // Start - Inline program
    mem.Data[0xFFFC] = INS_LDA_ZPX;
    mem.Data[0xFFFD] = 0x42;
    mem.Data[0x0047] = 0x37;
    // End - Inline program
    
    CPU copyCpu = cpu;
    i32 cycles = execute_instruction(&cpu, &mem, 4);
    
    EXPECT_EQ(cycles, 0); // NOTE: Exact amount of cpu cycles consumed
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_FALSE(IS_SET(cpu.PS, 1));
    EXPECT_FALSE(IS_SET(cpu.PS, 6));
    
    LDA_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
}

TEST_F(Test6502Cpu1, lda_zero_page_X_can_load_a_value_to_the_a_register_when_it_wraps)
{
    cpu.X = 0xFF;
    // Start - Inline program
    mem.Data[0xFFFC] = INS_LDA_ZPX;
    mem.Data[0xFFFD] = 0x80;
    mem.Data[0x007F] = 0x37;
    // End - Inline program
    
    CPU copyCpu = cpu;
    i32 cycles = execute_instruction(&cpu, &mem, 4);
    
    EXPECT_EQ(cycles, 0); // NOTE: Exact amount of cpu cycles consumed
    EXPECT_EQ(cpu.A, 0x37);
    EXPECT_FALSE(IS_SET(cpu.PS, 1));
    EXPECT_FALSE(IS_SET(cpu.PS, 6));
    
    LDA_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
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
    EXPECT_FALSE(IS_SET(cpu.PS, 1));
    EXPECT_FALSE(IS_SET(cpu.PS, 6));
    
    LDA_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
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
    EXPECT_FALSE(IS_SET(cpu.PS, 1));
    EXPECT_FALSE(IS_SET(cpu.PS, 6));
    
    LDA_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
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
    EXPECT_FALSE(IS_SET(cpu.PS, 1));
    EXPECT_FALSE(IS_SET(cpu.PS, 6));
    
    LDA_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
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
    EXPECT_FALSE(IS_SET(cpu.PS, 1));
    EXPECT_FALSE(IS_SET(cpu.PS, 6));
    
    LDA_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
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
    EXPECT_FALSE(IS_SET(cpu.PS, 1));
    EXPECT_FALSE(IS_SET(cpu.PS, 6));
    
    LDA_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
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
    EXPECT_FALSE(IS_SET(cpu.PS, 1));
    EXPECT_FALSE(IS_SET(cpu.PS, 6));
    
    LDA_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
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
    EXPECT_FALSE(IS_SET(cpu.PS, 1));
    EXPECT_FALSE(IS_SET(cpu.PS, 6));
    
    LDA_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
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
    EXPECT_FALSE(IS_SET(cpu.PS, 1));
    EXPECT_FALSE(IS_SET(cpu.PS, 6));
    
    LDA_CHECK_UNCHANGED_CPU_FLAGS(cpu, copyCpu);
}