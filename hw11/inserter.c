#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

#include "inserter.h"

#define NUM_KIDS (2)
#define LEFT (0)
#define RIGHT (1)

struct inserter{
    struct element ** header;
    struct node * root_pointer;
};

struct node{
    char * string;
    struct element * e;
    struct node * kids[NUM_KIDS];
    int priority;
};

// Create a new Inserter to manage the list 
// whose head is stored in *head.
// This list should start empty.
Inserter inserterCreate(struct element **head){
    Inserter i = malloc(sizeof(struct inserter));
    i->root_pointer = calloc(1, sizeof(struct node));
    i->header = head;
    return i;
}

static void inserterRotate(struct node ** root, int side){
    struct node * old_root = *root;
    struct node * new_root = (*root)->kids[side];
    struct node * other = new_root->kids[!side];

    *root = new_root;
    new_root->kids[!side] = old_root;
    old_root->kids[side] = other;
}

static void inserterAddInternal
(struct node ** cur, const char * s, struct element ** top, struct element ** before){
    size_t len = strlen(s);
    if ((*cur)->string){
        int comp = strcmp(s, (*cur)->string);
        // s aleady in tree
        if (!comp){
            return;
        }
        int side = comp > 0;
        // the side to insert is empty
        if(!(*cur)->kids[side]){
            struct node * new = malloc(sizeof(struct node));
            new->string = malloc(len + 1);
            strcpy(new->string, s);
            new->priority = rand();
            for (int i = 0; i < NUM_KIDS; i++){
                new->kids[i] = 0;
            }

            // make a new elt
            new->e = malloc(sizeof(struct element));
            new->e->string = malloc(len + 1);
            strcpy(new->e->string, s);

            // s needs to go on right but there is no right kid   
            // thus s will be placed AFTER root_pointer
            if (side){
                new->e->next = (*cur)->e->next;
                (*cur)->e->next = new->e;
            }
            // s needs to go on left but there is no left kid
            // s needs to be made the header
            else if (strcmp((*top)->string, s) > 0){
                new->e->next = *top;
                * top = new->e;   
            }
            // s needs to be placed AFTER before AND before the current node
            else{
                new->e->next = (*before)->next;
                (*before)->next = new->e;
            }

            (*cur)->kids[side] = new;
        }
        else {
            // keep before at what it is
            if (strcmp((*cur)->kids[side]->string, s) > 0){
                // before is not null
                if (before && * before){
                    inserterAddInternal(&(*cur)->kids[side], s, top, before);
                } else{
                    inserterAddInternal(&(*cur)->kids[side], s, top, &(*cur)->e);
                }
            }
            else{
                inserterAddInternal(&(*cur)->kids[side], s, top, &(*cur)->kids[side]->e);
            }
            // preserve heap property
            if ((*cur)->kids[side]->priority > (*cur)->priority){
                inserterRotate(cur, side);
            }
        }
    }
    // first node is completely empty
    else{
        //*cur = malloc(sizeof(struct node));
        (*cur)->string = malloc(len + 1);
        strcpy((*cur)->string, s);
        (*cur)->priority = rand();

        (*top) = malloc(sizeof(struct element));
        (*top)->next = 0;
        (*top)->string = malloc(len + 1);
        strcpy((*top)->string, s);
        (*cur)->e = *top;
    }
}

// Add a string to he managed list if not
// already present.
// The new string will be a malloc'd copy
// of s.
void inserterAdd(Inserter i, const char *s){
    inserterAddInternal(&i->root_pointer, s, i->header, 0);
}

static void inserterDestroyInternal(struct node * root){
    if(root){
        for(int i = 0; i < NUM_KIDS; i++){
            inserterDestroyInternal(root->kids[i]);
        }
        free(root->string);
        free(root);
    }
}

// Free all space used by the given Inserter.
// This will *not* free any space used by
// the managed linked list.
void inserterDestroy(Inserter i){
    inserterDestroyInternal(i->root_pointer);
    free(i);
}


