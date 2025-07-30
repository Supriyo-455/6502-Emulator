#include "test_defines.h"

// NOTE: CPU bugs test
TEST_F(Test6502Cpu1, the_cpu_does_nothing_when_we_execute_zero_cycles)
{
    const i32 NUM_CYCLES = 0;
    
    i32 cyclesUsed = execute_instruction(&cpu, &mem, 0);
    
    EXPECT_EQ(cyclesUsed, 0);
}

TEST_F(Test6502Cpu1, the_cpu_can_execute_more_cycles_than_requested_if_required)
{
    i32 cycles = Test_Load_Register_Immediate(&cpu, &mem, 1, INS_LDA_IM, 0x84);
    
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


#include "lda_instruction_tests.cpp"
#include "ldx_instruction_tests.cpp"