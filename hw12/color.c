#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "graph.h"

int main(){
    size_t vertices;
    scanf("%zu", &vertices);
    Graph g = graphCreate(vertices);
    graphPopulate(g);
    //graphPrint(g);
    graphColor(g); 
    graphColorPrint(g);
    graphDestroy(g);
}

