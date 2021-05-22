#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>

#include "compressionDict.h"
#include "helperFunctions.h"
#include "compressionFormat.h"

// take a string

// create and print out dict

// iterate through word using created hash map and convert to compression

int main(){
    CompressionDict c = compressionDictCreate();

    // get inputs and set up compression dictionaries
    int * input = getInput();
    compressionDictUpdateIter(c, input);
    FlatCompressionDict f = compressionDictAssign(c);

    // for testing purposes
    //compressionDictPrint(c);
    //printf("---------------------\n");
    //flatCompressionDictPrint(c, f);
    
    compressInput(c, input);

    free(input);
    compressionDictDestroy(c);
    free(f);
}

