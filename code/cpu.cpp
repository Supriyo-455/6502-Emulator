#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "cpu.h"

void cpu_load_set_status(CPU* cpu, Byte Register)
{
    if(Register == 0)
    {
        cpu->PS.ZeroFlag = true;
    }
    else
    {
        cpu->PS.ZeroFlag = false;
    }
    
    // TODO:How to detect negative numbers??
    if(Register < 0)
    {
        cpu->PS.OverflowFlag = true;
    }
    else
    {
        cpu->PS.OverflowFlag = false;
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
    
    cpu->PS = {};
    
    initialize_memory(mem);
}

Byte fetch_byte(CPU* cpu, MEM* mem, i32* cycles)
{
    Byte data = mem->Data[cpu->PC++];
    *cycles -= 1;
    return data;
}

Word fetch_word(CPU* cpu, MEM* mem, i32* cycles)
{
    // NOTE: 6502 is little endian
    Word Data = mem->Data[cpu->PC++];
    
    Data |= (mem->Data[cpu->PC++] << 8);
    *cycles -= 2;
    
    return Data;
}

Byte read_byte(MEM* mem, Word address, i32* cycles)
{
    Byte data = mem->Data[address];
    *cycles -= 1;
    return data;
}

Word read_word(MEM* mem, Word address, i32* cycles)
{
    // NOTE: 6502 is little endian
    Byte LowByte = read_byte(mem, address, cycles);
    Byte HighByte = read_byte(mem, address + 1, cycles);
    
    return (HighByte << 8) | LowByte;
}

void write_word(MEM* mem, Word value, Word address, i32* cycles)
{
    mem->Data[address] = value & 0xFF;
    mem->Data[address + 1] = (value >> 8);
    *cycles -= 2;
}

// TODO: Compress execute function
i32 execute_instruction(CPU* cpu, MEM* mem, i32 cycles)
{
    while(cycles > 0)
    {
        Byte ins = fetch_byte(cpu, mem, &cycles);
        switch(ins)
        {
            // NOTE: LDA Addressing modes
            case INS_LDA_IM:
            {
                Byte value = fetch_byte(cpu, mem, &cycles);
                
                cpu->A = value;
                
                cpu_load_set_status(cpu, cpu->A);
            }
            break;
            
            case INS_LDA_ZP:
            {
                Byte ZeroPageAddress = fetch_byte(cpu, mem, &cycles);
                
                cpu->A = read_byte(mem, ZeroPageAddress, &cycles);
                
                cpu_load_set_status(cpu, cpu->A);
            }
            break;
            
            case INS_LDA_ZP_X:
            {
                Byte ZeroPageAddress = fetch_byte(cpu, mem, &cycles);
                
                // TODO: Overflow assertions
                ZeroPageAddress += cpu->X;
                cycles--;
                
                cpu->A = read_byte(mem, ZeroPageAddress, &cycles);
                
                cpu_load_set_status(cpu, cpu->A);
            }
            break;
            
            case INS_LDA_ABS:
            {
                Word AbsoluteAddress = fetch_word(cpu, mem, &cycles);
                
                cpu->A = read_byte(mem, AbsoluteAddress, &cycles);
                
                cpu_load_set_status(cpu, cpu->A);
            }
            break;
            
            case INS_LDA_ABS_X:
            {
                Word AbsoluteAddress = fetch_word(cpu, mem, &cycles);
                
                Byte AbsoluteAddressHighBitsPreviously = (AbsoluteAddress >> 8);
                
                // TODO: Overflow assertions
                AbsoluteAddress += cpu->X;
                
                Byte AbsoluteAddressHighBitsNow = (AbsoluteAddress >> 8);
                
                if(AbsoluteAddressHighBitsNow - AbsoluteAddressHighBitsPreviously != 0)
                    cycles--;
                
                cpu->A = read_byte(mem, AbsoluteAddress, &cycles);
                
                cpu_load_set_status(cpu, cpu->A);
            }
            break;
            
            case INS_LDA_ABS_Y:
            {
                Word AbsoluteAddress = fetch_word(cpu, mem, &cycles);
                
                Byte AbsoluteAddressHighBitsPreviously = (AbsoluteAddress >> 8);
                
                // TODO: Overflow assertions
                AbsoluteAddress += cpu->Y;
                
                Byte AbsoluteAddressHighBitsNow = (AbsoluteAddress >> 8);
                
                if(AbsoluteAddressHighBitsNow - AbsoluteAddressHighBitsPreviously != 0)
                    cycles--;
                
                cpu->A = read_byte(mem, AbsoluteAddress, &cycles);
                
                cpu_load_set_status(cpu, cpu->A);
                
            }
            break;
            
            case INS_LDA_IND_X:
            {
                Byte ZeroPageAddress = fetch_byte(cpu, mem, &cycles);
                
                // TODO: Overflow assertions
                ZeroPageAddress += cpu->X;
                cycles--;
                
                Word EffectiveAddress = read_word(mem, ZeroPageAddress, &cycles);
                
                cpu->A = read_byte(mem, EffectiveAddress, &cycles);
                
                cpu_load_set_status(cpu, cpu->A);
            }
            break;
            
            case INS_LDA_IND_Y:
            {
                Byte ZeroPageAddress = fetch_byte(cpu, mem, &cycles);
                
                Word EffectiveAddress  = read_word(mem, ZeroPageAddress, &cycles);
                
                Byte EffectiveAddressHighPreviously = (EffectiveAddress >> 8);
                
                // TODO: Overflow assertions
                EffectiveAddress += cpu->Y;
                
                Byte EffectiveAddressHighNow = (EffectiveAddress >> 8);
                
                if(EffectiveAddressHighNow - EffectiveAddressHighPreviously != 0)
                    cycles--;
                
                cpu->A = read_byte(mem, EffectiveAddress, &cycles);
                
                cpu_load_set_status(cpu, cpu->A);
            }
            break;
            
            //NOTE: LDX Addressing modes
            case INS_LDX_IM:
            {
                Byte value = fetch_byte(cpu, mem, &cycles);
                
                cpu->X = value;
                
                cpu_load_set_status(cpu, cpu->X);
            }
            break;
            
            case INS_LDX_ZP:
            {
                Byte ZeroPageAddress = fetch_byte(cpu, mem, &cycles);
                
                cpu->X = read_byte(mem, ZeroPageAddress, &cycles);
                
                cpu_load_set_status(cpu, cpu->X);
            }
            break;
            
            case INS_LDX_ZP_Y:
            {
                Byte ZeroPageAddress = fetch_byte(cpu, mem, &cycles);
                
                // TODO: Overflow assertions
                ZeroPageAddress += cpu->X;
                cycles--;
                
                cpu->X = read_byte(mem, ZeroPageAddress, &cycles);
                
                cpu_load_set_status(cpu, cpu->X);
            }
            break;
            
            case INS_LDX_ABS:
            {
                Word AbsoluteAddress = fetch_word(cpu, mem, &cycles);
                
                cpu->X = read_byte(mem, AbsoluteAddress, &cycles);
                
                cpu_load_set_status(cpu, cpu->A);
            }
            break;
            
            case INS_LDX_ABS_Y:
            {
                Word AbsoluteAddress = fetch_word(cpu, mem, &cycles);
                
                Byte AbsoluteAddressHighBitsPreviously = (AbsoluteAddress >> 8);
                
                // TODO: Overflow assertions
                AbsoluteAddress += cpu->Y;
                
                Byte AbsoluteAddressHighBitsNow = (AbsoluteAddress >> 8);
                
                if(AbsoluteAddressHighBitsNow - AbsoluteAddressHighBitsPreviously != 0)
                    cycles--;
                
                cpu->X = read_byte(mem, AbsoluteAddress, &cycles);
                
                cpu_load_set_status(cpu, cpu->X);
                
            }
            break;
            
            // NOTE: JSR Addressing modes
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
    return cycles;
}