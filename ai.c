#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "helper_commands.h"
#include "bfs.h"

#define INFINITY FLT_MAX
#define NEG_INFINITY FLT_MIN

float minimax(char** wall_matrix, int boardsize, char depth, float alpha, float beta, player* maximizing, player* minimizing, char pl, char maximizingPlays, player *black, player *white);

typedef struct player
{
    int i;
    int j;
    int walls;
}
player;

typedef struct returningMove
{
    int x;
    int y;
    char move;  // w for wall , m for move
    char or;
}
returningMove;

returningMove *bestMove(char** wall_matrix, int boardsize, char pl, player* black, player* white, char depth)
{
    //if (pl == 'w') minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, 'w', white, black, 1);
    //else minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, 'w', black, white, 1);

    float eval;
    returningMove *best;

    if (pl == 'w')
    {
        // check each possible move
        // check pawn advancement
        float max_eval = NEG_INFINITY;

        if (white->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize))
        {
            --white->i;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, pl, 0, black, white);
            if (eval > max_eval)
            {
                max_eval = eval;
                best->move = 'm';
                best->x = white->i;
                best->y = white->j;
            }
            ++white->i;  // reset movement
        }

        if (white->j < boardsize - 1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))  // right
        {
            ++white->j;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, pl, 0, black, white);
            if (eval > max_eval)
            {
                max_eval = eval;
                best->move = 'm';
                best->x = white->i;
                best->y = white->j;
            }
            --white->j;  // reset movement
        }

        if (white->j > 0 &&!wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // left
        {
            --white->j;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, pl, 0, black, white);
            if (eval > max_eval)
            {
                max_eval = eval;
                best->move = 'm';
                best->x = white->i;
                best->y = white->j;
            }
            ++white->j;  // reset movement
        }
        if (white->i < boardsize - 1 && !wallAbove(white->i, white->j, wall_matrix, boardsize)) // up
        {
            ++white->i;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, pl, 0, black, white);
            if (eval > max_eval)
            {
                max_eval = eval;
                best->move = 'm';
                best->x = white->i;
                best->y = white->j;
            }
            --white->i;  // reset movement
        }

        // check wall placement
        for (int i = 1; i < boardsize; i++)
        {
            for (int j = 0; j < boardsize-1; j++)
            {
                if (!thereIsAWall('b', wall_matrix, boardsize, i, j) && is_vertex_valid(i, boardsize) && is_vertex_valid(j, boardsize))
                {
                    wall_matrix[i][j] = 'b';  // place horizontal wall
                    if (!there_is_a_path(wall_matrix, boardsize, white, black)) wall_matrix[i][j] = 0;
                    else
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, pl, 0, black, white);
                        if (eval > max_eval)
                        {
                            max_eval = eval;
                            best->move = 'w';
                            best->or = 'b';
                            best->x = white->i;
                            best->y = white->j;
                            wall_matrix[i][j] = 0;  // reset placement
                        }
                    }
                }
                if (!thereIsAWall('r', wall_matrix, boardsize, i, j) && is_vertex_valid(i, boardsize) && is_vertex_valid(j, boardsize) && i != 0)
                {
                    wall_matrix[i][j] = 'r';  // place vertical wall
                    if (!there_is_a_path(wall_matrix, boardsize, white, black)) wall_matrix[i][j] = 0;
                    else
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, pl, 0, black, white);
                        if (eval > max_eval)
                        {
                            max_eval = eval;
                            best->move = 'w';
                            best->or = 'r';
                            best->x = white->i;
                            best->y = white->j;
                        }
                        wall_matrix[i][j] = 0; // reset placement
                    }
                } 
            }
        }
        return best;
    }
    else  // pl == 'b'
    {
        float min_eval = INFINITY;
        // check each possible move

        // check pawn advancement
        if (black->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // down
        {
            --black->i;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, black, white, pl, 1, black, white);
            if (eval < min_eval)
            {
                min_eval = eval;
                best->move = 'm';
                best->x = black->i;
                best->y = black->j;
            }
            ++black->i;  // reset movement
        }
        
        if (black->j < boardsize - 1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))  // right
        {
            ++black->j;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, black, white, pl, 1, black, white);
            if (eval < min_eval)
            {
                min_eval = eval;
                best->move = 'm';
                best->x = black->i;
                best->y = black->j;
            }
            --black->j;  // reset movement
        }

        if (black->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // left
        {
            --black->j;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, black, white, pl, 1, black, white);
            if (eval < min_eval)
            {
                min_eval = eval;
                best->move = 'm';
                best->x = black->i;
                best->y = black->j;
            }
            ++black->j;  // reset movement
        }
        
        if (black->i < boardsize -1 && !wallAbove(black->i, black->j, wall_matrix, boardsize))  // up
        {
            ++black->i;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, black, white, pl, 1, black, white);
            if (eval < min_eval)
            {
                min_eval = eval;
                best->move = 'm';
                best->x = black->i;
                best->y = black->j;
            }
            --black->i;  // reset movement
        }

        // check wall placement
        for (int i = 1; i < boardsize-1; i++)
        {
            for (int j = 0; j < boardsize-1; j++)
            {
                if (!thereIsAWall('b', wall_matrix, boardsize, i, j) && is_vertex_valid(i, boardsize) && is_vertex_valid(j, boardsize))
                {
                    wall_matrix[i][j] = 'b';  // place horizontal wall
                    if (!there_is_a_path(wall_matrix, boardsize, white, black)) wall_matrix[i][j] = 0;
                    else
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, black, white, pl, 1, black, white);
                        if (eval < min_eval)
                        {
                            min_eval = eval;
                            best->move = 'w';
                            best->or = 'b';
                            best->x = black->i;
                            best->y = black->j;
                        }
                        wall_matrix[i][j] = 0;  // reset placement
                    }
                }
                if (!thereIsAWall('r', wall_matrix, boardsize, i, j) && is_vertex_valid(i, boardsize) && is_vertex_valid(j, boardsize) && i != 0)
                {
                    wall_matrix[i][j] = 'r';  // place vertical wall 
                    if (!there_is_a_path(wall_matrix, boardsize, white, black)) wall_matrix[i][j] = 0;
                    else
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, black, white, pl, 1, black, white);
                        wall_matrix[i][j] = 0;  // reset placement
                        if (eval < min_eval)
                        {
                            min_eval = eval;
                            best->move = 'w';
                            best->or = 'r';
                            best->x = black->i;
                            best->y = black->j;
                        }
                    }
                }
            }
        }
        return best;
    }
}

