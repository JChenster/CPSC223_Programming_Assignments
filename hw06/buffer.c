#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "buffer.h"

// Abstract data type for an editable string buffer.
//
// A buffer is a sequence of zero or more non-null characters
// followed by a null.
//
// It also tracks n cursor positions, each of which is a pointer
// to some character.
//
// Initially, the buffer is empty, and all cursors point to the null.
//
// For the functions bufferInsert, bufferCursorForward,
// and BufferCursorBack, specifying a cursor index that is
// too large results in the function having no effect.

#define LEFT (0)
#define RIGHT (1)

struct node{
    char value;
    struct node * next[2];
};

struct buffer{
    size_t size;
    size_t num_cursors;
    struct node ** cursors;
    struct node * first_node;
};

// Make a buffer with n cursors.
Buffer bufferCreate(size_t n){
    struct buffer * b = malloc(sizeof(struct buffer));
    b->size = 0;
    b->num_cursors = n;

    // intialize first node
    b->first_node = malloc(sizeof(struct node));
    b->first_node->value = '\0';
    b->first_node->next[LEFT] = b->first_node->next[RIGHT] = b->first_node;

    // initialize cursors
    b->cursors = malloc(n * sizeof(struct node *));
    for(size_t i = 0; i<n; i++){
       b->cursors[i] = b->first_node;
    }

    return b;
}

// Free all space used by a buffer.
void bufferDestroy(Buffer b){
    // delete the chain of nodes
    // go to last one
    struct node * cur_node = b->first_node;
    while(cur_node->value != '\0'){
        cur_node = cur_node->next[RIGHT];
    }
    while(cur_node != b->first_node){
        cur_node = cur_node->next[LEFT];
        free(cur_node->next[RIGHT]);
    }
    free(b->first_node);

    free(b->cursors);
    free(b);
}

// Insert a new non-null character before the i-th cursor position.
// All cursors continue to point to the same character
// they pointed to before.
// If i is out of range or c is '\0', has no effect.
void bufferInsert(Buffer b, size_t i, char c){
    if (i >= b->num_cursors || c == '\0'){
        return;
    }
    struct node * cur_node = b->cursors[i];
    struct node * new_node = malloc(sizeof(struct node));
    new_node->value = c;
    new_node->next[LEFT] = cur_node->next[LEFT];
    new_node->next[RIGHT] = cur_node;
    if (cur_node == b->first_node){
        b->first_node = new_node;
    }

    cur_node->next[LEFT]->next[RIGHT] = new_node;
    cur_node->next[LEFT] = new_node;
    b->size++;
}

// Advance cursor i one position.
// If i is out of range, or the i-th cursor
// already points to the final '\0',
// has no effect.
void bufferCursorForward(Buffer b, size_t i){
    if (i >= b->num_cursors){
        return;
    }
    struct node * cur_loc = b->cursors[i];
    if (cur_loc->value == '\0'){
        return;
    }
    b->cursors[i] = cur_loc->next[RIGHT];
}

// Move cursor i back one position.
// If i is out of range, or the i-th cursor
// already points to the first character,
// has no effect.
void bufferCursorBack(Buffer b, size_t i){
    if (i >= b->num_cursors){
        return;
    }
    struct node * cur_loc = b->cursors[i];
    if (cur_loc == b->first_node){
        return;
    }
    b->cursors[i] = cur_loc->next[LEFT];
}

// Return the number of cursors in a buffer
size_t bufferCursors(Buffer b){
    return b->num_cursors;
}

// Return the number of characters in a buffer,
// not including the final null.
size_t bufferSize(Buffer b){
    return b->size;
}

// Return the characters in a buffer
// as a null-terminated sequence of chars.
// Return value is malloc'd and should be freed by caller.
char *bufferContents(Buffer b){
    char * contents = malloc(sizeof(char) * (b->size + 1));
    struct node * cur_node = b->first_node;
    for (size_t i = 0; i < (b->size + 1); i++, cur_node = cur_node->next[RIGHT]){
        contents[i] = cur_node->value;
    }
    return contents;
}
