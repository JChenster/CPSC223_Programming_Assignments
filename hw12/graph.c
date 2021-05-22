#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "graph.h"

struct graph{
    size_t n; // number of nodes
    struct node ** nodes;
};

struct node{
    size_t c; // color of node, initialized to -1
    size_t d;
    size_t capacity;
    size_t * neighbors;
};

#define INTIAL_SIZE (8)
#define SIZE_MULTIPLIER (2)

static void nodeInitalize(struct node * n, size_t impossible_color){
    n->c = impossible_color;
    n->d = 0;
    n->capacity = INTIAL_SIZE;
    n->neighbors = malloc(sizeof(size_t) * n->capacity);
}

// create an empty graph
Graph graphCreate(size_t vertices){
    Graph g = malloc(sizeof(struct graph));
    g->n = vertices;
    g->nodes = malloc(g->n * sizeof(struct node *));
    for (int i = 0; i < g->n; i++){
        g->nodes[i] = malloc(sizeof(struct node));
        nodeInitalize(g->nodes[i], g->n);
    }
    return g;
}

void graphDestroy(Graph g){
    for(size_t i = 0; i < g->n; i++){
        free(g->nodes[i]->neighbors);
        free(g->nodes[i]);
    }
    free(g->nodes);
    free(g);
}

// inserts neighbor v into node n
static void nodeInsertNeighbor(struct node * n, size_t v){
    n->neighbors[n->d++] = v;
    if (n->d >= n->capacity){
        n->capacity *= 2;
        n->neighbors = realloc(n->neighbors, sizeof(size_t) * n->capacity);
    }
}

// insert edge into graph
// realloc graph->nodes to enable space for u if necessary
// insert v as one of its neighbors
static void graphInsert(Graph g, size_t u, size_t v){
    // insert u-v
    nodeInsertNeighbor(g->nodes[u], v);
    // insert v-u
    nodeInsertNeighbor(g->nodes[v], u);
}

// iteratively take input from stdin and populate in graph
// using graphInsert
void graphPopulate(Graph g){
    size_t u;
    size_t v;
    while(scanf("%zu%zu", &u, &v) == 2){
        graphInsert(g, u, v);
    }
}

void graphPrint(Graph g){
    printf("digraph{\n");
    for (size_t i = 0; i < g->n; i++){
        size_t d = g->nodes[i]->d;
        if(d){
           printf("%zu->{", i);
           for (size_t j = 0; j < d - 1; j++){
               printf("%zu,", g->nodes[i]->neighbors[j]);
           }
           printf("%zu}\n", g->nodes[i]->neighbors[d - 1]);
        }
    }
    printf("}\n");
}

// color the graph
// go through each node index i
// create a colors array that can accomodate i colors
// go through at most d(d-1) 2-neighbors and fill in array with colors already used
// only need to check the nodes that are less than i
// iter through colors array to find the lowest color that has not been used by 
// one of its 2-neighbors
void graphColor(Graph g){
    for (size_t i = 0; i < g->n; i++){
        if (i){
            size_t * colors = calloc(i + 1, sizeof(size_t));
            // j represents all the first degree neighbors
            for (size_t j = 0; j < g->nodes[i]->d; j++){
                // k represents all the second degree neighbors
                for(size_t k = 0; k < g->nodes[g->nodes[i]->neighbors[j]]->d; k++){
                    size_t d2neighbor = g->nodes[g->nodes[i]->neighbors[j]]->neighbors[k];
                    if (d2neighbor < i){
                        colors[g->nodes[d2neighbor]->c] = 1;
                    }
                }
            }
            for (size_t col_index = 0; col_index < i + 1; col_index++){
                if(!colors[col_index]){
                    g->nodes[i]->c = col_index;
                    break;
                }
            }
            free(colors);
        }
        // first node
        else{
            g->nodes[i]->c = 0;
        }
    }
}

// prints the necessary output
void graphColorPrint(Graph g){
    for(size_t i = 0; i < g->n; i++){
        printf("%zu %zu\n", i, g->nodes[i]->c);
    }
}


