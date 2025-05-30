#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "cpu.h"

void cpu_lda_set_status(CPU* cpu)
{
    if(cpu->A == 0)
    {
        SET_BIT(cpu->PS, 1);
    }
    else
    {
        UNSET_BIT(cpu->PS, 0);
    }
    
    if((cpu->A & 0b10000000) > 0)
    {
        SET_BIT(cpu->PS, 6);
    }
    else
    {
        UNSET_BIT(cpu->PS, 6);
    }
}

void initialize_memory(MEM* memory)
{
    for(u32 i = 0; i < MAX_MEM; i++) 
    {
        memory->Data[i] = 0;
    }
}

void reset_cpu(CPU* cpu, MEM* mem)
{
    cpu->PC = 0xFFFC;
    cpu->SP = 0x0100;
    
    cpu->A = 0;
    cpu->X = 0;
    cpu->Y = 0;
    
    cpu->PS = 0b00000000;
    
    initialize_memory(mem);
}

Byte fetch_byte(CPU* cpu, MEM* mem, u32* cycles)
{
    Byte data = mem->Data[cpu->PC++];
    *cycles -= 1;
    return data;
}

Word fetch_word(CPU* cpu, MEM* mem, u32* cycles)
{
    // NOTE: 6502 is little endian
    Word Data = mem->Data[cpu->PC++];
    
    Data |= (mem->Data[cpu->PC++] << 8);
    *cycles -= 2;
    
    return Data;
}

Byte read_byte(MEM* mem, u32 address, u32* cycles)
{
    Byte data = mem->Data[address];
    *cycles -= 1;
    return data;
}

void write_word(MEM* mem, Word value, u32 address, u32* cycles)
{
    mem->Data[address] = value & 0xFF;
    mem->Data[address + 1] = (value >> 8);
    *cycles -= 2;
}

void execute_instruction(CPU* cpu, MEM* mem, u32 cycles)
{
    while(cycles > 0)
    {
        Byte ins = fetch_byte(cpu, mem, &cycles);
        switch(ins)
        {
            // NOTE: LDA addressing modes
            case INS_LDA_IM:
            {
                Byte value = fetch_byte(cpu, mem, &cycles);
                cpu->A = value;
                
                cpu_lda_set_status(cpu);
            }
            break;
            
            case INS_LDA_ZP:
            {
                u32 index = cpu->PC;
                Byte ZeroPageAddress = fetch_byte(cpu, mem, &cycles);
                cpu->A = read_byte(mem, ZeroPageAddress, &cycles);
                
                cpu_lda_set_status(cpu);
            }
            break;
            
            case INS_LDA_ZPX:
            {
                u32 index = cpu->PC;
                Byte ZeroPageAddress = fetch_byte(cpu, mem, &cycles);
                // TODO: Overflow address check
                ZeroPageAddress += cpu->X;
                cpu->A = read_byte(mem, ZeroPageAddress, &cycles);
                cycles--;
            }
            break;
            
            case INS_JSR_ABS:
            {
                Word jmpAddress = fetch_word(cpu, mem, &cycles);
                write_word(mem, cpu->PC - 1, cpu->SP, &cycles);
                cpu->SP++;
                cpu->PC = jmpAddress;
                cycles--;
            }
            break;
            
            default:
            {
                printf("Instruction not handled 0x%.2x\n", ins);
            }
            break;
        }
    }
}