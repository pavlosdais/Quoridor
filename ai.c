#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include "helper_commands.h"
#include "bfs.h"
#include <float.h>

#define INFINITY FLT_MAX
#define NEG_INFINITY FLT_MIN

typedef struct player {
    int i;
    int j;
    int walls;
} player;

// initial call -> minimax(currentPosition, depth, -infinity, +infinity, true)

float minimax(char**wall_matrix, int boardsize, char depth, float alpha, float beta, char maximizingPlayer, player* black, player* white, char pl, char *move, int *x, int *y, char* or)
{
    if (depth == 0 || white->i==boardsize-1 || black->i==0)  // End of search - base case
    {
        float positionEval;
        if(!positionEvaluation(black, white, boardsize, wall_matrix, &positionEval)) printf("Allocation failure\n");
        return positionEval;
    }

    float eval;
    if (maximizingPlayer)  // white
    {
        float max_eval = NEG_INFINITY;

        // check each possible move

        // check pawn advancement
        
        if (!wallBelow(white->i, white->j, wall_matrix, boardsize)) // up
        {
            --white->i;  
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 0, black, white, pl, move, x, y, or);
            if (eval >= alpha)
            {
                alpha = eval;
                if (pl == 'w')
                {
                    *move = 'm';
                    *x = white->i;
                    *y = white->j;
                }
            }
            ++white->i;  // reset movement
        }

        if (!wallOnTheRight(white->i, white->j, wall_matrix, boardsize))  // right
        {
            ++white->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 0, black, white, pl, move, x, y, or);
            if (eval >= alpha)
            {
                alpha = eval;
                if (pl == 'w')
                {
                    *move = 'm';
                    *x = white->i;
                    *y = white->j;
                }
            }
            --white->j;  // reset movement
        }

        if (!wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // left
        {
            --white->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 0, black, white, pl, move, x, y, or);
            if (eval >= alpha)
            {
                alpha = eval;
                if (pl == 'w')
                {
                    *move = 'm';
                    *x = white->i;
                    *y = white->j;
                }
            }
            ++white->j;  // reset movement
        }

        if (!wallAbove(white->i, white->j, wall_matrix, boardsize))
        {
            ++white ->i;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 0, black, white, pl, move, x, y, or);
            if (eval >= alpha)
            {
                alpha = eval;
                if (pl == 'w')
                {
                    *move = 'm';
                    *x = white->i;
                    *y = white->j;
                }
            }
            --white->i;  // reset movement
        }

        // check wall placement
        for (int i = 1; i < boardsize; i++)
        {
            for (int j = 0; j < boardsize; j++)
            {
                if (!there_is_a_wall(i, j, wall_matrix, boardsize))  // a wall isn't already placed there
                {
                    wall_matrix[i][j] = 'b';  // place horizontal wall
                    if (!there_is_a_path(wall_matrix, boardsize, white, black)) wall_matrix[i][j] = 0;
                    else
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 0, black, white, pl, move, x, y, or);
                        wall_matrix[i][j] = 0;  // reset placement
                        if (eval >= alpha)
                        {
                            alpha = eval;
                            if (pl == 'w')
                            {
                                *or = 'h';
                                *move = 'w';
                                *x = i;
                                *y = j;
                            }
                        }
                        if (beta <= alpha) break;
                    }
                    
                    wall_matrix[i][j] = 'r';  // place vertical wall
                    if (!there_is_a_path(wall_matrix, boardsize, white, black)) wall_matrix[i][j] = 0;
                    else
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 0, black, white, pl, move, x, y, or);
                        wall_matrix[i][j] = 0;  // reset placement
                        if (pl == 'w')
                        {
                            *or = 'v';
                            *move = 'w';
                            *x = i;
                            *y = j;
                        }
                        if (beta <= alpha) break;
                    }
                }
            }
        }
        return max_eval;
    }
    else  // !maximizingPlayer - black
    {
        float min_eval = INFINITY;
        // check each possible move
        
        // check pawn advancement

        if (!wallAbove(black->i, black->j, wall_matrix, boardsize))  // down
        {
            ++black->i;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 1, black, white, pl, move, x, y, or);
            if (eval <= beta)
            {
                beta = eval;
                if (pl == 'b')
                {
                    *move = 'm';
                    *x = black->i;
                    *y = black->j;
                }
            }
            --black->i;  // reset movement
        }
        
        if (!wallOnTheRight(black->i, black->j, wall_matrix, boardsize))  // right
        {
            ++black->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 1, black, white, pl, move, x, y, or);
            if (eval <= beta)
            {
                beta = eval;
                if (pl == 'b')
                {
                    *move = 'm';
                    *x = black->i;
                    *y = black->j;
                }
            }
            --black->j;  // reset movement
        }

        if (!wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // left
        {
            --black->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 1, black, white, pl, move, x, y, or);
            if (eval <= beta)
            {
                beta = eval;
                if (pl == 'b')
                {
                    *move = 'm';
                    *x = black->i;
                    *y = black->j;
                }
            }
            ++black->j;  // reset movement
        }
        
        if (!wallBelow(black->i, black->j, wall_matrix, boardsize))  // up
        {
            --black->i;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 1, black, white, pl, move, x, y, or);
            if (eval <= beta)
            {
                beta = eval;
                if (pl == 'b')
                {
                    *move = 'm';
                    *x = black->i;
                    *y = black->j;
                }
            }
            ++black->i;  // reset movement
        }

        // check wall placement
        for (int i = 1; i < boardsize-1; i++)
        {
            for (int j = 0; j < boardsize-1; j++)
            {
                if (!there_is_a_wall(i, j, wall_matrix, boardsize))  // a wall isn't already placed there
                {

                    wall_matrix[i][j] = 'b';  // place horizontal wall
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 1, black, white, pl, move, x, y, or);
                    wall_matrix[i][j] = 0;  // reset placement
                    if (eval <= beta)
                    {
                        beta = eval;
                        if (pl == 'b')
                        {
                            *or = 'h';
                            *move = 'w';
                            *x = i;
                            *y = j;
                        }
                    }
                    if (beta <= alpha) break;

                    wall_matrix[i][j] = 'r';  // place vertical wall 
                    if (!there_is_a_path(wall_matrix, boardsize, white, black)) wall_matrix[i][j] = 0;
                    else
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 1, black, white, pl, move, x, y, or);
                        wall_matrix[i][j] = 0;  // reset placement

                        if (eval <= beta)
                        {
                            beta = eval;
                            if (pl == 'b')
                            {
                                *or = 'v';
                                *move = 'w';
                                *x = i;
                                *y = j;
                            }
                        }
                        if (beta <= alpha) break;
                    }
                }
            }
        }
        return min_eval;
    }
}
