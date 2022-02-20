#include <stdio.h>
#include <stdlib.h>

typedef struct QueueNode
{
    // coordinates
    int i;
    int j;

    struct QueueNode* next;
}
QueueNode;

// Function Prototypes
void explore_neighbours(QueueNode** q, int cur_r, int cur_c, int boardsize, char**m, char**have_visited, int* nodes_next_in_layer);
void Enqueue(QueueNode** q, int i, int j);
void Dequeue(QueueNode** q, int *ri, int *rj);
void freeQueue(QueueNode** q);
char isQueueEmpty(QueueNode** q);
void CreateQueueNode(int i, int j, QueueNode** nn);
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

    QueueNode* q = NULL;  // create queue

    Enqueue(&q, startx, starty);

    while (!isQueueEmpty(&q))
    {
        Dequeue(&q, &nr, &nc);
        
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

    // free Queue
    freeQueue(&q);
    
    if (reached_the_end) return move_count;
    return -1;
}

void explore_neighbours(QueueNode** q, int cur_r, int cur_c, int boardsize, char**m, char**have_visited, int* nodes_next_in_layer)
{
    /* 
    matrices with the coordinates of adjacent cells of (c,r) 
    -south (c-1,r)
    -north (c+1,r)
    -east (c,r+1)
    -west (c,r-1)
    */
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

        Enqueue(q, rr, cc);
        have_visited[rr][cc] = 1;
        (*nodes_next_in_layer)++;
    }
}

void Enqueue(QueueNode** q, int i, int j) 
{
    if (q == NULL) return;  // empty queue
    QueueNode* nn;
    CreateQueueNode(i, j, &nn);

    nn->next = (*q);
    (*q) = nn;
}

void Dequeue(QueueNode** q, int *ri, int *rj)
{ 
    while (*q != NULL)
    {
        if ((*q)->next == NULL)
        {
            QueueNode *temp = *q;

            *ri = (*q)->i;
            *rj = (*q)->j;

            *q = NULL;
            free(temp);
            break;
        }
        q = &((*q)->next);
    }
}

void CreateQueueNode(int i, int j, QueueNode** nn)
{
    *nn = malloc(sizeof(QueueNode));

    (*nn)->next = NULL;
    (*nn)->i = i;
    (*nn)->j = j;
}

char isQueueEmpty(QueueNode** q)
{
    if (*q != NULL) return 0;
    else return 1;
}

void freeQueue(QueueNode** q)
{
    QueueNode *temp = NULL;
    while (*q != NULL)
    {
        temp = *q;
        *q = (*q)->next;
        free(temp);
    }
    free(*q);
}

char wallBelow(int i, int j, char **w_mtx, int boardsize) {
    if (i==0) return 0;
    return (w_mtx[i][j]=='b' || (j>0 && w_mtx[i][j-1]=='b'));
}

char wallAbove(int i, int j, char **w_mtx, int boardsize) {
    if (i==boardsize-1) return 0;
    return wallBelow(i+1, j, w_mtx, boardsize);
}

char wallOnTheRight(int i, int j, char **w_mtx, int boardsize) {
    if (j==boardsize-1) return 0;
    return (w_mtx[i][j]=='r' || (i<boardsize-1 && w_mtx[i+1][j]=='r'));
}

char wallOnTheLeft(int i, int j, char **w_mtx, int boardsize) {
    if (j==0) return 0;
    return wallOnTheRight(i, j-1, w_mtx, boardsize);
}

char there_is_a_wall(int i, int j, char **wall_matrix, int boardsize)
{
    if (wallOnTheLeft(i, j, wall_matrix, boardsize) ) return 1;
    else if (wallOnTheLeft(i, j, wall_matrix, boardsize) ) return 1;
    else if (wallAbove(i, j, wall_matrix, boardsize) ) return 1;
    else if (wallBelow(i, j, wall_matrix, boardsize) ) return 1;
    else return 0;
}