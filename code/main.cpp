#include "defines.h"
#include "cpu.h"

int main()
{
    MEM mem = {};
    CPU cpu = {};
    
    reset_cpu(&cpu, &mem);
    
    // Hack: Inline a little program
    mem.Data[0xFFFC] = INS_JSR_ABS;
    mem.Data[0xFFFD] = 0x42;
    mem.Data[0xFFFE] = 0x42;
    mem.Data[0x4242] = INS_LDA_IM;
    mem.Data[0x4243] = 0x82;
    
    execute_instruction(&cpu, &mem, 9);
    
    return 0;
}