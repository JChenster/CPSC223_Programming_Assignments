#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "flip.h"

int main(){
    int c;
    while ((c = getchar()) != EOF){
        if (c == '{'){
            // make a new pancake stack and fill it up with the results of the delimiter and then print
            pancakeStackPrint(stackify(pancakeStackCreate()));
        }
        else{
            putchar(c);
        }
    }
}




