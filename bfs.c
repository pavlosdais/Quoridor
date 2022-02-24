#include <stdio.h>
#include <stdlib.h>
#include "helper_commands.h"

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
    QueueNode *head;
    QueueNode *tail;
}
queue;

// Function Prototypes
void explore_neighbours(queue *q, int cur_r, int cur_c, int boardsize, char**m, char**have_visited, int* nodes_next_in_layer);
void init_queue(queue *q);
void enqueue(queue *q, int i, int j);
void dequeue(queue *q, int *i, int *j);
void freeQueue(queue *q);
char isQueueEmpty(queue *q);
char wallBelow(int i, int j, char **w_mtx, int boardsize);
char wallAbove(int i, int j, char **w_mtx, int boardsize);
char wallOnTheRight(int i, int j, char **w_mtx, int boardsize);
char wallOnTheLeft(int i, int j, char **w_mtx, int boardsize);

/*
Breadth-first search
wiki page: https://en.wikipedia.org/wiki/Breadth-first_search
*/
int bfs(int boardsize, char**m, int startx, int starty, int goalx)
{
    char **have_visited = malloc(sizeof(char*) * boardsize);
    for (int i = 0; i < boardsize; i++) have_visited[i] = calloc(sizeof(char), boardsize);

    int move_count = 0, nodes_left_in_layer = 1, nodes_next_in_layer = 0, nr, nc;
    char reached_the_end = 0;
    queue q;
    init_queue(&q);

    enqueue(&q, startx, starty);

    while (!isQueueEmpty(&q))
    {
        dequeue(&q, &nr, &nc);
        
        if (nr == goalx)
        {
            reached_the_end = 1;
            break;
        }

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
    freeQueue(&q);
    
    if (reached_the_end) return move_count;
    return -1;
}

void explore_neighbours(queue *q, int cur_r, int cur_c, int boardsize, char**m, char**have_visited, int* nodes_next_in_layer)
{
    /*  matrices with the coordinates of adjacent cells of (c,r) 
    -south (c-1,r)
    -north (c+1,r)
    -east (c,r+1)
    -west (c,r-1)  */
    
    int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, 1, -1}, rr, cc;

    for (int i = 0; i < 4; i++)
    {
        // new adjacent cell (rr, cc)
        rr = cur_r + dr[i];
        cc = cur_c + dc[i];
        if (rr < 0 || cc < 0 || rr >= boardsize || cc >= boardsize) continue;  // cell out of bounds
        if (have_visited[rr][cc]) continue;  // already have visited that cell
        
        // There's a wall on the way
        if (i == 0 && wallBelow(cur_r, cur_c, m, boardsize)) continue;  // south
        else if (i == 1 && wallAbove(cur_r, cur_c, m, boardsize)) continue;  // north
        else if (i == 2 && wallOnTheRight(cur_r, cur_c, m, boardsize)) continue;  // east
        else if (i == 3 && wallOnTheLeft(cur_r, cur_c, m, boardsize)) continue;  // west

        enqueue(q, rr, cc);
        have_visited[rr][cc] = 1;
        (*nodes_next_in_layer)++;
    }
}

void init_queue(queue *q)
{
    // Initialize Queue
    q->head = NULL;
    q->tail = NULL;
}

char isQueueEmpty(queue *q)
{
    return (q->head == NULL);
}

void enqueue(queue *q, int i, int j)
{
    QueueNode* newnode = malloc(sizeof(QueueNode));
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

void dequeue(queue *q, int *i, int*j)
{
    if (q->head == NULL) return;
    QueueNode* tmp = q->head;
    *i = tmp->i;
    *j = tmp->j;

    q->head = q->head->next;
    free(tmp);

    // empty queue
    if (isQueueEmpty(q)) q->tail = NULL;
}

void freeQueue(queue *q)
{
    QueueNode *tmp = NULL;
    while(q->head != NULL)
    {
        tmp = q->head;
        q->head = q->head->next;
        free(tmp);
    }
}

