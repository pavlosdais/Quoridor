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
            alpha = max(alpha, eval);
            ++white->i;  // reset
        }

        if (!wallOnTheRight(white->i, white->j, wall_matrix, boardsize))  // right
        {
            ++white->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 0, black, white);
            alpha = max(alpha, eval);
            --white->j;  // reset
        }

        if (!wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // left
        {
            --white->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 0, black, white);
            alpha = max(alpha, eval);
            ++white->j;  // reset
        }
        
        if (wallOnTheLeft(white->i, white->j, wall_matrix, boardsize) && wallOnTheRight(white->i, white->j, wall_matrix, boardsize) && wallBelow(white->i, white->j, wall_matrix, boardsize))  // boxed in - go down
        {
            if (!wallAbove(white->i, white->j, wall_matrix, boardsize))
            {
                ++white ->i;
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 0, black, white);
                alpha = max(alpha, eval);
                --white->i;  // reset
            }
        }

        // check wall placement
        for (int i = 1; i < boardsize; i++)
        {
            for (int j = 1; j < boardsize; j++)
            {
                if (!there_is_a_wall(i, j, wall_matrix, boardsize))
                {
                    wall_matrix[i][j] = 'b';  // place wall below
                    if (!there_is_a_path(wall_matrix, boardsize, white, black)) wall_matrix[i][j] = 0;
                    else eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 0, black, white);

                    wall_matrix[i][j] = 'r';  // place wall on the right
                    if (!there_is_a_path(wall_matrix, boardsize, white, black)) wall_matrix[i][j] = 0;
                    else eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 0, black, white);

                    wall_matrix[i][j] = 0;  // reset placement
                    alpha = max(alpha, eval);
                    if (beta <= alpha) break;
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
            alpha = max(alpha, eval);
            --black->i;  // reset
        }
        
        if (!wallOnTheRight(black->i, black->j, wall_matrix, boardsize))  // right
        {
            ++black->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 0, black, white);
            alpha = max(alpha, eval);
            --black->j;  // reset
        }

        if (!wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // left
        {
            --black->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 0, black, white);
            alpha = max(alpha, eval);
            ++black->j;  // reset
        }
        
        if (wallOnTheLeft(black->i, black->j, wall_matrix, boardsize) && wallOnTheRight(white->i, white->j, wall_matrix, boardsize) && wallAbove(white->i, white->j, wall_matrix, boardsize))  // boxed in - go up
        {
            if (!wallBelow(black->i, black->j, wall_matrix, boardsize))
            {
                --black->i;
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 0, black, white);
                alpha = max(alpha, eval);
                ++black->i;  // reset
            }
        }

        // check wall placement
        for (int i = 1; i <= boardsize-1; i++)
        {
            for (int j = 1; j <= boardsize-1; j++)
            {
                if (!there_is_a_wall(i, j, wall_matrix, boardsize))
                {
                    wall_matrix[i][j] = 'b';  // place wall below
                    if (!there_is_a_path(wall_matrix, boardsize, white, black)) wall_matrix[i][j] = 0;
                    else eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 1, black, white);

                    wall_matrix[i][j] = 'r';  // place wall on the right
                    if (!there_is_a_path(wall_matrix, boardsize, white, black)) wall_matrix[i][j] = 0;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, 1, black, white);

                    wall_matrix[i][j] = 0;  // reset placement
                    alpha = max(alpha, eval);
                    if (beta <= alpha) break;
                }
            }
        }
        return min_eval;
    }
}
