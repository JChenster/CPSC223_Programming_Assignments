#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "orderBook.h"

struct heap{
    int * a;
    size_t capacity;
    size_t size;
};

struct orderBook{
    struct heap * buy_heap;
    struct heap * sell_heap;
};

#define INITIAL_SIZE (32)
#define SIZE_MULTIPLIER (2)

// Make a new empty order book.
OrderBook orderBookCreate(void){
    OrderBook ob = malloc(sizeof(struct orderBook));
    ob->buy_heap = malloc(sizeof(struct heap));
    ob->sell_heap = malloc(sizeof(struct heap));
    ob->buy_heap->capacity = ob->sell_heap->capacity = INITIAL_SIZE;
    ob->buy_heap->size = ob->sell_heap->size = 0;
    ob->buy_heap->a = calloc(INITIAL_SIZE, sizeof(int));
    ob->sell_heap->a = calloc(INITIAL_SIZE, sizeof(int));
    return ob;
}

// Destroy an order book,
// freeing all space
// and discarding all pending orders.
void orderBookDestroy(OrderBook ob){
    free(ob->buy_heap->a);
    free(ob->sell_heap->a);
    free(ob->buy_heap);
    free(ob->sell_heap);
    free(ob);
}

// Following 5 functions are from heapsort.c, 04-12-2021 lecture
static void
swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

static size_t
parent(size_t i)
{
    return (i-1)/2;
}

static size_t
child(size_t i, int side)
{
    return 2*i + 1 + side;
}

static void
floatDown(size_t i, size_t n, int *a)
{
    size_t bigger = child(i,0);

    // need to check if bigger >= n
    if(bigger < n) {
        if(bigger + 1 < n && a[bigger+1] > a[bigger]) {
            // right child exists and is bigger
            bigger = bigger + 1;
        }

        // bigger now is index of the bigger child
        // is that child bigger than me?
        if(a[bigger] > a[i]) {
            swap(&a[bigger], &a[i]);
            floatDown(bigger, n, a);
        }
    }
}

static void
floatUp(size_t i, int *a)
{
    if(i != 0 && a[parent(i)] < a[i]) {
        swap(&a[parent(i)], &a[i]);
        floatUp(parent(i), a);
    }
}

// removes the max elt from root
// updates heap by moving bottom right most elt to root
// floats down the root until it is greater than larger of children
// need to update the size of heap after called
static void heapDelete(struct heap * h){
    h->size--;
    h->a[0] = h->a[h->size];
    floatDown(0, h->size, h->a);
}

// inserts a new elt into heap
// need to increment size of heap after called
static void heapInsert(struct heap * h, int new){
    h->a[h->size] = new;
    floatUp(h->size, h->a);
    h->size++;
    if (h->size >= h->capacity){
        h->capacity *= 2;
        h->a = realloc(h->a, h->capacity * sizeof(int));
    }
}

// returns root if it exists
static int heapPeek(struct heap * h){
    if (h->size){
        return h->a[0];
    } else{
        return 0;
    }
}

// Enter a new order in the order book.
//
// If price > 0, it represents a buy order.
// Return value will be price p2 of sell order
// maximizing price + p2 > 0, or 0 if there
// is no such sell order.
//
// If price < 0, it represents a sell order.
// Return value will be price p2 of buy order
// maximizing price + p2 > 0, or 0 if there
// is no such buy order.
//
// In either of the above cases, if 0 is returned,
// then price is entered into the order book
// as a pending order available for future matches.
// If a nonzero value is returned, the corresponding
// pending order is removed.
//
// If price == 0, no order is entered and 
// return value is 0.
int orderBookInsert(OrderBook ob, int price){
    if (price == 0){
        return 0;
    }
    int top;
    // buy order
    if (price > 0){
        // add to buy queue
        top = heapPeek(ob->sell_heap);
        if (top == 0 || price + top <= 0){
            heapInsert(ob->buy_heap, price);
            return 0;
        }
        // there is a sell order that is profitable
        else{
            heapDelete(ob->sell_heap);
            return top;
        }
    }
    // sell order
    else{
        top = heapPeek(ob->buy_heap);
        // add to sell queue
        if (top == 0 || price + top <= 0){
            heapInsert(ob->sell_heap, price);
            return 0;
        }
        else{
            heapDelete(ob->buy_heap);
            return top;
        }
    }
}


