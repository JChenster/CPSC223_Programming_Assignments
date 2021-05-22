#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "flip.h"

struct panStack{
    struct elt * top;
};

struct elt{
    int value;
    struct elt * next;
};

// create a panStack
PancakeStack pancakeStackCreate(){
    PancakeStack p = malloc(sizeof(struct panStack *));
    p->top = 0;
    return p;
}

static void pancakeStackPush(PancakeStack p, int c){
    struct elt * new = malloc(sizeof(struct elt));
    new->next = p->top;
    new->value = c;
    p->top = new;
}

// returns -1 if pancake stack  is empty
// otherwise popped value
static int pancakeStackPop(PancakeStack p){
    struct elt * popped = p->top;
    p->top = popped->next;
    int popped_value = popped->value;
    // free the popped element
    free(popped);
    return popped_value;
}

// we will sometimes have to add the results of a nested recursive call 
static void pancakeStackPushStack(PancakeStack p, PancakeStack nested){
    while(nested->top != 0){
        pancakeStackPush(p, nested->top->value);
        pancakeStackPop(nested);
    }
    free(nested);
}

// our recursive function
// parses input and adds it to the stack if it's just a char
// if it's a nested delimiter, it will call itself and add the result to the stack
// returns the stack 
PancakeStack stackify(PancakeStack p){
    int c;
    top:
        c = getchar();

        switch(c){
            // end of input so we are done
            case EOF:
                return p;
            // finished parsing an expression
            case '}':
                return p;
            // there is a nested expression
            case '{':
                // create a new pancake stack for nested and add the results to p
                pancakeStackPushStack(p, stackify(pancakeStackCreate()));
                goto top;
            // a regular char
            default:
                pancakeStackPush(p, c);
                goto top;
        }
}

// print pancake stack in correct order by popping
// destroy pancake stack
void pancakeStackPrint(PancakeStack p){
    while(p->top != 0){
        putchar(pancakeStackPop(p));
    }
    free(p);
}



