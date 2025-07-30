/* date = May 5th 2025 8:54 pm */

#ifndef CPU_H
#define CPU_H

#include "defines.h"

// NOTE: Opcodes definition
#define INS_LDA_IM                            0xA9
#define INS_LDA_ZP                            0xA5
#define INS_LDA_ZP_X                          0xB5
#define INS_LDA_ABS                           0xAD
#define INS_LDA_ABS_X                         0xBD
#define INS_LDA_ABS_Y                         0xB9
#define INS_LDA_IND_X                         0xA1
#define INS_LDA_IND_Y                         0xB1

#define INS_LDX_IM                            0xA2
#define INS_LDX_ZP                            0xA6
#define INS_LDX_ZP_Y                          0xB6
#define INS_LDX_ABS                           0xAE
#define INS_LDX_ABS_Y                         0xBE

#define INS_JSR_ABS                           0x20

struct MEM
{
    Byte Data[MAX_MEM];
};

struct ProcessorStatus
{
    bool CarryFlag;
    bool ZeroFlag;
    bool InterruptDisableFlag;
    bool DecimalModeFlag;
    bool BreakCommandFlag;
    bool OverflowFlag;
    bool NegativeFlag;
};

struct CPU
{
    Word PC; // Program Counter
    Word SP; // Stack Pointer
    
    Byte A, X, Y; // Registers
    
    ProcessorStatus PS;
};

void cpu_lda_set_status(CPU* cpu);

void initialize_memory(MEM* memory);

void reset_cpu(CPU* cpu, MEM* mem);

Byte fetch_byte(CPU* cpu, MEM* mem, i32* cycles);

Word fetch_word(CPU* cpu, MEM* mem, i32* cycles);

Byte read_byte(MEM* mem, Word address, i32* cycles);

Word read_word(MEM* mem, Word address, i32* cycles);

void write_word(MEM* mem, Word value, Word address, i32* cycles);

i32 execute_instruction(CPU* cpu, MEM* mem, i32 cycles);


#endif //CPU_H
