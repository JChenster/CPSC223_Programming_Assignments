
// typedef a pancake stack
typedef struct panStack * PancakeStack;

PancakeStack pancakeStackCreate();

// our recursive function
// parses input and adds it to the stack if it's just a char
// if it's a nested delimiter, it will call itself and add the result to the stack
// returns the stack 
PancakeStack stackify(PancakeStack p);

// print pancake stack in correct order by popping
void pancakeStackPrint(PancakeStack p);

