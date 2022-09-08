#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "../include/typedefs.h"
#include "../include/structs.h"
#include "../include/utilities.h"

typedef char (*check_path)(char** wall_matrix, cint boardsize, player* white, player* black);

// Helper function prototypes
static int minimax(char** wall_matrix, cint boardsize, csint depth, int alpha, int beta, player* white, player* black, char Maximizing, cchar pseudo, cint start_range, cint end_range);
static void uwp_white(returningMove* evalMove, cchar orientation, cint i, cint j, int* eval, int* max_eval);
static void uwp_black(returningMove* evalMove, cchar orientation, cint i, cint j, int* eval, int* min_eval);
static void updatePawnMovementWhite(player* white, returningMove* evalMove, int* eval, int* max_eval);
static void updatePawnMovementBlack(player* black, returningMove* evalMove, int* eval, int* min_eval);
static void placeWall(player* pl, char** wall_matrix, char orientation, cint i, cint j);
static void resetWallPlacement(player* pl, char** wall_matrix, cint i, cint j);
static void updateEvalWhite(int* max_eval, int* eval, int* alpha);
static void updateEvalBlack(int* max_eval, int* eval, int* beta);
static void check_walls(char** wall_matrix, player* white, player* black, player* curr_player, cint boardsize, csint depth, cchar pseudo, int start, cint end, cfloat max_time,
  int* eval, int* best_eval, const clock_t t, returningMove* evalMove, QuitFunc quit, ChangeFunc change, cchar cond, updateWallWallPlacement uwp, char col, cfloat scope);

// Functions needed for wall placement
int compare_white(int a, int b) { return a < b; }
int change_white(int* a) { (*a)++; }
int compare_black(int a, int b) { return a > b; }
int change_black(int* a) { (*a)--; }

/* For each potential move the corresponding player has, search the evaluated best responses for both players up to a certain
depth, or moves ahead, and return the move which is believed to be the most advantageous for the player who's playing assuming
optimal play for both sides. */

#define SEARCH_START_RANGE(range) (range * 1.5)
#define SEARCH_END_RANGE(range) (range / 1.5)
#define MINIMAX_START_RANGE(range) (range * 1.5)
#define MINIMAX_END_RANGE(range) (range / 1.5)

