#pragma once  // include at most once

// available (prime) transposition table sizes:
// 393241, 786433, 1572869, 2000003, 3000017, 3500033, 3750001, 4000037, 4250021, 4500007, 5000011, 5250029, 5500003,  
// 6000011, 6291469, 6500003, 7000003, 8000009, 9000011, 10000019, 10500013, 11000027, 12000017, 12500003, 12582917
// 13000027, 13500007, 14000029, 14500001, 15000017, 15500011, 16000057, 16500007, 16999999, 17500013, 18000041
// 18500017, 19000013, 19500017, 20000003, 20500019, 21000037, 21500021, 22000001, 22500011, 23000009, 23500003
// 24000001, 25000009, 26000003, 27000011, 28000019, 29000039, 30000001, 31000003, 32000011, 33000001, 34000009,
// 35000011, 36000007, 37000039, 38000009, 39000001, 40000003, 42500023, 45000017

// every 1mil buckets use ~25 mb of memory, for example:
// 4166447  - 100 mb
// 6249773  - 150 mb
// 8333113  - 200 mb
// 10416463 - 250 mb
// 12499801 - 300 mb

#define NUM_OF_ENTRIES 24000001  // uses about 550 mb of memory

typedef struct hash_table* HashTable;

typedef enum
{
    UPPERBOUND, LOWERBOUND, EXACT
}
flags;
#define UNKNOWN 99999999

typedef struct _tt* transposition_table;  // handle


// initializes transposition table
void tt_init(transposition_table* TT);

// returns the size of the transposition table
uint tt_size(transposition_table TT);

// searches for the position at the transposition table
// returns UNKNOWN if the position does not exist
// otherwise, returns the value associated with the value
int tt_search(transposition_table TT, int* alpha, int* beta, int depth, unsigned long long key);

// inserts position at the transposition table
void tt_insert(transposition_table TT, int position_eval, int flag, int depth, unsigned long long key);

// clears transposition table
void tt_clear(transposition_table TT);

// destroys transposition table
void tt_destroy(transposition_table TT);
