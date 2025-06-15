#include "defines.h"
#include "cpu.h"

int main()
{
    MEM mem = {};
    CPU cpu = {};
    
    reset_cpu(&cpu, &mem);
    
    // Start - Inline program
    cpu.X = 0xFF;
    
    mem.Data[0xFFFC] = INS_LDA_ABS_X;
    mem.Data[0xFFFD] = 0x02;
    mem.Data[0xFFFE] = 0x44; // NOTE: Should load from 0x4488 as little endian
    mem.Data[0x4501] = 0x37; // NOTE: 0x4402 + 0xFF -> 0x4501 (crossed the page boundary )
    // End - Inline program
    
    
    CPU copyCpu = cpu;
    i32 cycles = execute_instruction(&cpu, &mem, 5);
    
    return 0;
}