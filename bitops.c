#include "bitops.h"
#include <stdio.h>

unsigned int SetBit(unsigned int value, int position) {
    if (position < 0 || position >= 32) return value;
    return value | (1u << position);
}

unsigned int ClearBit(unsigned int value, int position) {
    if (position < 0 || position >= 32) return value;
    return value & ~(1u << position);
}

unsigned int ToggleBit(unsigned int value, int position) {
    if (position < 0 || position >= 32) return value;
    return value ^ (1u << position);
}

int GetBit(unsigned int value, int position) {
    if (position < 0 || position >= 32) return 0;
    return (value >> position) & 1u;
}

int CountBits(unsigned int value) {
    int count = 0;
    while (value) {
        count += value & 1u;
        value >>= 1;
    }
    return count;
}

unsigned int ShiftLeft(unsigned int value, int positions) {
    if (positions <= 0 || positions >= 32) return 0u;
    return value << positions;
}

unsigned int ShiftRight(unsigned int value, int positions) {
    if (positions <= 0 || positions >= 32) return 0u;
    return value >> positions;
}

void PrintBinary(unsigned int value) {
    for (int i = 31; i >= 0; --i) {
        putchar((value & (1u << i)) ? '1' : '0');
        if (i % 4 == 0 && i != 0) putchar(' ');
    }
    putchar('\n');
}

void PrintHex(unsigned int value) {
    printf("0x%08X\n", value);
}
