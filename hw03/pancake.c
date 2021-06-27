#include <stdio.h>
#include <stdlib.h>
#include "pancake.h"

void runPancakeProgram(unsigned int *memory){

    unsigned int i = 0;
    unsigned int x, y, next; 
    while(1) {
        switch (memory[i]) {
            // flip
            case 0:
                // malloc temp space
                x = memory[++i];
                y = memory[++i];
                size_t temp_size = (y - x) * sizeof(unsigned int);
                unsigned int * temp = malloc(temp_size);

                // j represents location on memory
                for (int j = x; j < y; j++){
                    temp[j - x] = memory[j];
                }
                for (int j = x; j < y; j++){
                    memory[j] = temp[y - j - 1];
                }

                free(temp);
                i++;
                break;

            // print
            case 1:
                x = memory[++i];
                y = memory[++i];
                
                if (y - x){
                    for (int j = x; j < y; j++){
                        if (j == y - 1){
                            printf("%u\n", memory[j]);
                        }
                        else{
                            printf("%u ", memory[j]);
                        }
                    }
                }
                else{
                    printf("\n");
                }

                i++;
                break;

            // jump
            case 2:
                next = memory[++i];
                i = next;
                break;

            // skip
            case 3:
                x = memory[i + 1];
                y = memory[i + 2];
                if (memory[x] > memory[y]){
                    // need implementation for finding what the next instruction is
                    next = memory[i + 3];
                    // 2 args
                    if (next == 0 || next == 1 || next == 3){
                        i += 6;
                    }
                    // 1 arg
                    else if (next == 2){
                        i += 5;
                    }
                    // no args
                    else{
                        i += 4;
                    }
                }
                else{
                    i += 3;
                }
                break;

            // stop
            default:
                return;
        }
        
    }

}
