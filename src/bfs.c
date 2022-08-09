#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "../include/typedefs.h"
#include "../include/utilities.h"

typedef struct QueueNode
{
    // player's coordinates
    int i;
    int j;

    struct QueueNode* next;
}
QueueNode;

typedef struct 
{
    QueueNode* head;
    QueueNode* tail;
}
queue;

/* matrices with the coordinates of adjacent cells of (c,r) 
-south (c-1,r)
-north (c+1,r)
-east (c,r+1)
-west (c,r-1) */
static char dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, 1, -1};

// Function Prototypes
static void explore_neighbours(queue *q, const int cur_r, const int cur_c, const int boardsize, char** m, char** have_visited, uint* nodes_next_in_layer);
static void init_queue(queue* q);
static void enqueue(queue* q, const int i, const int j);
static void dequeue(queue* q, int* i, int* j);
static bool isQueueEmpty(queue* q);

/* Breadth-first search
-wiki page: https://en.wikipedia.org/wiki/Breadth-first_search
-pathfinding algorithm: https://www.youtube.com/watch?v=KiCBXu4P-2Y
Function returns the steps a cell requires moving only up, down, left and right from a certain cell (startx, starty)
in order to reach a certain row goalx. In this context it calculates how many steps a player either with the black or white
colour needs in order to reach the row and win. If such path doesn't exist it returns -1, in any other case a positive integer. */

uint bfs(const int boardsize, char** m, const int startx, const int starty, const int goalx)
{
    // create and intialize an visited grid
    char **have_visited = malloc(sizeof(char*) * boardsize);
    assert(have_visited != NULL);  // allocation failure
    for (int i = 0; i < boardsize; i++)
    {
        have_visited[i] = calloc(sizeof(char), boardsize);
        assert(have_visited[i] != NULL);  // allocation failure
    }

    uint move_count = 0, nodes_left_in_layer = 1, nodes_next_in_layer = 0, nr, nc;

    // create and initialize queue
    queue q;
    init_queue(&q);

    enqueue(&q, startx, starty);

    while (!isQueueEmpty(&q))
    {
        dequeue(&q, &nr, &nc);
        
        if (nr == goalx)  // reached the goal row
            break;
        
        // enqueue adjecent cells
        explore_neighbours(&q, nr, nc, boardsize, m, have_visited, &nodes_next_in_layer);

        if (--nodes_left_in_layer == 0)
        {
            nodes_left_in_layer = nodes_next_in_layer;
            nodes_next_in_layer = 0;
            move_count++;
        }
    }
    
    // free array
    for (int i = 0; i < boardsize; i++) free(have_visited[i]);
    free(have_visited);

    // free queue
    QueueNode *tmp = NULL;
    while(q.head != NULL)
    {
        tmp = q.head;
        q.head = q.head->next;
        free(tmp);
    }

    return move_count;
}

static void explore_neighbours(queue* q, const int cur_r, const int cur_c, const int boardsize, char** m, char** have_visited, uint* nodes_next_in_layer)
{
    int rr, cc;

    for (small_int i = 0; i < 4; i++)
    {
        // new adjacent cell (rr, cc)
        rr = cur_r + dr[i];
        cc = cur_c + dc[i];
        if (rr < 0 || cc < 0 || rr >= boardsize || cc >= boardsize)  // cell out of bounds
            continue;
        if (have_visited[rr][cc])  // already have visited that cell
            continue;
        
        // There's a wall on the way
        if (i == 0 && wallBelow(cur_r, cur_c, m, boardsize)) continue;  // south
        else if (i == 1 && wallAbove(cur_r, cur_c, m, boardsize)) continue;  // north
        else if (i == 2 && wallOnTheRight(cur_r, cur_c, m, boardsize)) continue;  // east
        else if (i == 3 && wallOnTheLeft(cur_r, cur_c, m, boardsize)) continue;  // west

        // cell is valid, enqueue it
        enqueue(q, rr, cc);

        // mark the cell as visited
        have_visited[rr][cc] = true;
        (*nodes_next_in_layer)++;
    }
}

// initialize queue
static void init_queue(queue* q)
{
    q->head = NULL;
    q->tail = NULL;
}

// return true if the queue is empty
static bool isQueueEmpty(queue* q)
{
    return (q->head == NULL);
}

// enqueue operation
static void enqueue(queue* q, int i, int j)
{
    QueueNode* newnode = malloc(sizeof(QueueNode));
    assert(newnode != NULL);  // allocation failure

    newnode->i = i;
    newnode->j = j;

    newnode->next = NULL;

    if (q->tail != NULL)  // queue was not empty previously
        q->tail->next = newnode;

    q->tail = newnode;

    // empty queue
    if (isQueueEmpty(q)) 
        q->head = newnode;
}

// dequeue operation
static void dequeue(queue* q, int* i, int* j)
{
    QueueNode* tmp = q->head;
    *i = tmp->i;
    *j = tmp->j;

    q->head = q->head->next;
    free(tmp);

    // reset empty queue
    if (isQueueEmpty(q)) q->tail = NULL;
}