returningMove bestMove(char** wall_matrix, cint boardsize, cchar pl, player* black, player* white, csint depth, char pseudo, cfloat max_time, cfloat scope)
{
    clock_t t = clock();
    int eval, start, end;
    cint range = boardsize/scope, start_range = SEARCH_START_RANGE(range), end_range = SEARCH_END_RANGE(range); 
    returningMove evalMove;
    
    if (pl == 'w')  // white plays
    {
        // check each possible move
        int max_eval = NEG_INFINITY;

        // -check pawn movement-

        // check diagonal moves
        if (white->i+1 == black->i && white->j == black->j && !wallAbove(white->i, white->j, wall_matrix, boardsize)) // black above white
        {
            if (black->i < boardsize - 1 && !wallAbove(black->i, black->j, wall_matrix, boardsize))
            {
                (white->i)+=2;
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
                updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                (white->i)-=2;  // reset movement
            }
            else
            {
                if (white->j < boardsize-1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    ++white->i;
                    ++white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
                    updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    --white->i;  // reset movement
                    --white->j;
                }

                if (white->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    ++white->i;
                    --white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
                    updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    --white->i;  // reset movement
                    ++white->j;
                }
            }
        }
        else if (white->i == black->i && white->j+1 == black->j && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))  // black on the right of white
        {
            if (black->j < boardsize - 1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))
            {
                (white->j)+=2;
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
                updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                (white->j)-=2;  // reset movement
            }
            else
            {
                if (white->i < boardsize-1 && !wallAbove(black->i, black->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    ++white->i;
                    ++white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
                    updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    --white->i;  // reset movement
                    --white->j;
                }

                if (white->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // diagonally down - right
                {
                    --white->i;
                    ++white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
                    updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    ++white->i;  // reset movement
                    --white->j;
                }
            }
        }
        else if (white->i == black->i && white->j-1 == black->j && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // black on the left of white
        {
            if (black->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))
            {
                (white->j)-=2;
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
                updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                (white->j)+=2;  // reset movement
            }
            else
            {
                if (white->i < boardsize-1 && !wallAbove(black->i, black->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    ++white->i;
                    --white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
                    updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    --white->i;  // reset movement
                    ++white->j;
                }

                if (white->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    --white->i;
                    --white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
                    updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    ++white->i;  // reset movement
                    ++white->j;
                }
            }
        }
        else if (white->i-1 == black->i && white->j == black->j && !wallBelow(white->i, white->j, wall_matrix, boardsize))  // black below white
        {
            if (black->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize))
            {
                (white->i)-=2;
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
                updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                (white->i)+=2;  // reset movement
            }
            else
            {
                if (white->j < boardsize-1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))  // diagonally  down - right
                {
                    --white->i;
                    ++white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
                    updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    ++white->i;  // reset movement
                    --white->j;
                }

                if (white->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    --white->i;
                    --white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
                    updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    ++white->i;  // reset movement
                    ++white->j;
                }
            }
        }     
        // check normal moves
        if (white->i < boardsize-1 && !wallAbove(white->i, white->j, wall_matrix, boardsize) && !(white->i + 1 == black->i && white->j == black->j))  // up
        {
            ++white->i;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
            updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
            --white->i;  // reset movement
        }
        if (white->j < boardsize - 1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize) && !(white->i == black->i && white->j + 1 == black->j))  // right
        {
            ++white->j;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
            updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
            --white->j;  // reset movement
        }
        if (white->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize) && !(white->i == black->i && white->j - 1 == black->j))  // left
        {
            --white->j;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
            updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
            ++white->j;  // reset movement
        }
        if (white->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize) && !(white->i - 1 == black->i && white->j == black->j))  // down
        {
            --white->i;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
            updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
            ++white->i;  // reset movement
        }

        // -check wall placement-
        if (white->walls == 0) return evalMove;  // white does not have sufficient walls to place

        start = black->i - start_range;
        if (start < 1) start = 1;

        end = black->i + end_range;
        if (end > boardsize) end = boardsize;

        check_walls(wall_matrix, white, black, white, boardsize, depth, pseudo, start, end, max_time,
        &eval, &max_eval, t, &evalMove, compare_white, change_white, false, uwp_white, pl, scope);
    }
    else  // black plays
    {
        // check each possible move
        int min_eval = INFINITY;

        // -check pawn movement-

        // check diagonal moves
        if (black->i-1 == white->i && black->j == white->j && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // white below black
        {
            if (white->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize))
            {
                (black->i)-=2;
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);
                updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                (black->i)+=2;  // reset movement
            }
            else
            {
                if (black->j < boardsize-1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))  // diagonally  down - right
                {
                    --black->i;
                    ++black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);

                    updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    ++black->i;  // reset movement
                    --black->j;
                }

                if (black->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    --black->i;
                    --black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);

                    updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    ++black->i;  // reset movement
                    ++black->j;
                }
            }
        }
        else if (black->i == white->i && black->j-1 == white->j && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // white on the left of black
        {
            if (white->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))
            {
                (black->j)-=2;
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);
                updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                (black->j)+=2;  // reset movement
            }
            else
            {
                if (black->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    --black->i;
                    --black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);
                    updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    ++black->i;  // reset movement
                    ++black->j; 
                }
                
                if (black->i < boardsize - 1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    ++black->i;
                    --black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);
                    updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    --black->i;  // reset movement
                    ++black->j;
                }
            }
        }
        else if (black->i == white->i && black->j+1 == white->j && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))  // white on the right of black
        {
            if (white->j < boardsize - 1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))
            {
                (black->j)+=2;
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);
                updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                (black->j)-=2;  // reset movement
            }
            else
            {
                if (black->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize))  // diagonally down - right
                {
                    --black->i;
                    ++black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);
                    updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    ++black->i;  // reset movement
                    --black->j; 
                }

                if (black->i < boardsize - 1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    ++black->i;
                    ++black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);
                    updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    --black->i;  // reset movement
                    --black->j;
                }
            }
        }
        else if (black->i+1 == white->i && black->j == white->j && !wallAbove(black->i, black->j, wall_matrix, boardsize))  // white above black
        {
            if (white->i < boardsize - 1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))
            {
                (black->i)+=2;
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);
                updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                (black->i)-=2;  // reset movement
            }
            else
            {
                if (black->j < boardsize-1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    ++black->i;
                    ++black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);

                    updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    --black->i;  // reset movement
                    --black->j; 
                }

                if (black->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    ++black->i;
                    --black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);

                    updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    --black->i;  // reset movement
                    ++black->j;
                }
            }
        }
        // check normal moves
        if (black->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize) && !(black->i - 1 == white->i && black->j == white->j))  // down
        {
            --black->i;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);
            updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
            ++black->i;  // reset movement
        }
        if (black->j < boardsize-1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize) && !(black->i == white->i && black->j + 1 == white->j))  // right
        {
            ++black->j;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);
            updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
            --black->j;  // reset movement
        }
        if (black->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize) && !(black->i == white->i && black->j - 1 == white->j))  // left
        {
            --black->j;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);
            updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
            ++black->j;  // reset movement
        }      
        if (black->i < boardsize - 1 && !wallAbove(black->i, black->j, wall_matrix, boardsize) && !(black->i + 1 == white->i && black->j == white->j))  // up
        {
            ++black->i;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);
            updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
            --black->i;  // reset movement
        }

        // -check wall placement-
        if (black->walls == 0) return evalMove;  // black does not have sufficient walls to place

        start = white->i + start_range;
        if (start > boardsize-1) start = boardsize-1;

        end = white->i - end_range;
        if (end < 1) end = 1;
        
        check_walls(wall_matrix, white, black, black, boardsize, depth, pseudo, start, end, max_time, 
        &eval, &min_eval, t, &evalMove, compare_black, change_black, true, uwp_black, pl, scope);
    }

    return evalMove;
}

