/* date = May 5th 2025 8:46 pm */

#ifndef DEFINES_H
#define DEFINES_H

#define MAX_MEM 1024*64

#define SET_BIT(NUM, N) (NUM |= (1 << N))
#define UNSET_BIT(NUM, N) (NUM &= (~(1 << N)))

typedef unsigned char Byte;
typedef unsigned short Word;
typedef unsigned int u32;

#endif //DEFINES_H
