#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/pq.h"

typedef struct n
{
    Pointer data;
}
n;
typedef struct n* node;

typedef struct _pq
{
    node arr;          // array of nodes containing the data
    uint front, rear;  // front and rear indices
    uint capacity;     // max capacity of the heap
}
_pq;

typedef struct _move
{
    char col;
    unsigned char i, j;
    unsigned char previ;
    int move_eval;
    char move;
    unsigned char or_prevj;
}
*move;

// function prototypes
static void swap_nodes(node a, node b);
static int compare_eval(Pointer v1, Pointer v2);

#define compare_eval(v1, v2)  (((move)v1)->move_eval - ((move)v1)->move_eval)

void pq_init(PQ* pq)
{
    *pq = malloc(sizeof(_pq));
    assert(*pq != NULL);  // allocation failure
    
    // allocate memory for the array of nodes
    (*pq)->arr = calloc(MIN_SIZE, sizeof( *((*pq)->arr)) );
    assert((*pq)->arr != NULL);  // allocation failure

    (*pq)->rear = (*pq)->front = 0;
    (*pq)->capacity = MIN_SIZE;
}

unsigned int pq_size(PQ pq)
{
    return pq->rear;
}

bool is_pq_empty(PQ pq)
{
    return (pq->rear == pq->front);
}

void pq_insert(PQ pq, Pointer value)
{
    pq->arr[pq->rear++].data = value;

    // queue is full, double its size
    if (pq->rear == pq->capacity)
    {
        pq->capacity *= 2;

        node new_arr = realloc(pq->arr, pq->capacity * sizeof(*new_arr));
        assert(new_arr != NULL);  // allocation failure
        
        pq->arr = new_arr;
    }
}

// sort the queue using the quicksort algorithm
// source: https://www.programiz.com/dsa/quick-sort
static void quicksort(PQ pq, int left, int right);
void pq_sort(PQ pq, int start)
{
    quicksort(pq, start, pq->rear-1);
}

static int partition(PQ pq, int left, int right)
{
    n pivot = pq->arr[right];

    int i = left-1;
 
    for (int j = left; j < right; j++)
    {
        if (compare_eval(pq->arr[j].data, pivot.data) < 0)
            swap_nodes(&(pq->arr[++i]), &(pq->arr[j]));
    }
    swap_nodes(&(pq->arr[i+1]), &(pq->arr[right]));

    return i+1;
}

static void quicksort(PQ pq, int left, int right)
{
    if (left < right)
    {
        int pi = partition(pq, left, right);
        
        quicksort(pq, left, pi - 1);
        quicksort(pq, pi + 1, right);
    }
}

Pointer pq_remove(PQ pq)
{
    return pq->arr[pq->front++].data;
}

void pq_destroy(PQ pq)
{
    for (uint i = 0; i < pq->rear; i++)
        free(pq->arr[i].data);
    
    free(pq->arr);
    free(pq);
}

static void swap_nodes(node a, node b)
{
    node tmp = a->data;
    a->data = b->data;
    b->data = tmp;
}
