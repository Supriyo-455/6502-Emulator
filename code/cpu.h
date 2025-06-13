/* date = May 5th 2025 8:54 pm */

#ifndef CPU_H
#define CPU_H

#include "defines.h"

// NOTE: Opcodes definition
#define INS_LDA_IM                            0xA9
#define INS_LDA_ZP                            0xA5
#define INS_LDA_ZPX                           0xB5
#define INS_LDA_ABS                           0xAD
#define INS_LDA_ABS_X                         0xBD
#define INS_LDA_ABS_Y                         0xB9
#define INS_LDA_IND_X                         0xA1
#define INS_LDA_IND_Y                         0xB1

#define INS_JSR_ABS                           0x20

struct MEM
{
    Byte Data[MAX_MEM];
};

struct CPU
{
    Word PC; // Program Counter
    Word SP; // Stack Pointer
    
    Byte A, X, Y; // Registers
    
    // NOTE: Processor status, 8 bit number, each bit represents a flag
    // Bit 0 -> Carry Flag
    // Bit 1 -> Zero Flag
    // Bit 2 -> Interrupt Disable Flag
    // Bit 3 -> Decimal Mode Flag
    // Bit 4 -> Break Command Flag
    // Bit 5 -> Overflow Flag
    // Bit 6 -> Negative Flag
    Byte PS;
};

void cpu_lda_set_status(CPU* cpu);

void initialize_memory(MEM* memory);

void reset_cpu(CPU* cpu, MEM* mem);

Byte fetch_byte(CPU* cpu, MEM* mem, i32* cycles);

Word fetch_word(CPU* cpu, MEM* mem, i32* cycles);

Byte read_byte(MEM* mem, Word address, i32* cycles);

void write_word(MEM* mem, Word value, i32 address, i32* cycles);

i32 execute_instruction(CPU* cpu, MEM* mem, i32 cycles);


#endif //CPU_H
