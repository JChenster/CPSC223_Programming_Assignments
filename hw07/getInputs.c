#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <assert.h>

#include "getInputs.h"

/*
The following 6 functions are all taken/modified from encrypt.c
*/
#define FNV_PRIME_64 ((1ULL<<40)+(1<<8)+0xb3)
#define FNV_OFFSET_BASIS_64 (14695981039346656037ULL)

void FNV1a(uint64_t *hash, const char *s)
{
    while(*s) {
        *hash ^= *s++;
        *hash *= FNV_PRIME_64;
    }
}

uint64_t precomputeSaltHash(const char *salt)
{
    uint64_t hash = FNV_OFFSET_BASIS_64;
    FNV1a(&hash, salt);
    return hash;
}

uint64_t passwordHash(uint64_t saltHash, const char *password)
{
    FNV1a(&saltHash, password);
    return saltHash;
}

#define GETLINE_INITIAL_SIZE (16)
#define GETLINE_MULTIPLIER (2)
char * getLine()
{
    int c;
    
    // invariant:
    // size > top
    // size = size of buffer
    size_t size = GETLINE_INITIAL_SIZE;
    size_t top = 0;
    char *buffer = calloc(size, sizeof(char));

    assert(buffer);

    for(;;) {
        switch((c = getchar())) {

            case EOF:
                if(top == 0) {
                    // got nothing
                    free(buffer);
                    return 0;
                }
                // else fall through

            case '\n':
                buffer[top] = '\0';
                return buffer;

            default:
                buffer[top++] = c;
                if(top >= size) {
                    size *= GETLINE_MULTIPLIER;
                    buffer = realloc(buffer, size * sizeof(char));

                    assert(buffer);
                }
        }
    }
}

char * getUsername()
{
    int c;
    
    // invariant:
    // size > top
    // size = size of buffer
    size_t size = GETLINE_INITIAL_SIZE;
    size_t top = 0;
    char *buffer = calloc(size, sizeof(char));

    assert(buffer);

    for(;;) {
        switch((c = getchar())) {

            case EOF:
                if(top == 0) {
                    // got nothing
                    free(buffer);
                    return 0;
                }
                // else fall through

            case ':':
                buffer[top] = '\0';
                return buffer;

            default:
                buffer[top++] = c;
                if(top >= size) {
                    size *= GETLINE_MULTIPLIER;
                    buffer = realloc(buffer, size * sizeof(char));

                    assert(buffer);
                }
        }
    }
}

char * fileGetLine(FILE * f)
{
    int c;
    
    // invariant:
    // size > top
    // size = size of buffer
    size_t size = GETLINE_INITIAL_SIZE;
    size_t top = 0;
    char *buffer = calloc(size, sizeof(char));

    assert(buffer);

    for(;;) {
        switch((c = getc(f))) {

            case EOF:
                if(top == 0) {
                    // got nothing
                    free(buffer);
                    return 0;
                }
                // else fall through

            case '\n':
                buffer[top] = '\0';
                return buffer;

            default:
                buffer[top++] = c;
                if(top >= size) {
                    size *= GETLINE_MULTIPLIER;
                    buffer = realloc(buffer, size * sizeof(char));

                    assert(buffer);
                }
        }
    }
}

