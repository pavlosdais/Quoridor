#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include "helper_commands.h"
#include "bfs.h"

#define INFINITY 9999999999
#define NEG_INFINITY -9999999999
#define true 1
#define false 0

// Function Prototype
float minimax(char** wall_matrix, int boardsize, char depth, float alpha, float beta, player** white, player** black, char Maximizing);

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
    char move;  // w for wall at (x,y), m for move at (x,y)
    char or;  // orientation of the wall
}
returningMove;

returningMove *bestMove(char** wall_matrix, int boardsize, char pl, player** black, player** white, char depth)
{
    float eval;
    returningMove *best;

    if (pl == 'w')  // white plays
    {
        // check each possible move

        // check pawn advancement
        float max_eval = NEG_INFINITY;

        if ((*white)->i > 0 && !wallBelow((*white)->i, (*white)->j, wall_matrix, boardsize))
        {
            --(*white)->i;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false);
            if (eval > max_eval)
            {
                max_eval = eval;
                best->move = 'm';
                best->x = (*white)->i;
                best->y = (*white)->j;
            }
            ++(*white)->i;  // reset movement
        }

        if ((*white)->j < boardsize - 1 && !wallOnTheRight((*white)->i, (*white)->j, wall_matrix, boardsize))
        {
            ++(*white)->j;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false);
            if (eval > max_eval)
            {
                max_eval = eval;
                best->move = 'm';
                best->x = (*white)->i;
                best->y = (*white)->j;
            }
            --(*white)->j;  // reset movement
        }

        if ((*white)->j > 0 &&!wallOnTheLeft((*white)->i, (*white)->j, wall_matrix, boardsize))  // left
        {
            --(*white)->j;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false);
            if (eval > max_eval)
            {
                max_eval = eval;
                best->move = 'm';
                best->x = (*white)->i;
                best->y = (*white)->j;
            }
            ++(*white)->j;  // reset movement
        }
        if ((*white)->i < boardsize - 1 && !wallAbove((*white)->i, (*white)->j, wall_matrix, boardsize))
        {
            ++(*white)->i;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false);
            if (eval > max_eval)
            {
                max_eval = eval;
                best->move = 'm';
                best->x = (*white)->i;
                best->y = (*white)->j;
            }
            --(*white)->i;  // reset movement
        }

        // check wall placement
        for (int i = 1; i < boardsize; i++)
        {
            if ((*white)->walls == 0) break;
            for (int j = 0; j < boardsize-1; j++)
            {
                if ((*white)->walls == 0) break;
                if (!thereIsAWall('b', wall_matrix, boardsize, i, j) && is_vertex_valid(i, boardsize) && is_vertex_valid(j, boardsize))
                {
                    wall_matrix[i][j] = 'b';  // place horizontal wall
                    (*white)->walls--;
                    if (there_is_a_path(wall_matrix, boardsize, *white, *black))
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false);
                        if (eval > max_eval)
                        {
                            max_eval = eval;
                            best->move = 'w';
                            best->or = 'b';
                            best->x = i;
                            best->y = j;
                        }
                    }
                    // reset placement
                    (*white)->walls++;
                    wall_matrix[i][j] = 0;
                }
                
                if (!thereIsAWall('r', wall_matrix, boardsize, i, j) && is_vertex_valid(i, boardsize) && is_vertex_valid(j, boardsize))
                {
                    wall_matrix[i][j] = 'r';  // place vertical wall
                    (*white)->walls--;
                    if (there_is_a_path(wall_matrix, boardsize, *white, *black))
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false);
                        if (eval > max_eval)
                        {
                            max_eval = eval;
                            best->move = 'w';
                            best->or = 'r';
                            best->x = i;
                            best->y = j;
                        }
                    }
                    // reset placement
                    (*white)->walls++;
                    wall_matrix[i][j] = 0;
                } 
            }
        }
        if (best->move == 'm')  // Ai moved the pawn
        {
            (*white)->i = best->x;
            (*white)->j = best->y;
        }
        else (*white)->walls--;
    }
    else  // black plays
    {
        float min_eval = INFINITY;
        // check each possible move

        // check pawn advancement
        if ((*black)->i < boardsize -1 && !wallAbove((*black)->i, (*black)->j, wall_matrix, boardsize))  // up
        {
            ++(*black)->i;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true);
            if (eval < min_eval)
            {
                min_eval = eval;
                best->move = 'm';
                best->x = (*black)->i;
                best->y = (*black)->j;
            }
            --(*black)->i;  // reset movement
        }
        
        if ((*black)->j < boardsize - 1 && !wallOnTheRight((*black)->i, (*black)->j, wall_matrix, boardsize))  // right
        {
            ++(*black)->j;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true);
            if (eval < min_eval)
            {
                min_eval = eval;
                best->move = 'm';
                best->x = (*black)->i;
                best->y = (*black)->j;
            }
            --(*black)->j;  // reset movement
        }

        if ((*black)->j > 0 && !wallOnTheLeft((*black)->i, (*black)->j, wall_matrix, boardsize))  // left
        {
            --(*black)->j;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true);
            if (eval < min_eval)
            {
                min_eval = eval;
                best->move = 'm';
                best->x = (*black)->i;
                best->y = (*black)->j;
            }
            ++(*black)->j;  // reset movement
        }
        
        if ((*black)->i > 0 && !wallBelow((*black)->i, (*black)->j, wall_matrix, boardsize))  // down
        {
            --(*black)->i;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true);
            if (eval < min_eval)
            {
                min_eval = eval;
                best->move = 'm';
                best->x = (*black)->i;
                best->y = (*black)->j;
            }
            ++(*black)->i;  // reset movement
        }

        // check wall placement
        for (int i = 1; i < boardsize-1; i++)
        {
            if ((*white)->walls == 0) break;
            for (int j = 0; j < boardsize-1; j++)
            {
                if ((*white)->walls == 0) break;
                if (!thereIsAWall('b', wall_matrix, boardsize, i, j) && is_vertex_valid(i, boardsize) && is_vertex_valid(j, boardsize))
                {
                    wall_matrix[i][j] = 'b';  // place horizontal wall
                    (*black)->walls--;
                    if (there_is_a_path(wall_matrix, boardsize, *white, *black))
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true);
                        if (eval < min_eval)
                        {
                            min_eval = eval;
                            best->move = 'w';
                            best->or = 'b';
                            best->x = i;
                            best->y = j;
                        }
                    }
                    // reset placement
                    (*black)->walls++;
                    wall_matrix[i][j] = 0;
                }
                if (!thereIsAWall('r', wall_matrix, boardsize, i, j) && is_vertex_valid(i, boardsize) && is_vertex_valid(j, boardsize))
                {
                    wall_matrix[i][j] = 'r';  // place vertical wall 
                    (*black)->walls--;
                    if (there_is_a_path(wall_matrix, boardsize, *white, *black))
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true);
                        if (eval < min_eval)
                        {
                            min_eval = eval;
                            best->move = 'w';
                            best->or = 'r';
                            best->x = i;
                            best->y = j;
                        }
                    }
                    // reset placement
                    wall_matrix[i][j] = 0;
                    (*black)->walls++;
                }
            }
        }
        if (best->move == 'm')  // Ai moved the pawn
        {
            (*black)->i = best->x;
            (*black)->j = best->y;
        }
        else (*black)->walls--;

    }
    if (best->move == 'w') wall_matrix[best->x][best->y] = best->or;  // Ai placed a wall
    return best;
}

