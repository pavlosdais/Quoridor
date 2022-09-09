#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "../../include/typedefs.h"
#include "../../include/bfs.h"
#include "../../include/dfs.h"
#include "../../include/structs.h"
#include "../../include/utilities.h"
#include "../../include/eng_helper.h"

#define WHITE_WINS 999999
#define BLACK_WINS -999999

int positionEvaluation(player black, player white, cint boardsize, char** wall_matrix)
{
    if (black.i == 0)  // black reached his goal row - win
        return BLACK_WINS;
    else if (white.i == boardsize-1)  // white reached his goal row - win
        return WHITE_WINS;

    // calculate the distance white needs to get to the goal row (win)
    uint whiteDistanceFromWinning = bfs(boardsize, wall_matrix, white.i, white.j, boardsize-1);

    // calculate the distance black needs to get to the goal row (win)
    uint blackDistanceFromWinning = bfs(boardsize, wall_matrix, black.i, black.j, 0);

    // calculate the distance white needs to get to the next row
    uint whiteDistanceFromNextRow = bfs(boardsize, wall_matrix, white.i, white.j, white.i+1);

    // calculate the distance black needs to get to the next row
    uint blackDistanceFromNextRow = bfs(boardsize, wall_matrix, black.i, black.j, black.i-1);

    return 10*(blackDistanceFromWinning-whiteDistanceFromWinning) + 6*(blackDistanceFromNextRow - whiteDistanceFromNextRow) + 8*(white.walls - black.walls);
}

char there_is_a_path_black(char** wall_matrix, cint boardsize, player* white, player* black)
{
    if (!dfs(boardsize, wall_matrix, white->i, white->j, boardsize-1, 'w'))
        return false;

    if (!dfs(boardsize, wall_matrix, black->i, black->j, 0, 'b'))
        return false;
    
    // the path is not blocked for neither player
    return true;
}


void placeWall(player* pl, char** wall_matrix, char orientation, cint i, cint j)
{
    pl->walls--;
    wall_matrix[i][j] = orientation;
}

void resetWallPlacement(player* pl, char** wall_matrix, cint i, cint j)
{
    pl->walls++;
    wall_matrix[i][j] = 0;
}

void uwp_white(returningMove* evalMove, cchar orientation, cint i, cint j, int* eval, int* max_eval)
{
    *max_eval = *eval;
    evalMove->move = 'w';
    evalMove->or = orientation;
    evalMove->x = i;
    evalMove->y = j;
}

void uwp_black(returningMove* evalMove, cchar orientation, cint i, cint j, int* eval, int* min_eval)
{
    *min_eval = *eval;
    evalMove->move = 'w';
    evalMove->or = orientation;
    evalMove->x = i;
    evalMove->y = j;
}

void updatePawnMovementWhite(player* white, returningMove* evalMove, int* eval, int* max_eval)
{
    if (*eval > *max_eval)
    {
        *max_eval = *eval;
        evalMove->move = 'm';
        evalMove->x = white->i;
        evalMove->y = white->j;
    }
}

void updatePawnMovementBlack(player* black, returningMove* evalMove, int* eval, int* min_eval)
{
    if (*eval < *min_eval)
    {
        *min_eval = *eval;
        evalMove->move = 'm';
        evalMove->x = black->i;
        evalMove->y = black->j;
    }
}

void check_walls_white(char** wall_matrix, player* white, player* black, cint boardsize, csint depth, cchar pseudo, int start_range, int end_range, cfloat max_time,
int* eval, int* best_eval, const clock_t t, returningMove* evalMove, cfloat scope)
{
    cint range = boardsize/scope, minimax_start_range = MINIMAX_START_RANGE(range), minimax_end_range = MINIMAX_END_RANGE(range);
    
    if (isValidWall(black->i, black->j, boardsize, wall_matrix, 'b'))
    {
        placeWall(white, wall_matrix, 'b', black->i, black->j);
        if (there_is_a_path(wall_matrix, boardsize, white, black))
        {
            *eval = minimax(wall_matrix, boardsize, depth, NEG_INFINITY, INFINITY, white, black, false, pseudo, minimax_start_range, minimax_end_range);
            if (*eval > *best_eval) uwp_white(evalMove, 'b', black->i, black->j, eval, best_eval);
        }
        resetWallPlacement(white, wall_matrix, black->i, black->j);  // reset placing the wall
    }
    if (isValidWall(black->i, black->j, boardsize, wall_matrix, 'r'))
    {
        placeWall(white, wall_matrix, 'r', black->i, black->j);
        if (there_is_a_path(wall_matrix, boardsize, white, black))
        {
            *eval = minimax(wall_matrix, boardsize, depth, NEG_INFINITY, INFINITY, white, black, false, pseudo, minimax_start_range, minimax_end_range);
            if (*eval > *best_eval) uwp_white(evalMove, 'r', black->i, black->j, eval, best_eval);
        }
        resetWallPlacement(white, wall_matrix, black->i, black->j);  // reset placing the wall
    }

    int s = MIN(black->i, start_range)+1, a = boardsize - black->i, e = MIN(a, end_range)+1, end = 8;
    for (small_int j = 1; j <= s; j++, e--)
    {
        if (e == 0) end = PRIMARY;
        for (small_int i = 0; i < end; i++)
        {
            int rr = black->i + j*drwhite[i], cc = black->j + j*dcwhite[i];
            if (!(rr >= 0 && rr < boardsize) || !(cc >= 0 && cc < boardsize) || rr == 0 || cc >= boardsize-1)
                    continue;
            
            if (!thereIsAWall('b', wall_matrix, boardsize, rr, cc))
            {
                placeWall(white, wall_matrix, 'b', rr, cc);
                if (there_is_a_path(wall_matrix, boardsize, white, black))
                {
                    *eval = minimax(wall_matrix, boardsize, depth, NEG_INFINITY, INFINITY, white, black, false, pseudo, minimax_start_range, minimax_end_range);
                    if (*eval > *best_eval) uwp_white(evalMove, 'b', rr, cc, eval, best_eval);
                }
                resetWallPlacement(white, wall_matrix, rr, cc);  // reset placing the wall
            }
            if (((double)(clock() - t))/CLOCKS_PER_SEC > max_time) return;  // ran out of time

            // place vertical wall
            if (!thereIsAWall('r', wall_matrix, boardsize, rr, cc))
            {
                placeWall(white, wall_matrix, 'r', rr, cc);
                if (there_is_a_path(wall_matrix, boardsize, white, black))
                {
                    *eval = minimax(wall_matrix, boardsize, depth, NEG_INFINITY, INFINITY, white, black, false, pseudo, minimax_start_range, minimax_end_range);
                    if (*eval > *best_eval) uwp_white(evalMove, 'r', rr, cc, eval, best_eval);
                }
                resetWallPlacement(white, wall_matrix, rr, cc);  // reset placing the wall
                if ((((double)clock() - t)/CLOCKS_PER_SEC) > max_time) return;  // ran out of time
            }
        }
    }
}

