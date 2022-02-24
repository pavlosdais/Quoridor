#include <float.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "helper_commands.h"
#include "bfs.h"
#include <float.h>

#define INFINITY FLT_MAX
#define NEG_INFINITY FLT_MIN

typedef struct player
{
    int i;
    int j;
    int walls;
} player;

typedef struct stackNode
{
    int x;
    int y;
    char* move;  // "wh" --> horizontal wall at (x,y) , "m" --> moved at (x,y)

    struct stackNode* next;
}
stackNode;

void stackInit(stackNode** stack);
char isEmpty(stackNode* stack);
void push(stackNode** stack, int x, int y, char* move);
char pop(stackNode** stack, int *x, int *y, char *play[3]);
void printStack(stackNode* stack);
void freeStack(stackNode* stack);

float minimax(char** wall_matrix, int boardsize, char depth, float alpha, float beta, char maximizingPlayer, player* black, player* white, char pl, char *move, int *x, int *y, char* or)
{
    if (depth == 0 || white->i==boardsize-1 || black->i==0)  // End of search - base case
    {
        return positionEvaluation(black, white, boardsize, wall_matrix);
    }

    int i, eval, x, y;
    char play[3];

    if (maximizingPlayer)  // white
    {
        int maxEval = NEG_INFINITY;
        stackNode* possibleMoves;
        int n = findPossibleMoves(wall_matrix, boardsize, &possibleMoves, black, white);
        for (i = 0; i < n; i++)
        {
            pop(&possibleMoves, &x, &y, &play);

            if (play[0] == 'w')  // wall placed at (x,y)
            {

            }
            else if (play[0] == 'm')  // movement at (x,y)
            {
                              
            }
        }
    }

}

int findPossibleMoves(char** wall_matrix, int boardsize, stackNode** moves, player* opponent, player* pl)
{
    // add pawn movement
    stackInit(&moves);
    if (!wallAbove(pl->i, pl->j, wall_matrix, boardsize)) push(&moves, pl->i+1, pl->j, "m");

    if (!wallBelow(pl->i, pl->j, wall_matrix, boardsize)) push(&moves, pl->i-1, pl->j, "m");

    if (!wallOnTheLeft(pl->i, pl->j, wall_matrix, boardsize)) push(&moves, pl->i, pl->j-1, "m");

    if (!wallOnTheRight(pl->i, pl->j+1, wall_matrix, boardsize)) push(&moves, pl->i, pl->j+1, "m");

    // add wall placement
    for (int i = 1; i < boardsize; i++)
    {
        for (int j = 0; j < boardsize; j++)
        {
            if (!there_is_a_wall(i, j, wall_matrix, boardsize) && (opponent->i+1 == i || opponent->i - 1 == i || opponent->j+1 == j || opponent->j-1 == j))
            {
                
            }
            if (wallOnTheLeft(i, j, wall_matrix, boardsize))
            {
                push(&moves, i, j, "wh");
                push(&moves, i, j, "wv");
            }
            else if (wallOnTheRight(i, j, wall_matrix, boardsize))
            {

            }
            else if (wallAbove(i, j, wall_matrix, boardsize))
            {

            }
            else if(wallBelow(i, j, wall_matrix, boardsize))
            {
                
            }
        }
    }
}

void stackInit(stackNode** stack)
{
    (*stack) = NULL;
}

char isEmpty(stackNode* stack)
{
    if (stack != NULL) return 0;
    return 1;
}

void push(stackNode** stack, int x, int y, char* move)
{
    stackNode* temp = malloc(sizeof(stackNode));

    temp->x = x;
    temp->y = y;
    temp->move = move;

    temp->next = *stack;
    *stack = temp;
}

char pop(stackNode** stack, int *x, int *y, char *play[3])
{
    if (isEmpty(*stack)) return 0;
    stackNode *tmp = *stack;

    *x = tmp->x;
    *y = tmp->y;
    strcpy(*play, tmp->move);

    *stack = (*stack)->next;
    free(tmp);
    return 1;
}

void printStack(stackNode* stack)
{
    while (stack!=NULL)
    {
        printf("%c: %d%d\n", stack->move, stack->x, stack->y);
        stack = stack->next;
    }
    printf("\n");
}

void freeStack(stackNode* stack)
{
    int tmp;
    while (!isEmpty(stack)) pop(&stack);
    free(stack);
}