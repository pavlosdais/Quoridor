#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "../../include/dfs.h"
#include "../../include/bfs.h"
#include "../../include/typedefs.h"
#include "../../include/structs.h"
#include "../../include/utilities.h"
#include "../../include/engine_utilities.h"

char there_is_a_path_black(char** wall_matrix, cint boardsize, player* white, player* black)
{
    if (!dfs(boardsize, wall_matrix, white->i, white->j, boardsize-1, 'w'))
        return false;

    if (!dfs(boardsize, wall_matrix, black->i, black->j, 0, 'b'))
        return false;
    
    // the path is not blocked for neither player
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

int positionEvaluation(player black, player white, const int boardsize, char** wall_matrix)
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

    return 10*(blackDistanceFromWinning-whiteDistanceFromWinning) + 6*(blackDistanceFromNextRow - whiteDistanceFromNextRow) + 7*(white.walls - black.walls);
}

void check_walls(char** wall_matrix, player* white, player* black, player* curr_player, cint boardsize, csint depth, cchar pseudo, int start, cint end, cfloat max_time,
int* eval, int* best_eval, const clock_t t, returningMove* evalMove, QuitFunc quit, ChangeFunc change, cchar cond, updateWallWallPlacement uwp, char col)
{
    check_path path_exists;
    if (col == 'w') path_exists = there_is_a_path;
    else path_exists = there_is_a_path_black;

    const char wall_types[] = {'b', 'r'};
    cint range = boardsize/SCOPE;
    while (true)
    {
        for (int j = 0; j < boardsize-1; j++)
        {
            for (small_int k = 0; k < 2; k++)
            {
                if (!thereIsAWall(wall_types[k], wall_matrix, boardsize, start, j))
                {
                    placeWall(curr_player, wall_matrix, wall_types[k], start, j);
                    if (path_exists(wall_matrix, boardsize, white, black))
                    {
                        *eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, cond, pseudo, range);
                        uwp(evalMove, wall_types[k], start, j, eval, best_eval);
                    }
                    resetWallPlacement(curr_player, wall_matrix, start, j);  // reset placing the wall
                }
                if (((double)(clock() - t))/CLOCKS_PER_SEC > max_time) return;  // ran out of time
            }
        }
        if ((((double)clock() - t)/CLOCKS_PER_SEC) > max_time) return;  // ran out of time
        change(&start);
        if (!quit(start, end)) return;
    }
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

void updatePawnMovementWhite(player* white, returningMove* evalMove, int* eval, int* max_eval)
{
    *max_eval = *eval;
    evalMove->move = 'm';
    evalMove->x = white->i;
    evalMove->y = white->j;
}

void updatePawnMovementBlack(player* black, returningMove* evalMove, int* eval, int* min_eval)
{
    *min_eval = *eval;
    evalMove->move = 'm';
    evalMove->x = black->i;
    evalMove->y = black->j;
}

void uwp_white(returningMove* evalMove, cchar orientation, cint i, cint j, int* eval, int* max_eval)
{
    if (*eval > *max_eval)
    {
        *max_eval = *eval;
        evalMove->move = 'w';
        evalMove->or = orientation;
        evalMove->x = i;
        evalMove->y = j;
    }
}

void uwp_black(returningMove* evalMove, cchar orientation, cint i, cint j, int* eval, int* min_eval)
{
    if (*eval < *min_eval)
    {
        *min_eval = *eval;
        evalMove->move = 'w';
        evalMove->or = orientation;
        evalMove->x = i;
        evalMove->y = j;
    }
}

int compare_white(int a, int b) { return a < b; }
int change_white(int* a) { (*a)++; }
int compare_black(int a, int b) { return a > b; }
int change_black(int* a) { (*a)--; }