/*
sources: https://en.wikipedia.org/wiki/Minimax
         https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning
*/

float minimax(char** wall_matrix, int boardsize, char depth, float alpha, float beta, player** white, player** black, char Maximizing)
{
    if (depth == 0 || (*white)->i == boardsize-1 || (*black)->i == 0)  // Base case
    {
        // Evaluate the position
        float positionEval;
        if(!positionEvaluation(*black, *white, boardsize, wall_matrix, &positionEval))
        {
            printf("? allocation failure\n\n");
            fflush(stdout);
            return INFINITY;
        }
        return positionEval;
    }

    float eval;
    if (Maximizing)  // White
    {
        // check each possible move
        
        // check pawn advancement
        float max_eval = NEG_INFINITY;
        
        if ((*white)->i > 0 && !wallBelow((*white)->i, (*white)->j, wall_matrix, boardsize))  // down
        {
            --(*white)->i;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false);
            max_eval = max(max_eval, eval);
            alpha = max(alpha, max_eval);
            ++(*white)->i;  // reset movement
            if (beta <= alpha) return max_eval;
        }

        if ((*white)->j < boardsize - 1 && !wallOnTheRight((*white)->i, (*white)->j, wall_matrix, boardsize))  // right
        {
            ++(*white)->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false);
            max_eval = max(max_eval, eval);
            alpha = max(alpha, eval);
            --(*white)->j;  // reset movement
            if (beta <= alpha) return max_eval;
        }
        
        if ((*white)->j > 0 &&!wallOnTheLeft((*white)->i, (*white)->j, wall_matrix, boardsize))  // left
        {
            --(*white)->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false);
            max_eval = max(max_eval, eval);
            alpha = max(alpha, eval);
            ++(*white)->j;  // reset movement
            if (beta <= alpha) return max_eval;
        }

        if ((*white)->i < boardsize - 1 && !wallAbove((*white)->i, (*white)->j, wall_matrix, boardsize)) // up
        {
            ++(*white)->i;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false);
            max_eval = max(max_eval, eval);
            alpha = max(alpha, eval);
            --(*white)->i;  // reset movement
            if (beta <= alpha) return max_eval;
        }

        // check wall placement
        for (int i = 1; i < boardsize; i++)
        {
            if ((*white)->walls == 0) break;
            for (int j = 0; j < boardsize-1; j++)
            {
                if ((*white)->walls == 0) break;
                if (!thereIsAWall('b', wall_matrix, boardsize, i, j) && is_vertex_valid(i, boardsize) && is_vertex_valid(j, boardsize))
                {
                    (*white)->walls--;
                    wall_matrix[i][j] = 'b';  // place horizontal wall
                    if (there_is_a_path(wall_matrix, boardsize, *white, *black))
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false);
                        max_eval = max(max_eval, eval);
                        wall_matrix[i][j] = 0;
                        alpha = max(alpha, eval);
                        if (beta <= alpha) return max_eval;
                    }
                    // reset placement
                    wall_matrix[i][j] = 0;
                    (*white)->walls++;
                }
                if (!thereIsAWall('r', wall_matrix, boardsize, i, j) && is_vertex_valid(i, boardsize) && is_vertex_valid(j, boardsize) && i != 0)
                {
                    (*white)->walls--;
                    wall_matrix[i][j] = 'r';  // place vertical wall
                    if (there_is_a_path(wall_matrix, boardsize, *white, *black))
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false);
                        max_eval = max(max_eval, eval);
                        wall_matrix[i][j] = 0;
                        alpha = max(alpha, eval);
                        if (beta <= alpha) return max_eval;
                    }
                    // reset placement
                    wall_matrix[i][j] = 0;
                    (*white)->walls++;
                } 
            }
        }
        return max_eval;
    }
    else  // !maximizingPlayer - Black
    {
        // check each possible move

        // check pawn advancement
        float min_eval = INFINITY;
        
        if ((*black)->i < boardsize -1 && !wallAbove((*black)->i, (*black)->j, wall_matrix, boardsize))  // up
        {
            ++(*black)->i;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true);
            min_eval = min(min_eval, eval);
            beta = min(beta, eval);
            --(*black)->i;  // reset movement
            if (beta <= alpha) return min_eval;
        }

        if ((*black)->i > 0 && !wallBelow((*black)->i, (*black)->j, wall_matrix, boardsize))  // down
        {
            --(*black)->i;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, 1);
            min_eval = min(min_eval, eval);
            beta = min(beta, min_eval);
            ++(*black)->i;  // reset movement
            if (beta <= alpha) return min_eval;
        }

        if ((*black)->j < boardsize-1 && !wallOnTheRight((*black)->i, (*black)->j, wall_matrix, boardsize))  // right
        {
            ++(*black)->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true);
            min_eval = min(min_eval, eval);
            beta = min(beta, eval);
            --(*black)->j;  // reset movement
            if (beta <= alpha) return min_eval;
        }
        
        if ((*black)->j > 0 && !wallOnTheLeft((*black)->i, (*black)->j, wall_matrix, boardsize))  // left
        {
            --(*black)->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true);
            min_eval = min(min_eval, eval);
            beta = min(beta, eval);
            ++(*black)->j;  // reset movement
            if (beta <= alpha) return min_eval;
        }

        // check wall placement
        for (int i = 1; i < boardsize-1; i++)
        {
            if (beta <= alpha) break;
            if ((*black)->walls == 0) break;
            for (int j = 0; j < boardsize-1; j++)
            {
                if (!thereIsAWall('b', wall_matrix, boardsize, i, j) && is_vertex_valid(i, boardsize) && is_vertex_valid(j, boardsize))
                {
                    if ((*black)->walls == 0) break;
                    (*black)->walls--;
                    wall_matrix[i][j] = 'b';  // place horizontal wall
                    if (there_is_a_path(wall_matrix, boardsize, *white, *black)) 
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true);
                        min_eval = max(min_eval, eval);
                        wall_matrix[i][j] = 0;
                        beta = min(beta, eval);
                        if (beta <= alpha) return min_eval;
                    }
                    // reset placement
                    wall_matrix[i][j] = 0;
                    (*black)->walls++;
                }
                if (!thereIsAWall('r', wall_matrix, boardsize, i, j) && is_vertex_valid(i, boardsize) && is_vertex_valid(j, boardsize))
                {
                    (*black)->walls--;
                    wall_matrix[i][j] = 'r';  // place vertical wall 
                    if (there_is_a_path(wall_matrix, boardsize, *white, *black))
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true);
                        min_eval = min(min_eval, eval);
                        beta = min(beta, eval);
                        wall_matrix[i][j] = 0;
                        if (beta <= alpha) return min_eval;
                    }
                    // reset placement
                    wall_matrix[i][j] = 0; 
                    (*black)->walls++;
                }
            }
        }
        return min_eval;
    }
}
