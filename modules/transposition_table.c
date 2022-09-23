#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/transposition_table.h"

typedef unsigned long long U64;

#define MAX(a, b) (a > b ? a:b)
#define MIN(a, b) (a < b ? a:b)

typedef struct n
{
    U64 position_id;      // "almost" unique position identifier
    unsigned char depth;  // 
    flags flag;           // flag the type of node (UPPERBOUND, LOWERBOUND, EXACT)
    int evaluation;       // evaluation (alpha/beta/exact)
}
n; typedef struct n* node;

typedef struct _tt
{
    node buckets;   // buckets storing the data
    uint capacity;  // number of buckets
    uint elements;  // number of elements in the hash table
}
_tt;

void tt_init(transposition_table* TT)
{
    *TT = malloc(sizeof(_tt));
    assert(*TT != NULL);  // allocation failure
    
    (*TT)->capacity = NUM_OF_ENTRIES;

    (*TT)->buckets = calloc(sizeof(n), (*TT)->capacity);  // allocate memory for the buckets
    assert((*TT)->buckets != NULL);  // allocation failure

    (*TT)->elements = 0;
}

uint tt_size(transposition_table TT)
{
    return TT->elements;
}

void tt_clear(transposition_table TT)
{
    for (uint i = 0; i < TT->capacity; i++)
    {
        TT->buckets[i].position_id = 0;
        TT->buckets[i].flag = 0;
        TT->buckets[i].depth = 0;
        TT->buckets[i].evaluation = 0;
    }
    TT->elements = 0;
}

int tt_search(transposition_table TT, int* alpha, int* beta, int depth, U64 key)
{
    node position = &(TT->buckets[key % TT->capacity]);

    if (position->position_id == key && position->depth >= depth)  // it is the exact same position
    {
        if (position->flag == EXACT)  // exact evaluation
            return position->evaluation;

        if (position->flag == UPPERBOUND)  // lowerbound
            *alpha = MAX(*alpha, position->evaluation);

        else if (position->flag == LOWERBOUND)  // upperbound
            *beta = MIN(*beta, position->evaluation);

        if (*alpha >= *beta)
            return position->evaluation;
    }

    // didn't find the actual position
    return UNKNOWN;
}

void tt_insert(transposition_table TT, int position_eval, int flag, int depth, U64 key)
{
    // find the move
    node position = &(TT->buckets[key % TT->capacity]);
    
    // currently always replace - with a different approach there could be optimizations
    position->evaluation = position_eval;
    position->flag = flag;
    position->depth = depth;
    position->position_id = key;
        
    TT->elements++;
}

void tt_destroy(transposition_table TT)
{
    // free buckets
    free(TT->buckets);

    // free transposition table
    free(TT);
}
