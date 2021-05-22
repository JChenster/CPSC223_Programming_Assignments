#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <assert.h>

#include "decrypt.h"
#include "getInputs.h"

int main(int argc, char ** argv){
    uint64_t salt_hash = precomputeSaltHash(argv[1]);
    EncryptInput inputs = encryptInputCreate();
    Hashmap h = hashmapCreate();
    h = hashmapAddInputs(h, inputs, salt_hash);

    // Read in password dict
    FILE * dict = fopen(argv[2], "r");
    char * cur;
    while((cur = fileGetLine(dict))){
        crackPassword(h, cur, salt_hash);
        free(cur);
    }
    fclose(dict);

    matchCrackedPasswords(h, inputs);
    encryptInputDestroy(inputs);
    hashmapDestroy(h);
}
