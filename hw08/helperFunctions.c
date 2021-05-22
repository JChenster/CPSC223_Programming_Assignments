#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>

#include "compressionFormat.h"

// hashing
// function is modified from 03-31-2021 lecture notes
#define FNV_PRIME_64 ((1ULL<<40)+(1<<8)+0xb3)
#define FNV_OFFSET_BASIS_64 (14695981039346656037ULL)

uint64_t FNV1a(const int *s){
    uint64_t h = FNV_OFFSET_BASIS_64;

    for(int i = 0; i < COMPRESSED_EXPANSION; i++){
        h ^= s[i];
        h *= FNV_PRIME_64;
    }

    return h;
}

// Function is adapted from encrypt.c from Assignment 7
#define GETLINE_INITIAL_SIZE (16)
#define GETLINE_MULTIPLIER (2)
int * getInput(){
    int c;
    
    // invariant:
    // size > top
    // size = size of buffer
    size_t size = GETLINE_INITIAL_SIZE;
    size_t top = 0;
    int *buffer = calloc(size, sizeof(int));

    assert(buffer);

    for(;;) {
        switch((c = getchar())) {

            case EOF:
                buffer[top] = -1;
                return buffer;

            default:
                buffer[top++] = c;
                if(top >= size) {
                    size *= GETLINE_MULTIPLIER;
                    buffer = realloc(buffer, size * sizeof(int));

                    assert(buffer);
                }
        }
    }
}