/* sources: https://en.wikipedia.org/wiki/Minimax
            https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning */

static int minimax(char** wall_matrix, cint boardsize, csint depth, int alpha, int beta, player* white, player* black, char Maximizing, cchar pseudo, cint start_range, cint end_range)
{
    if (depth == 0 || white->i == boardsize-1 || black->i == 0)  // Base case
        return positionEvaluation(*black, *white, boardsize, wall_matrix);

    int eval, start, end;
    small_int path_exists;
    
    if (Maximizing)  // Maximizing - White
    {
        // check each possible move
    
        // -check pawn movement-
        int max_eval = NEG_INFINITY;

        // check diagonal moves
        if (white->i+1 == black->i && white->j == black->j && !wallAbove(white->i, white->j, wall_matrix, boardsize))  // black above white
        {
            if (black->i < boardsize-1 && !wallAbove(black->i, black->j, wall_matrix, boardsize))
            {
                (white->i)+=2;
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                updateEvalWhite(&max_eval, &eval, &alpha);
                (white->i)-=2;  // reset movement
                if (beta <= alpha) return max_eval;
            }
            else
            {
                if (white->j < boardsize - 1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    ++white->i;
                    ++white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                    updateEvalWhite(&max_eval, &eval, &alpha);
                    --white->i;  // reset movement
                    --white->j;
                    if (beta <= alpha) return max_eval; 
                }

                if (white->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    ++white->i;
                    --white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                    updateEvalWhite(&max_eval, &eval, &alpha);
                    --white->i;  // reset movement
                    ++white->j;
                    if (beta <= alpha) return max_eval; 
                }
            }
        }
        else if (white->i == black->i && white->j+1 == black->j && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))  // black on the right of white
        {
            if (black->j < boardsize-1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))
            {
                (white->j)+=2;
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                updateEvalWhite(&max_eval, &eval, &alpha);
                (white->j)-=2;  // reset movement
                if (beta <= alpha) return max_eval;
            }
            else
            {
                if (white->i < boardsize-1 && !wallAbove(black->i, black->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    ++white->i;
                    ++white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                    updateEvalWhite(&max_eval, &eval, &alpha);
                    --white->i;  // reset movement
                    --white->j;
                    if (beta <= alpha) return max_eval; 
                }

                if (white->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // diagonally down - right
                {
                    --white->i;
                    ++white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                    updateEvalWhite(&max_eval, &eval, &alpha);
                    ++white->i;  // reset movement
                    --white->j;
                    if (beta <= alpha) return max_eval; 
                }
            }
        }
        else if (white->i == black->i && white->j - 1 == black->j && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // black on the left of white
        {
            if (black->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))
            {
                (white->j)-=2;
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                updateEvalWhite(&max_eval, &eval, &alpha);
                (white->j)+=2;  // reset movement
                if (beta <= alpha) return max_eval;
            }
            else
            {
                if (white->i < boardsize-1 && !wallAbove(black->i, black->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    ++white->i;
                    --white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                    updateEvalWhite(&max_eval, &eval, &alpha);
                    --white->i;  // reset movement
                    ++white->j;
                    if (beta <= alpha) return max_eval; 
                }

                if (white->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    --white->i;
                    --white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                    updateEvalWhite(&max_eval, &eval, &alpha);
                    ++white->i;  // reset movement
                    ++white->j;
                    if (beta <= alpha) return max_eval; 
                }
            }
        }
        else if (white->i-1 == black->i && white->j == black->j && !wallBelow(white->i, white->j, wall_matrix, boardsize))  // black below white
        {
            if (black->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize))
            {
                (white->i)-=2;
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                updateEvalWhite(&max_eval, &eval, &alpha);
                (white->i)+=2;  // reset movement
                if (beta <= alpha) return max_eval;
            }
            else
            {
                if (white->j < boardsize-1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))  // diagonally  down - right
                {
                    --white->i;
                    ++white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                    updateEvalWhite(&max_eval, &eval, &alpha);
                    ++white->i;  // reset movement
                    --white->j;
                    if (beta <= alpha) return max_eval; 
                }

                if (white->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    --white->i;
                    --white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                    updateEvalWhite(&max_eval, &eval, &alpha);
                    ++white->i;  // reset movement
                    ++white->j;
                    if (beta <= alpha) return max_eval; 
                }
            }
        }
        // check normal moves
        if (white->i < boardsize-1 && !wallAbove(white->i, white->j, wall_matrix, boardsize) && !(white->i + 1 == black->i && white->j == black->j))  // up
        {
            ++white->i;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
            updateEvalWhite(&max_eval, &eval, &alpha);
            --white->i;  // reset movement
            if (beta <= alpha) return max_eval;
        }
        if (white->j < boardsize - 1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize) && !(white->i == black->i && white->j + 1 == black->j))  // right
        {
            ++white->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
            updateEvalWhite(&max_eval, &eval, &alpha);
            --white->j;  // reset movement
            if (beta <= alpha) return max_eval;
        }    
        if (white->j > 0 &&!wallOnTheLeft(white->i, white->j, wall_matrix, boardsize) && !(white->i == black->i && white->j - 1 == black->j))  // left
        {
            --white->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
            updateEvalWhite(&max_eval, &eval, &alpha);
            ++white->j;  // reset movement
            if (beta <= alpha) return max_eval;
        } 
        if (white->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize) && !(white->i - 1 == black->i && white->j == black->j))  // down
        {
            --white->i;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
            updateEvalWhite(&max_eval, &eval, &alpha);
            ++white->i;  // reset movement
            if (beta <= alpha) return max_eval;
        }
        
        // -check wall placement-
        if (white->walls == 0) return max_eval;  // white does not have sufficient walls to place

        if (depth == 1 && pseudo == true)
        {
            // last search of a pseudodepth check, only check wall placement near black's pawn
            start = 1;
            if (black->i > 1) start = black->i-1;

            end = boardsize;
            if (black->i < boardsize-1) end = black->i+1;
        }
        else 
        {
            end = black->i + end_range;
            if (end > boardsize) end = boardsize;

            start = black->i - start_range;
            if (start < 1) start = 1;
        }

        for (int i = start; i < end; i++)
        {
            for (int j = 0; j < boardsize-1; j++)
            {
                // place horizontal wall
                if (!thereIsAWall('b', wall_matrix, boardsize, i, j))
                {
                    placeWall(white, wall_matrix, 'b', i, j);
                    path_exists = false;
                    if (there_is_a_path(wall_matrix, boardsize, white, black))
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                        path_exists = 1;
                        updateEvalWhite(&max_eval, &eval, &alpha);
                        resetWallPlacement(white, wall_matrix, i, j);
                        if (beta <= alpha) return max_eval;
                    }
                    if (!path_exists) resetWallPlacement(white, wall_matrix, i, j);
                }

                // place vertical wall
                if (!thereIsAWall('r', wall_matrix, boardsize, i, j))
                {
                    placeWall(white, wall_matrix, 'r', i, j);
                    path_exists = false;
                    if (there_is_a_path(wall_matrix, boardsize, white, black))
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                        path_exists = 1;
                        updateEvalWhite(&max_eval, &eval, &alpha);
                        resetWallPlacement(white, wall_matrix, i, j);
                        if (beta <= alpha) return max_eval;
                    }
                    if (!path_exists) resetWallPlacement(white, wall_matrix, i, j);
                }
                if (white->walls == 0) return max_eval;
            }
        }
        return max_eval;
    }
    else  // Minimizing - Black
    {
        // check each possible move

        // -check pawn movement-
        int min_eval = INFINITY;

        // check diagonal moves
        if (black->i-1 == white->i && black->j == white->j && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // white below black
        {
            if (white->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize))
            {
                (black->i)-=2;
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                updateEvalBlack(&min_eval, &eval, &beta);
                (black->i)+=2;  // reset movement
                if (beta <= alpha) return min_eval;
            }
            else
            {
                if (black->j < boardsize-1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))  // diagonally  down - right
                {
                    --black->i;
                    ++black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                    updateEvalBlack(&min_eval, &eval, &beta);
                    ++black->i;  // reset movement
                    --black->j;
                    if (beta <= alpha) return min_eval; 
                }

                if (black->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    --black->i;
                    --black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                    updateEvalBlack(&min_eval, &eval, &beta);
                    ++black->i;  // reset movement
                    ++black->j;
                    if (beta <= alpha) return min_eval;
                }
            }
        }
        else if (black->i == white->i && black->j-1 == white->j && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // white on the left of black
        {
            if (white->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))
            {
                (black->j)-=2;
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                updateEvalBlack(&min_eval, &eval, &beta);
                (black->j)+=2;  // reset movement
                if (beta <= alpha) return min_eval;
            }
            else
            {
                if (black->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    --black->i;
                    --black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                    updateEvalBlack(&min_eval, &eval, &beta);
                    ++black->i;  // reset movement
                    ++black->j;
                    if (beta <= alpha) return min_eval; 
                }
                
                if (black->i < boardsize - 1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    ++black->i;
                    --black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                    updateEvalBlack(&min_eval, &eval, &beta);
                    --black->i;  // reset movement
                    ++black->j;
                    if (beta <= alpha) return min_eval; 
                }
            }
        }
        else if (black->i == white->i && black->j+1 == white->j && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))  // white on the right of black
        {
            if (white->j < boardsize-1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))
            {
                (black->j)+=2;
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                updateEvalBlack(&min_eval, &eval, &beta);
                (black->j)-=2;  // reset movement
                if (beta <= alpha) return min_eval;
            }
            else
            {
                if (black->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize))  // diagonally down - right
                {
                    --black->i;
                    ++black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                    updateEvalBlack(&min_eval, &eval, &beta);
                    ++black->i;  // reset movement
                    --black->j;
                    if (beta <= alpha) return min_eval; 
                }

                if (black->i < boardsize-1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    ++black->i;
                    ++black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                    updateEvalBlack(&min_eval, &eval, &beta);
                    --black->i;  // reset movement
                    --black->j;
                    if (beta <= alpha) return min_eval; 
                }
            }
        } 
        else if (black->i+1 == white->i && black->j == white->j && !wallAbove(black->i, black->j, wall_matrix, boardsize))  // white above black
        {
            if (white->i < boardsize-1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))
            {
                (black->i)+=2;
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                updateEvalBlack(&min_eval, &eval, &beta);
                (black->i)-=2;  // reset movement
                if (beta <= alpha) return min_eval;
            }
            else
            {
                if (black->j < boardsize-1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    ++black->i;
                    ++black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                    updateEvalBlack(&min_eval, &eval, &beta);
                    --black->i;  // reset movement
                    --black->j;
                    if (beta <= alpha) return min_eval; 
                }

                if (black->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    ++black->i;
                    --black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                    updateEvalBlack(&min_eval, &eval, &beta);
                    --black->i;  // reset movement
                    ++black->j;
                    if (beta <= alpha) return min_eval; 
                }
            }
        }
        // check normal moves
        if (black->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize) && !(black->i - 1 == white->i && black->j == white->j))  // down
        {
            --black->i;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
            updateEvalBlack(&min_eval, &eval, &beta);
            ++black->i;  // reset movement
            if (beta <= alpha) return min_eval;
        }
        if (black->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize) && !(black->i == white->i && black->j - 1 == white->j))  // left
        {
            --black->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
            updateEvalBlack(&min_eval, &eval, &beta);
            ++black->j;  // reset movement
            if (beta <= alpha) return min_eval;
        }
        if (black->j < boardsize-1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize) && !(black->i == white->i && black->j + 1 == white->j))  // right
        {
            ++black->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
            updateEvalBlack(&min_eval, &eval, &beta);
            --black->j;  // reset movement
            if (beta <= alpha) return min_eval;
        }      
        if (black->i < boardsize-1 && !wallAbove(black->i, black->j, wall_matrix, boardsize) && !(black->i + 1 == white->i && black->j == white->j))  // up
        {
            ++black->i;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
            updateEvalBlack(&min_eval, &eval, &beta);
            --black->i;  // reset movement
            if (beta <= alpha) return min_eval;
        }

        // -check wall placement-
        if (black->walls == 0) return min_eval;  // black does not have sufficient walls to place
        
        if (depth == 1 && pseudo == true)
        {
            // last search of a pseudodepth check, only check wall placement near white's pawn
            start = boardsize-1;
            if (white->i < boardsize-1) start = white->i+1;

            end = 1;
            if (white->i > 1) end = white->i-1;
        }
        else
        {
            start = white->i + start_range;
            if (start > boardsize-1) start = boardsize-1;

            end = white->i - end_range;
            if (end < 1) end = 1;
        }

        for (int i = start; i > end; i--)
        {
            for (int j = 0; j < boardsize-1; j++)
            {
                // place horizontal wall
                if (!thereIsAWall('b', wall_matrix, boardsize, i, j))
                {
                    path_exists = false;
                    placeWall(black, wall_matrix, 'b', i, j);
                    if (there_is_a_path_black(wall_matrix, boardsize, white, black))
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                        path_exists = true;
                        updateEvalBlack(&min_eval, &eval, &beta);
                        resetWallPlacement(black, wall_matrix, i, j);
                        if (beta <= alpha) return min_eval;
                    }
                    if (!path_exists) resetWallPlacement(black, wall_matrix, i, j);
                }

                // place vertical wall
                if (!thereIsAWall('r', wall_matrix, boardsize, i, j))
                {
                    placeWall(black, wall_matrix, 'r', i, j);
                    path_exists = false;
                    if (there_is_a_path_black(wall_matrix, boardsize, white, black))
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                        path_exists = true;
                        updateEvalBlack(&min_eval, &eval, &beta);
                        resetWallPlacement(black, wall_matrix, i, j);
                        if (beta <= alpha) return min_eval;
                    }
                    if (!path_exists) resetWallPlacement(black, wall_matrix, i, j);
                }
            }
        }
        return min_eval;
    }
}

