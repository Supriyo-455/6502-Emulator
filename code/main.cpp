#include "defines.h"
#include "cpu.h"

int main()
{
    MEM mem = {};
    CPU cpu = {};
    
    reset_cpu(&cpu, &mem);
    
    // Start - Inline program
    mem.Data[0xFFFC] = INS_LDX_IM;
    mem.Data[0xFFFD] = 0x84;
    // End - Inline program
    
    execute_instruction(&cpu, &mem, 2);
    
    return 0;
}