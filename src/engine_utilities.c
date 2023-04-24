#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "../include/typedefs.h"
#include "../include/structs.h"
#include "../include/utilities.h"
#include "../include/engine_utilities.h"
#include "../include/dfs.h"
#include "../include/bfs.h"

bool there_is_a_path_black(gameState gs)
{
    if (!dfs(gs->boardsize, gs->wall_matrix, gs->white.i, gs->white.j, gs->boardsize-1, 'w'))
        return false;

    if (!dfs(gs->boardsize, gs->wall_matrix, gs->black.i, gs->black.j, 0, 'b'))
        return false;
    
    // the path is not blocked for neither player, return the minimum steps white needs to win
    return true;
}

bool there_is_a_path_white(gameState gs)
{
    if (!dfs(gs->boardsize, gs->wall_matrix, gs->black.i, gs->black.j, 0, 'b'))
        return false;
    
    if (!dfs(gs->boardsize, gs->wall_matrix, gs->white.i, gs->white.j, gs->boardsize-1, 'w'))
        return false;
    
    // the path is not blocked for neither player, return the minimum steps black needs to win
    return true;
}

/* positionEvaluation is a heuristic function used for the evaluation of the current position of the game.
It takes into account the distance each player needs in order to win (factor = 1), the number of steps each player
needs to get to the next row (factor = 0.6) as well as the number of walls each player has (factor = 0.8) in order
to give an advantage (or lack of) for a certain player. When it's positive it calculates that the position is
advantageous for white and when it's negative it calculates that black has an advantage. If it's 0 it means that
the position is equal so neither player has an advantage. */

#define WHITE_WINS 999999
#define BLACK_WINS -999999

int positionEvaluation(gameState gs)
{
    if (gs->black.i == 0)  // black reached his goal row - win
        return BLACK_WINS;
    else if (gs->white.i == gs->boardsize-1)  // white reached his goal row - win
        return WHITE_WINS;

    // calculate the distance white needs to get to the goal row (win)
    int whiteDistanceFromWinning = bfs(gs->boardsize, gs->wall_matrix, gs->white.i, gs->white.j, gs->boardsize-1);

    // calculate the distance black needs to get to the goal row (win)
    int blackDistanceFromWinning = bfs(gs->boardsize, gs->wall_matrix, gs->black.i, gs->black.j, 0);

    // calculate the distance white needs to get to the next row
    int whiteDistanceFromNextRow = bfs(gs->boardsize, gs->wall_matrix, gs->white.i, gs->white.j, gs->white.i+1);

    // calculate the distance black needs to get to the next row
    int blackDistanceFromNextRow = bfs(gs->boardsize, gs->wall_matrix, gs->black.i, gs->black.j, gs->black.i-1);

    return 10*(blackDistanceFromWinning-whiteDistanceFromWinning) + 6*(blackDistanceFromNextRow-whiteDistanceFromNextRow) + 7*(gs->white.walls-gs->black.walls);
}

void updateWallPlacement(returningMove* evalMove, move a, int* eval, int* best_eval)
{
    evalMove->move = 'w';

    *best_eval = *eval;
    evalMove->or = a->or_prevj;
    evalMove->x = a->i;
    evalMove->y = a->j;
}

void updatePawnMovement(returningMove* evalMove, move a, int* eval, int* best_eval)
{
    evalMove->move = 'm';

    *best_eval = *eval;
    evalMove->x = a->i;
    evalMove->y = a->j;
}