// Helper functions

static void placeWall(player* pl, char** wall_matrix, char orientation, cint i, cint j)
{
    pl->walls--;
    wall_matrix[i][j] = orientation;
}

static void resetWallPlacement(player* pl, char** wall_matrix, cint i, cint j)
{
    pl->walls++;
    wall_matrix[i][j] = 0;
}

static void uwp_white(returningMove* evalMove, cchar orientation, cint i, cint j, int* eval, int* max_eval)
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

static void uwp_black(returningMove* evalMove, cchar orientation, cint i, cint j, int* eval, int* min_eval)
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

static void updatePawnMovementWhite(player* white, returningMove* evalMove, int* eval, int* max_eval)
{
    if (*eval > *max_eval)
    {
        *max_eval = *eval;
        evalMove->move = 'm';
        evalMove->x = white->i;
        evalMove->y = white->j;
    }
}

static void updatePawnMovementBlack(player* black, returningMove* evalMove, int* eval, int* min_eval)
{
    if (*eval < *min_eval)
    {
        *min_eval = *eval;
        evalMove->move = 'm';
        evalMove->x = black->i;
        evalMove->y = black->j;
    }
}

static void updateEvalWhite(int* max_eval, int* eval, int* alpha)
{
    *max_eval = *max_eval > *eval ? *max_eval:*eval;
    *alpha = *alpha > *eval ? *alpha:*eval;
}

