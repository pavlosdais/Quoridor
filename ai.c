#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include "helper_commands.h"
#include "bfs.h"

typedef struct player {
    int i;
    int j;
    int walls;
} player;

typedef struct stacknode *stackptr;
struct stacknode {
    int i,j;
    char *type; // 'b': black player left (i,j), 'w': white player left (i,j), 'n': new wall placed at (i,j)
    stackptr next;
};

float minimax(char**wall_matrix, int boardsize, char depth, float alpha, float beta, char maximizingPlayer, player* black, player* white)
{
    if (depth == 0 || white->i==boardsize-1 || black->i==0)  // End of search
    {
        return positionEvaluation(black, white, boardsize, wall_matrix);
    }

    if (maximizingPlayer)
    {
        float max_eval = FLT_MIN;
    }
    else  // !maximizingPlayer
    {
        float min_eval = FLT_MAX;
    }
}