/* date = May 5th 2025 8:46 pm */

#ifndef DEFINES_H
#define DEFINES_H

#define MAX_MEM 1024*64

#define SET_BIT(NUM, N) (NUM |= (1 << N))
#define UNSET_BIT(NUM, N) (NUM &= (~(1 << N)))
#define BIT_MASK(NUM, N) (NUM & (1 << N))
#define IS_SET(NUM, N) (BIT_MASK(NUM, N) == (1 << N))

typedef unsigned char Byte;
typedef unsigned short Word;
typedef unsigned int u32;
typedef int i32;

#endif //DEFINES_H