static void updateEvalBlack(int* min_eval, int* eval, int* beta)
{
    *min_eval = *min_eval < *eval ? *min_eval:*eval;
    *beta = *beta < *eval ? *beta:*eval;
}

static void check_walls(char** wall_matrix, player* white, player* black, player* curr_player, cint boardsize, csint depth, cchar pseudo, int start, cint end, cfloat max_time,
int* eval, int* best_eval, const clock_t t, returningMove* evalMove, QuitFunc quit, ChangeFunc change, cchar cond, updateWallWallPlacement uwp, char col, cfloat scope)
{
    cint range = boardsize/scope, start_range = MINIMAX_START_RANGE(range), end_range = MINIMAX_END_RANGE(range);
    check_path path_exists;
    if (col == 'w') path_exists = there_is_a_path;
    else path_exists = there_is_a_path_black;
    
    while (true)
    {
        for (int j = 0; j < boardsize-1; j++)
        {
            // place horizontal wall
            if (!thereIsAWall('b', wall_matrix, boardsize, start, j))
            {
                placeWall(curr_player, wall_matrix, 'b', start, j);
                if (path_exists(wall_matrix, boardsize, white, black))
                {
                    *eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, cond, pseudo, start_range, end_range);
                    uwp(evalMove, 'b', start, j, eval, best_eval);
                }
                resetWallPlacement(curr_player, wall_matrix, start, j);  // reset placing the wall
            }
            if (((double)(clock() - t))/CLOCKS_PER_SEC > max_time) return;  // ran out of time

            // place vertical wall
            if (!thereIsAWall('r', wall_matrix, boardsize, start, j))
            {
                placeWall(curr_player, wall_matrix, 'r', start, j);
                if (path_exists(wall_matrix, boardsize, white, black))
                {
                    *eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, cond, pseudo, start_range, end_range);
                    uwp(evalMove, 'r', start, j, eval, best_eval);
                }
                resetWallPlacement(curr_player, wall_matrix, start, j);  // reset placing the wall
                if ((((double)clock() - t)/CLOCKS_PER_SEC) > max_time) return;  // ran out of time
            }
        }
        if ((((double)clock() - t)/CLOCKS_PER_SEC) > max_time) return;  // ran out of time
        change(&start);
        if (!quit(start, end)) return;
    }
}