// initial call -> minimax(currentPosition, depth, -infinity, +infinity, true)
float minimax(char** wall_matrix, int boardsize, char depth, float alpha, float beta, player* maximizing, player* minimizing, char pl, char maximizingPlays, player *black, player *white)
{
    if (depth == 0 || white->i == boardsize-1 || black->i == 0)  // End of search - base case
    {
        float positionEval;
        if(!positionEvaluation(black, white, boardsize, wall_matrix, &positionEval))
        {
            printf("? allocation failure\n\n");
            fflush(stdout);
            return INFINITY;
        }
        printf("%f\n", positionEval);
        return positionEval;
    }

    float eval;
    if (maximizingPlays)
    {
        // check each possible move
        
        // check pawn advancement
        float max_eval = NEG_INFINITY;
        
        if (maximizing->i > 0 && !wallBelow(maximizing->i, maximizing->j, wall_matrix, boardsize))
        {
            --maximizing->i;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, maximizing, minimizing, pl, 0, black, white);
            max_eval = max(max_eval, eval);
            alpha = max(alpha, max_eval);
            ++maximizing->i;  // reset movement
        }
        if (beta <= alpha) return max_eval;

        if (maximizing->j < boardsize - 1 && !wallOnTheRight(maximizing->i, maximizing->j, wall_matrix, boardsize))  // right
        {
            ++maximizing->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, maximizing, minimizing, pl, 0, black, white);
            max_eval = max(max_eval, eval);
            alpha = max(alpha, max_eval);
            --maximizing->j;  // reset movement
        }
        if (beta <= alpha) return max_eval;

        if (maximizing->j > 0 &&!wallOnTheLeft(maximizing->i, maximizing->j, wall_matrix, boardsize))  // left
        {
            --maximizing->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, maximizing, minimizing, pl, 0, black, white);
            max_eval = max(max_eval, eval);
            alpha = max(alpha, max_eval);
            ++maximizing->j;  // reset movement
        }
        if (beta <= alpha) return max_eval;

        if (maximizing->i < boardsize - 1 && !wallAbove(maximizing->i, maximizing->j, wall_matrix, boardsize)) // up
        {
            ++maximizing->i;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, maximizing, minimizing, pl, 0, black, white);
            max_eval = max(max_eval, eval);
            alpha = max(alpha, max_eval);
            --maximizing->i;  // reset movement
        }
        if (beta <= alpha) return max_eval;

        // check wall placement
        for (int i = 1; i < boardsize; i++)
        {
            for (int j = 0; j < boardsize-1; j++)
            {
                if (!thereIsAWall('b', wall_matrix, boardsize, i, j) && is_vertex_valid(i, boardsize) && is_vertex_valid(j, boardsize))
                {
                    wall_matrix[i][j] = 'b';  // place horizontal wall
                    if (there_is_a_path(wall_matrix, boardsize, white, black)) wall_matrix[i][j] = 0;
                    else
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, maximizing, minimizing, pl, 0, black, white);
                        max_eval = max(max_eval, eval);
                        wall_matrix[i][j] = 0;  // reset placement
                        alpha = max(alpha, max_eval);
                        if (beta <= alpha) return max_eval;
                    }
                }
                if (!thereIsAWall('r', wall_matrix, boardsize, i, j) && is_vertex_valid(i, boardsize) && is_vertex_valid(j, boardsize) && i != 0)
                {
                    wall_matrix[i][j] = 'r';  // place vertical wall
                    if (there_is_a_path(wall_matrix, boardsize, white, black)) wall_matrix[i][j] = 0;
                    else
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, maximizing, minimizing, pl, 0, black, white);
                        max_eval = max(max_eval, eval);
                        wall_matrix[i][j] = 0; // reset placement
                        alpha = max(alpha, max_eval);
                        if (beta <= alpha) return max_eval;
                    }
                } 
            }
        }
        return max_eval;
    }
    else  // !maximizingPlayer
    {
        // check each possible move

        // check pawn advancement
        float min_eval = INFINITY;

        if (minimizing->i > 0 && !wallBelow(minimizing->i, minimizing->j, wall_matrix, boardsize))  // down
        {
            --minimizing->i;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, maximizing, minimizing, pl, 1, black, white);
            min_eval = min(min_eval, eval);
            beta = min(beta, min_eval);
            ++minimizing->i;  // reset movement
        }
        if (beta <= alpha) return min_eval;
        
        if (minimizing->j < boardsize - 1 && !wallOnTheRight(minimizing->i, minimizing->j, wall_matrix, boardsize))  // right
        {
            ++minimizing->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, maximizing, minimizing, pl, 1, black, white);
            min_eval = min(min_eval, eval);
            beta = min(beta, min_eval);
            --minimizing->j;  // reset movement
        }
        if (beta <= alpha) return min_eval;

        if (minimizing->j > 0 && !wallOnTheLeft(minimizing->i, minimizing->j, wall_matrix, boardsize))  // left
        {
            --minimizing->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, maximizing, minimizing, pl, 1, black, white);
            min_eval = min(min_eval, eval);
            beta = min(beta, min_eval);
            ++minimizing->j;  // reset movement
        }
        if (beta <= alpha) return min_eval;
        
        if (minimizing->i < boardsize -1 && !wallAbove(minimizing->i, minimizing->j, wall_matrix, boardsize))  // up
        {
            ++minimizing->i;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, maximizing, minimizing, pl, 1, black, white);
            min_eval = min(min_eval, eval);
            beta = min(beta, min_eval);
            --minimizing->i;  // reset movement
        }
        if (beta <= alpha) return min_eval;

        // check wall placement
        for (int i = 1; i < boardsize-1; i++)
        {
            if (beta <= alpha) break;
            for (int j = 0; j < boardsize-1; j++)
            {
                if (!thereIsAWall('b', wall_matrix, boardsize, i, j) && is_vertex_valid(i, boardsize) && is_vertex_valid(j, boardsize))
                {
                    wall_matrix[i][j] = 'b';  // place horizontal wall
                    if (there_is_a_path(wall_matrix, boardsize, white, black)) wall_matrix[i][j] = 0;
                    else
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, maximizing, minimizing, pl, 1, black, white);
                        min_eval = max(min_eval, eval);
                        wall_matrix[i][j] = 0;  // reset placement
                        alpha = max(alpha, min_eval);
                        if (beta <= alpha) return min_eval;
                    }
                }
                if (!thereIsAWall('r', wall_matrix, boardsize, i, j) && is_vertex_valid(i, boardsize) && is_vertex_valid(j, boardsize) && i != 0)
                {
                    wall_matrix[i][j] = 'r';  // place vertical wall 
                    if (there_is_a_path(wall_matrix, boardsize, white, black)) wall_matrix[i][j] = 0;  // reset placement
                    else
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, maximizing, minimizing, pl, 1, black, white);
                        min_eval = min(min_eval, eval);
                        beta = min(beta, min_eval);
                        wall_matrix[i][j] = 0;
                        if (beta <= alpha) return min_eval;
                    }
                }
            }
        }
        return min_eval;
    }
}
