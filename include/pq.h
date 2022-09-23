#pragma once  // include at most once

#include <stdbool.h>

// queue's minimum starting size
#define MIN_SIZE 10

typedef void* Pointer;

typedef struct _pq* PQ;  // handle


// initializes priority queue
void pq_init(PQ* pq);

// returns the size of the priority queue
unsigned int pq_size(PQ);

// returns true if the priority queue is empty, false otherwise
bool is_pq_empty(PQ);

// inserts value at the priority queue
void pq_insert(PQ vec, Pointer value);

// sorts priority queue from position start and right
void pq_sort(PQ, int start);

// returns the element with the highest priority as given by the compare function
// works only if the priority queue is sorted
Pointer pq_remove(PQ);

// destroys memory used by the priority queue
void pq_destroy(PQ);
