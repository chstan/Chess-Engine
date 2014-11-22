#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "bit.h"
#include "defines.h"
#include "extglobals.h"

unsigned int count_bits(U64 bits) {
    unsigned int bitCount;
    // each loop reduces the number of set bits by one
    for(bitCount = 0; bits; bitCount++) {
        bits &= (bits - 1);
    }
    return bitCount;
}

unsigned int LSB(U64 bits) {
    //if(!bits) return 0;
    unsigned long long lsb;
    #ifdef __clang__
    #if __SIZEOF_POINTER__ == 8
    // we're in a 64 bit world and can just use the 64 bit instruction
    __asm__("bsfq %1, %0" : "=r" (lsb) : "r" (bits));
    #else
    // we have to be careful and use the 32 bit version.
    unsigned int bits32, lsb32;
    if(!(bits >> 32)) {
        bits32 = (unsigned int) (bits >> 32);
        asm("bsfl %1, %0" : "=r" (lsb32) : "r" (bits32));
        return lsb32 + 32;
    } else {
        bits32 = (unsigned int) bits;
        asm("bsfl %1, %0" : "=r" (lsb32) : "r" (bits32));
        return lsb32;
    }
    #endif
    #elif (__GNUC__ || __cplusplus)
    __asm__("bsfq %1, %0" : "=r" (lsb) : "r" (bits));
    #endif
    return lsb;
}

unsigned int GSB(U64 bits) {
    //if(!bits) return 0;
    unsigned long long gsb;
    #ifdef __clang__
    #if __SIZEOF_POINTER__ == 8
    __asm__("bsrq %1, %0" : "=r" (gsb) : "r" (bits));
    #else
    unsigned int bits32, gsb32;
    if(bits >= BITSET(32)) {
        bits32 = (unsigned int) (bits >> 32);
        asm("bsrl %1, %0" : "=r" (gsb32) : "r" (bits32));
        return gsb32 + 32;
    } else {
        bits32 = (unsigned int) bits;
        asm("bsrl %1, %0" : "=r" (gsb32) : "r" (bits32));
        return gsb32;
    }
    #endif
    #elif (__GNUC__ || __cplusplus)
    __asm__("bsrq %0, %1" : "=r" (gsb) : "r" (bits));
    #endif
    return gsb;
}

void print_U64(U64 bits) {
    int rank, file;
    for(rank = 8; rank >= 1; rank--) {
        for(file = 1; file <= 8; file++) {
            printf(" %s ", (bits & BITSET(SQ(rank, file))) ? "#" : ".");
        }
        printf("\n");
    }
    printf("\n");
    return;
}

bool shared_file(U64 bits) {
    bits >>= LSB(bits);
    while(bits && bits != 1) {
        if((LSB(bits) % 8) == 1)
            bits >>= LSB(bits);
        else
            return false;
    }
    return true;
}

bool shared_rank(U64 bits) {
    int shift = LSB(bits) - (LSB(bits) % 8);
    return (bits >> shift) < (1 << 8);
}
