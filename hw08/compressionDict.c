#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>

#include "compressionDict.h"
#include "helperFunctions.h"
#include "compressionFormat.h"

// make hash map
struct hashmap{
    size_t locations;
    size_t stored;
    struct elt ** table;
};

struct elt{
    struct elt * next;
    int bigram[COMPRESSED_EXPANSION];
    int occurences;
    int compressed;
};

static CompressionDict compressionDictCreateInternal(size_t initial_size){
    CompressionDict c = malloc(sizeof(struct hashmap));
    c->locations = initial_size;
    c->stored = 0;
    c->table = calloc(initial_size, sizeof(struct elt *));
    return c;
}

#define INITIAL_SIZE (32)
CompressionDict compressionDictCreate(){
    return compressionDictCreateInternal(INITIAL_SIZE);
}

// destroy hash map
void compressionDictDestroy(CompressionDict c){
    struct elt * next;
    for(size_t i = 0; i < c->locations; i++){
       for(struct elt * e = c->table[i]; e != 0; e = next){
           next = e->next;
           free(e);
       }
    }
    free(c->table);
    free(c);
}

static int bigramIsEqual(const int * a, const int * b){
    for (int i = 0; i < COMPRESSED_EXPANSION; i++){
        if (a[i] != b[i]){
            return 0;
        }
    }
    return 1;
}

#define SIZE_MULTIPLIER (2)
// update frequency of a bigram in hash map
void compressionDictUpdate(CompressionDict c, int * bigram, int old_occurences){
    size_t i = FNV1a(bigram) % c->locations;
    for(struct elt * e = c->table[i]; e != 0; e = e->next){
        // found bigram
        if(bigramIsEqual(e->bigram, bigram)){
            e->occurences++;
            return;
        }
    }

    // insert new
    struct elt * new = malloc(sizeof(struct elt));
    //strncpy(new->bigram, bigram, COMPRESSED_EXPANSION);
    for (size_t i = 0; i < COMPRESSED_EXPANSION; i++){
        new->bigram[i] = bigram[i];
    }
    new->occurences = old_occurences;
    new->next = c->table[i];
    new->compressed = -1;
    
    c->table[i] = new;
    c->stored++;

    // resize hasmap
    if(c->stored >= c->locations){
        CompressionDict c2 = compressionDictCreateInternal(c->locations * SIZE_MULTIPLIER);
        // repopulate
        for(size_t j = 0; j < c->locations; j++){
            for(struct elt * e = c->table[j]; e != 0; e = e->next){
                compressionDictUpdate(c2, e->bigram, e->occurences);
            }
        }

        struct hashmap temp;
        temp = * c;
        * c = * c2;
        * c2 = temp;
        compressionDictDestroy(c2);
    }
}


// return compression of bigram, 0 if none
static int findCompressed(CompressionDict c, int * bigram){
    size_t i = FNV1a(bigram) % c->locations;
    for(struct elt * e = c->table[i]; e != 0; e = e->next){
        // found bigram
        if(bigramIsEqual(e->bigram, bigram)){
            return e->compressed;
        }
    }
    return 0;
}


// print hash map
void compressionDictPrint(CompressionDict c){
    printf("num loc: %zu, num stored: %zu\n", c->locations, c->stored);
    for(size_t i = 0; i < c->locations; i++){
        for(struct elt * e = c->table[i]; e != 0; e = e->next){
            //if (e->compressed != -1){
                printf("%c%c : %d,%d : %d : %x\n", e->bigram[0], e->bigram[1], e->bigram[0], e->bigram[1], e->occurences, e->compressed);
            //}
        }
    }
}

void flatCompressionDictPrint(CompressionDict c, FlatCompressionDict f){
    for(size_t i = 0; i < c->stored && i < COMPRESSED_CHARS; i++){
        struct elt * e = f[i];
        printf("%c%c : %d,%d : %d : %x\n", e->bigram[0], e->bigram[1], e->bigram[0], e->bigram[1], e->occurences, e->compressed);
    }
}

// iterate through a string and update hash map accordingly
void compressionDictUpdateIter(CompressionDict c, const int * input){
    if (input[0] == -1){
        return;
    }
    int cur[COMPRESSED_EXPANSION];
    for(size_t i = 0; input[i] != -1 && input[i + COMPRESSED_EXPANSION - 1] != -1; i+=2){
        for (size_t j = 0; j < COMPRESSED_EXPANSION; j++){
            cur[j] = input[i + j];
        }
        compressionDictUpdate(c, cur, 1);
    }
}

static int eltCompare(const void * a, const void * b){
    int boc = (*((const struct elt **) b))->occurences;
    int aoc = (*((const struct elt **) a))->occurences;
    return boc - aoc;
}

// user must free this at some point
FlatCompressionDict compressionDictAssign(CompressionDict c){
    // flatten dictionary to a singular array
    FlatCompressionDict f = calloc(c->stored, sizeof(struct elt *));
    size_t f_place = 0;
    for(size_t i = 0; i < c->locations; i++){
        for (struct elt * e = c->table[i]; e != 0; e = e->next){
            f[f_place++] = e;
        }
    }

    // flatCompressionDictPrint(c, f);
    // use qsort to sort the array by occurences
    qsort(f, c->stored, sizeof(struct elt *), eltCompare);

    int cur = COMPRESSED_CHAR_MIN;
    // assign dictionary compressed values and print out dictionary
    for(size_t i = 0; i < c->stored && i < COMPRESSED_CHARS; i++){
        f[i]->compressed = cur++;
        for (size_t j = 0; j < COMPRESSED_EXPANSION; j++){
            putchar(f[i]->bigram[j]);
        }
    }
    // print out filler if COMPRESSED_CHARS > c->stored
    for (size_t i = c->stored; i < COMPRESSED_CHARS; i++){
        for (size_t j = 0; j < COMPRESSED_EXPANSION; j++){
            putchar('x' + j);
        }
    }

    // return new data value
    return f;
}


static void outputNoncompressedChar(int nc){
    if (nc < 0 || nc >= COMPRESSED_CHAR_MIN){
        putchar(COMPRESSED_CHAR_ESCAPE);
    }
    putchar(nc);
}

// go 2 by 2
// iterate through the input and output compression
void compressInput(CompressionDict c, const int * input){
    // input of length 1
    if(input[1] == -1){
        outputNoncompressedChar(input[0]);
        return;
    }
    int cur[COMPRESSED_EXPANSION];
    int compressed;
    for (size_t i = 0; input[i] != -1; i+=2){
        // next char is end
        if (input[i + COMPRESSED_EXPANSION - 1] == -1){
            outputNoncompressedChar(input[i]);
            return;
        }
        // there is room for bigrams
        else{
            // fill up cur with current bigram
            for (size_t j = 0; j < COMPRESSED_EXPANSION; j++){
                cur[j] = input[i + j];
            }
            // compress if there is match in dict
            if ((compressed = findCompressed(c, cur)) != -1){
                putchar(compressed);
            }
            // else just output current char
            else{ 
                for (size_t j = 0; j < COMPRESSED_EXPANSION; j++){
                    outputNoncompressedChar(input[i + j]);
                }
            }
        }
    }
}

