#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "../include/typedefs.h"
#include "../include/utilities.h"

typedef struct StackNode
{
    // player's coordinates
    int i;
    int j;
    
    struct StackNode* next;
}
StackNode;
typedef struct StackNode* StackNodePointer;

typedef struct StackSet
{
    StackNodePointer top;  // node at the top of the stack, NULL if the stack is empty
    char dr[4];
}
StackSet;
typedef struct StackSet* Stack;

static char dc[] = {0, 1, -1, 0};
static isThereWall wall_exists_white[] = {wallBelow, wallOnTheRight, wallOnTheLeft, wallAbove};
static isThereWall wall_exists_black[] = {wallAbove, wallOnTheRight, wallOnTheLeft, wallBelow};

// Function Prototypes
void explore_neighbours_white(Stack, cint, cint, cint, char**, char**);
void explore_neighbours_black(Stack, cint, cint, cint, char**, char**);
static inline void stack_init(Stack*, const char);
static inline bool is_empty_stack(Stack);
static inline void stack_push(Stack, cint, cint);
static inline void stack_pop(Stack, int*, int*);
static inline void stack_destroy(Stack);

/* depth-first search
-wiki page: https://en.wikipedia.org/wiki/Depth-first_search
*/

typedef void (*explore)(Stack q, cint cur_r, cint cur_c, cint boardsize, char** m, char** have_visited);

bool dfs(cint boardsize, char** m, cint startx, cint starty, cint goalx, const char player)
{
    // create and intialize an visited grid
    char **have_visited = malloc(sizeof(char*) * boardsize);
    assert(have_visited != NULL);  // allocation failure
    for (int i = 0; i < boardsize; i++)
    {
        have_visited[i] = calloc(sizeof(char), boardsize);
        assert(have_visited[i] != NULL);  // allocation failure
    }

    int nr, nc;
    char reached_the_end = false;

    // create and initialize stack
    Stack st;
    stack_init(&st, player);
    stack_push(st, startx, starty);

    explore explore_func = explore_neighbours_white;
    if (player == 'b') explore_func = explore_neighbours_black;

    while (!is_empty_stack(st))
    {
        stack_pop(st, &nr, &nc);
        
        if (nr == goalx)  // reached the goal row
        {
            reached_the_end = true;
            break;
        }

        // enqueue adjecent cells
        explore_func(st, nr, nc, boardsize, m, have_visited);
    }

    // free array
    for (int i = 0; i < boardsize; i++) free(have_visited[i]);
    free(have_visited);

    // free stack
    stack_destroy(st);

    return reached_the_end;
}

void inline explore_neighbours_white(Stack q, cint cur_r, cint cur_c, cint boardsize, char** m, char** have_visited)
{   
    int rr, cc;

    for (small_int i = 0; i < 4; i++)
    {
        // new adjacent cell (rr, cc)
        rr = cur_r + q->dr[i];
        cc = cur_c + dc[i];
        if (rr < 0 || cc < 0 || rr >= boardsize || cc >= boardsize)  // cell out of bounds
            continue;
        if (have_visited[rr][cc])  // already have visited that cell
            continue;
        
        // There's a wall on the way
        if (wall_exists_white[i](cur_r, cur_c, m, boardsize))
            continue;
        
        // cell is valid, enqueue it
        stack_push(q, rr, cc);
        
        // mark the cell as visited
        have_visited[rr][cc] = true;
    }
}

void inline explore_neighbours_black(Stack q, cint cur_r, cint cur_c, cint boardsize, char** m, char** have_visited)
{   
    int rr, cc;

    for (small_int i = 0; i < 4; i++)
    {
        // new adjacent cell (rr, cc)
        rr = cur_r + q->dr[i];
        cc = cur_c + dc[i];
        if (rr < 0 || cc < 0 || rr >= boardsize || cc >= boardsize)  // cell out of bounds
            continue;
        if (have_visited[rr][cc])  // already have visited that cell
            continue;
        
        // There's a wall on the way
        if (wall_exists_black[i](cur_r, cur_c, m, boardsize))
            continue;
        
        // cell is valid, enqueue it 
        stack_push(q, rr, cc);
        
        // mark the cell as visited
        have_visited[rr][cc] = true;
    }
}

// initialize stack
static inline void stack_init(Stack* S, const char col)
{
    *S = malloc(sizeof(StackSet));
    assert(*S != NULL);  // allocation failure
    
    (*S)->top = NULL;

    (*S)->dr[1] = 0;
    (*S)->dr[2] = 0;

    if (col == 'b')
    {
        (*S)->dr[0] = 1;
        (*S)->dr[3] = -1;
    }
    else  // col == 'w'
    {
        (*S)->dr[0] = -1;
        (*S)->dr[3] = 1;
    }
}

// return true if the stack is empty
static inline bool is_empty_stack(Stack S)
{
    return S->top == NULL;
}

// push operation
static inline void stack_push(Stack S, cint i, cint j)
{
    StackNodePointer* head = &(S->top);

    // create a new node
    StackNodePointer new_node = malloc(sizeof(StackNode));
    assert(new_node != NULL);  // allocation failure

    // fill the node's contents
    new_node->i = i;
    new_node->j = j;

    new_node->next = *head;
    *head = new_node;
}

// pop operation
static inline void stack_pop(Stack S, int* i, int* j)
{
    StackNodePointer head = S->top;
    *i = head->i;
    *j = head->j;

    StackNodePointer tmp = head;

    S->top = S->top->next;

    free(tmp);
}

// destroy stack
static inline void stack_destroy(Stack S)
{
    StackNodePointer head = S->top;

    while (head != NULL)
    {
        StackNodePointer tmp = head;

        head = head->next;
        free(tmp);
    }

    free(S);
}
