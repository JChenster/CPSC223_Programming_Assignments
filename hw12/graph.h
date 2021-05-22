typedef struct graph * Graph;

// create an empty graph
Graph graphCreate();

void graphDestroy(Graph g);

// iteratively take input from stdin and populate in graph
// using graphInsert
void graphPopulate(Graph g);

void graphPrint(Graph g);

// color the graph
// go through each node index i
// create a colors array that can accomodate i colors
// go through at most d(d-1) 2-neighbors and fill in array with colors already used
// only need to check the nodes that are less than i
// iter through colors array to find the lowest color that has not been used by 
// one of its 2-neighbors
void graphColor(Graph g);

// prints the necessary output
void graphColorPrint(Graph g);