void check_walls_black(char** wall_matrix, player* white, player* black, cint boardsize, csint depth, cchar pseudo, int start_range, int end_range, cfloat max_time,
int* eval, int* best_eval, const clock_t t, returningMove* evalMove, cfloat scope)
{
    cint range = boardsize/scope, minimax_start_range = MINIMAX_START_RANGE(range), minimax_end_range = MINIMAX_END_RANGE(range);
    
    if (isValidWall(white->i, white->j, boardsize, wall_matrix, 'b'))
    {
        placeWall(black, wall_matrix, 'b', white->i, white->j);
        if (there_is_a_path_black(wall_matrix, boardsize, white, black))
        {
            *eval = minimax(wall_matrix, boardsize, depth, NEG_INFINITY, INFINITY, white, black, true, pseudo, minimax_start_range, minimax_end_range);
            if (*eval < *best_eval) uwp_black(evalMove, 'b', white->i, white->j, eval, best_eval);
        }
        resetWallPlacement(black, wall_matrix, white->i, white->j);  // reset placing the wall
    }
    if (isValidWall(white->i, white->j, boardsize, wall_matrix, 'r'))
    {
        placeWall(black, wall_matrix, 'r', white->i, white->j);
        if (there_is_a_path_black(wall_matrix, boardsize, white, black))
        {
            *eval = minimax(wall_matrix, boardsize, depth, NEG_INFINITY, INFINITY, white, black, true, pseudo, minimax_start_range, minimax_end_range);
            if (*eval < *best_eval) uwp_black(evalMove, 'r', white->i, white->j, eval, best_eval);
        }
        resetWallPlacement(black, wall_matrix, white->i, white->j);  // reset placing the wall
    }
    
    int a = boardsize-white->i,s = MIN(a, start_range)+1, e = end_range+1, end = 8;
    if (white->i + end_range >= boardsize) e = boardsize - white->i+1;

    for (small_int j = 1; j <= s; j++, e--)
    {
        if (e == 0) end = PRIMARY;
        for (small_int i = 0; i < end; i++)
        {
            int rr = white->i + j*drblack[i], cc = white->j + j*dcblack[i];
            if (!(rr >= 0 && rr < boardsize) || !(cc >= 0 && cc < boardsize) || rr == 0 || cc >= boardsize-1)
                    continue;
            
            if (!thereIsAWall('b', wall_matrix, boardsize, rr, cc))
            {
                placeWall(black, wall_matrix, 'b', rr, cc);
                if (there_is_a_path_black(wall_matrix, boardsize, white, black))
                {
                    *eval = minimax(wall_matrix, boardsize, depth, NEG_INFINITY, INFINITY, white, black, true, pseudo, minimax_start_range, minimax_end_range);
                    if (*eval < *best_eval) uwp_black(evalMove, 'b', rr, cc, eval, best_eval);
                }
                resetWallPlacement(black, wall_matrix, rr, cc);  // reset placing the wall
            }
            if (((double)(clock() - t))/CLOCKS_PER_SEC > max_time) return;

            // place vertical wall
            if (!thereIsAWall('r', wall_matrix, boardsize, rr, cc))
            {
                placeWall(black, wall_matrix, 'r', rr, cc);
                if (there_is_a_path_black(wall_matrix, boardsize, white, black))
                {
                    *eval = minimax(wall_matrix, boardsize, depth, NEG_INFINITY, INFINITY, white, black, true, pseudo, minimax_start_range, minimax_end_range);
                    if (*eval < *best_eval) uwp_black(evalMove, 'r', rr, cc, eval, best_eval);
                }
                resetWallPlacement(black, wall_matrix, rr, cc);  // reset placing the wall
                if ((((double)clock() - t)/CLOCKS_PER_SEC) > max_time) return;  // ran out of time
            }
        }
    }
    printf("Nani?!\n");
}