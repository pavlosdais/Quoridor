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

// initial call -> minimax(currentPosition, depth, -infinity, +infinity, true)

float minimax(char**wall_matrix, int boardsize, char depth, float alpha, float beta, char maximizingPlayer, player* black, player* white)
{
    if (depth == 0 || white->i==boardsize-1 || black->i==0)  // End of search
    {
        return positionEvaluation(black, white, boardsize, wall_matrix);
    }

    int eval;
    if (maximizingPlayer)  // white
    {
        float max_eval = FLT_MIN;

        // check each possible move

        // check pawn advancement
        
        if (!wallBelow(white->i, white->j, wall_matrix, boardsize)) // up
        {
            --white->i;  
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 0, black, white);
            if (eval >= alpha)
            {
                alpha = eval;
            }
            ++white->i;  // reset movement
        }

        if (!wallOnTheRight(white->i, white->j, wall_matrix, boardsize))  // right
        {
            ++white->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 0, black, white);
            if (eval >= alpha)
            {
                alpha = eval;
            }
            --white->j;  // reset movement
        }

        if (!wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // left
        {
            --white->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 0, black, white);
            if (eval >= alpha)
            {
                alpha = eval;
            }
            ++white->j;  // reset movement
        }

        if (!wallAbove(white->i, white->j, wall_matrix, boardsize))
        {
            ++white ->i;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 0, black, white);
            if (eval >= alpha)
            {
                alpha = eval;
            }
            --white->i;  // reset movement
        }

        // check wall placement
        for (int i = 0; i < boardsize; i++)
        {
            for (int j = 0; j < boardsize; j++)
            {
                if (is_vertex_valid(i, boardsize) && is_vertex_valid(j, boardsize) && (i != 0 || j != 0))
                {
                    if (!there_is_a_wall(i, j, wall_matrix, boardsize)) wall_matrix[i][j] = 'b';  // place wall below
                    if (!there_is_a_path(wall_matrix, boardsize, white, black)) wall_matrix[i][j] = 0;
                    else
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 0, black, white);
                        wall_matrix[i][j] = 0;  // reset placement
                        if (eval >= alpha)
                        {
                            alpha = eval;
                        }
                        if (beta <= alpha) break;
                    }

                    wall_matrix[i][j] = 'r';  // place wall on the right
                    if (!there_is_a_path(wall_matrix, boardsize, white, black)) wall_matrix[i][j] = 0;
                    else
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 0, black, white);
                        wall_matrix[i][j] = 0;  // reset placement
                        if (eval >= alpha)
                        {
                            alpha = eval;
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
        float min_eval = FLT_MAX;
        // check each possible move
        
        // check pawn advancement

        if (!wallAbove(black->i, black->j, wall_matrix, boardsize))  // down
        {
            ++black->i;  
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 0, black, white);
            if (eval <= beta)
            {
                beta = eval;
            }
            --black->i;  // reset movement
        }
        
        if (!wallOnTheRight(black->i, black->j, wall_matrix, boardsize))  // right
        {
            ++black->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 0, black, white);
            if (eval <= beta)
            {
                beta = eval;
            }
            --black->j;  // reset movement
        }

        if (!wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // left
        {
            --black->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 0, black, white);
            if (eval <= beta)
            {
                beta = eval;
            }
            ++black->j;  // reset movement
        }
        
        if (!wallBelow(black->i, black->j, wall_matrix, boardsize))  // up
        {
            --black->i;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 0, black, white);
            if (eval <= beta)
            {
                beta = eval;
            }
            ++black->i;  // reset movement
        }

        // check wall placement
        for (int i = 1; i < boardsize-1; i++)
        {
            for (int j = 1; j < boardsize-1; j++)
            {
                if (is_vertex_valid(i, boardsize) && is_vertex_valid(j, boardsize) && (i != 0 || j != 0))
                {
                    wall_matrix[i][j] = 'b';  // place wall below
                    if (!there_is_a_wall(i, j, wall_matrix, boardsize)) wall_matrix[i][j] = 0;
                    else
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 1, black, white);
                        wall_matrix[i][j] = 0;  // reset placement
                        if (eval <= beta)
                        {
                            beta = eval;
                        }
                        if (beta <= alpha) break;
                    }

                    wall_matrix[i][j] = 'r';  // place wall on the right
                    if (!there_is_a_path(wall_matrix, boardsize, white, black)) wall_matrix[i][j] = 0;
                    else
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 1, black, white);
                        wall_matrix[i][j] = 0;  // reset placement

                        if (eval <= beta)
                        {
                            beta = eval;
                        }
                        if (beta <= alpha) break;
                    }
                }
            }
        }
        return min_eval;
    }
}
