#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "../include/typedefs.h"
#include "../include/structs.h"
#include "../include/utilities.h"

typedef int (*QuitFunc)(int a, int b);
typedef int (*ChangeFunc)(int* a);
typedef void (*updateWallWallPlacement)(returningMove* evalMove, char orientation, int i, int j, int* eval, int* max_eval);

#define SCOPE 1.5

// Helper function prototypes
static int minimax(char** wall_matrix, const int boardsize, const small_int depth, int alpha, int beta, player* white, player* black, char Maximizing, const char pseudo, const int range);
static void uwp_white(returningMove* evalMove, const char orientation, const int i, const int j, int* eval, int* max_eval);
static void uwp_black(returningMove* evalMove, const char orientation, const int i, const int j, int* eval, int* min_eval);
static void updatePawnMovementWhite(player* white, returningMove* evalMove, int* eval, int* max_eval);
static void updatePawnMovementBlack(player* black, returningMove* evalMove, int* eval, int* min_eval);
static void placeWall(player* pl, char** wall_matrix, char orientation, const int i, const int j);
static void resetWallPlacement(player* pl, char** wall_matrix, const int i, const int j, char en);
static void updateEvalWhite(int* max_eval, int* eval, int* alpha);
static void updateEvalBlack(int* max_eval, int* eval, int* beta);
static void check_walls(char** wall_matrix, player* white, player* black, player* curr_player, const int boardsize, const small_int depth, const char pseudo, const int start, 
const int end, const float max_time, int* eval, int* best_eval, const clock_t t, returningMove* evalMove, QuitFunc quit, ChangeFunc change, const char cond, updateWallWallPlacement uwp);

/* For each potential move the corresponding player has, search the evaluated best responses for both players up to a certain
depth, or moves ahead, and return the move which is believed to be the most advantageous for the player who's playing assuming
optimal play for both sides. */

int compare_white(int a, int b) { return a < b; }
int change_white(int* a) { (*a)++; }

int compare_black(int a, int b) { return a > b; }
int change_black(int* a) { (*a)--; }

returningMove bestMove(char** wall_matrix, const int boardsize, const char pl, player* black, player* white, small_int depth, char pseudo, const float max_time)
{
    int eval, start, end, range = boardsize/SCOPE;
    returningMove evalMove;
    
    // check pawn movement
    int max_eval = NEG_INFINITY;
    clock_t t;
    if (pl == 'w')  // white plays
    {
        // check each possible move
        t = clock();
        // check pawn movement
        if (white->i+1 == black->i && white->j == black->j && !wallAbove(white->i, white->j, wall_matrix, boardsize)) // black above white
        {
            if (black->i < boardsize - 1 && !wallAbove(black->i, black->j, wall_matrix, boardsize))
            {
                (white->i)+=2;
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
                updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                (white->i)-=2;  // reset movement
            }
            else
            {
                if (white->j < boardsize-1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    ++white->i;
                    ++white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
                    updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    --white->i;  // reset movement
                    --white->j;
                }

                if (white->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    ++white->i;
                    --white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
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
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
                updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                (white->j)-=2;  // reset movement
            }
            else
            {
                if (white->i < boardsize-1 && !wallAbove(black->i, black->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    ++white->i;
                    ++white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
                    updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    --white->i;  // reset movement
                    --white->j;
                }

                if (white->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // diagonally down - right
                {
                    --white->i;
                    ++white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
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
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
                updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                (white->j)+=2;  // reset movement
            }
            else
            {
                if (white->i < boardsize-1 && !wallAbove(black->i, black->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    ++white->i;
                    --white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
                    updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    --white->i;  // reset movement
                    ++white->j;
                }

                if (white->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    --white->i;
                    --white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
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
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
                updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                (white->i)+=2;  // reset movement
            }
            else
            {
                if (white->j < boardsize-1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))  // diagonally  down - right
                {
                    --white->i;
                    ++white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
                    updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    ++white->i;  // reset movement
                    --white->j;
                }

                if (white->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    --white->i;
                    --white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
                    updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    ++white->i;  // reset movement
                    ++white->j;
                }
            }
        }
        
        if (white->i < boardsize-1 && !wallAbove(white->i, white->j, wall_matrix, boardsize) && !(white->i + 1 == black->i && white->j == black->j))  // up
        {
            ++white->i;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
            updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
            --white->i;  // reset movement
        }

        if (white->j < boardsize - 1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize) && !(white->i == black->i && white->j + 1 == black->j))  // right
        {
            ++white->j;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
            updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
            --white->j;  // reset movement
        }

        if (white->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize) && !(white->i == black->i && white->j - 1 == black->j))  // left
        {
            --white->j;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
            updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
            ++white->j;  // reset movement
        }
        
        if (white->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize) && !(white->i - 1 == black->i && white->j == black->j))  // down
        {
            --white->i;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
            updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
            ++white->i;  // reset movement
        }

        // check wall placement
        end = black->i + range;
        if (end > boardsize) end = boardsize;

        start = black->i - range;
        if (start < 1) start = 1;
        check_walls(wall_matrix, white, black, white, boardsize, depth, pseudo, start, end, max_time, &eval, &max_eval, t, &evalMove, compare_white, change_white, false, uwp_white);
    }
    else  // black plays
    {
        int min_eval = INFINITY;
        // check each possible move
        t = clock();
        // check pawn movement
        if (black->i-1 == white->i && black->j == white->j && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // white below black
        {
            if (white->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize))
            {
                (black->i)-=2;
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);
                updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                (black->i)+=2;  // reset movement
            }
            else
            {
                if (black->j < boardsize-1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))  // diagonally  down - right
                {
                    --black->i;
                    ++black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);

                    updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    ++black->i;  // reset movement
                    --black->j;
                }

                if (black->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    --black->i;
                    --black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);

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
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);
                updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                (black->j)+=2;  // reset movement
            }
            else
            {
                if (black->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    --black->i;
                    --black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);
                    updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    ++black->i;  // reset movement
                    ++black->j; 
                }
                
                if (black->i < boardsize - 1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    ++black->i;
                    --black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);
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
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);
                updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                (black->j)-=2;  // reset movement
            }
            else
            {
                if (black->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize))  // diagonally down - right
                {
                    --black->i;
                    ++black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);
                    updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    ++black->i;  // reset movement
                    --black->j; 
                }

                if (black->i < boardsize - 1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    ++black->i;
                    ++black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);
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
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);
                updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                (black->i)-=2;  // reset movement
            }
            else
            {
                if (black->j < boardsize-1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    ++black->i;
                    ++black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);

                    updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    --black->i;  // reset movement
                    --black->j; 
                }

                if (black->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    ++black->i;
                    --black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);

                    updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    --black->i;  // reset movement
                    ++black->j;
                }
            }
        }
        if (black->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize) && !(black->i - 1 == white->i && black->j == white->j))  // down
        {
            --black->i;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);
            updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
            ++black->i;  // reset movement
        }
        
        if (black->j < boardsize-1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize) && !(black->i == white->i && black->j + 1 == white->j))  // right
        {
            ++black->j;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);
            updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
            --black->j;  // reset movement
        }

        if (black->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize) && !(black->i == white->i && black->j - 1 == white->j))  // left
        {
            --black->j;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);
            updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
            ++black->j;  // reset movement
        }
        
        if (black->i < boardsize - 1 && !wallAbove(black->i, black->j, wall_matrix, boardsize) && !(black->i + 1 == white->i && black->j == white->j))  // up
        {
            ++black->i;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);
            updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
            --black->i;  // reset movement
        }

        // check wall placement
        start = white->i + range;
        if (start > boardsize-1) start = boardsize-1;

        end = white->i - range;
        if (end < 0) end = 0;

        check_walls(wall_matrix, white, black, black, boardsize, depth, pseudo, start, end, max_time, &eval, &min_eval, t, &evalMove, compare_black, change_black, true, uwp_black);
    }

    return evalMove;
}

/* sources: https://en.wikipedia.org/wiki/Minimax
            https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning */

static int minimax(char** wall_matrix, const int boardsize, const small_int depth, int alpha, int beta, player* white, player* black, char Maximizing, const char pseudo, const int range)
{
    if (depth == 0 || white->i == boardsize-1 || black->i == 0)  // Base case
        return positionEvaluation(*black, *white, boardsize, wall_matrix);

    int eval, start, end;
    char en;
    if (Maximizing)  // White
    {
        // check each possible move
    
        // check pawn movement
        int max_eval = NEG_INFINITY;

        if (white->i+1 == black->i && white->j == black->j && !wallAbove(white->i, white->j, wall_matrix, boardsize))  // black above white
        {
            if (black->i < boardsize-1 && !wallAbove(black->i, black->j, wall_matrix, boardsize))
            {
                (white->i)+=2;
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
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
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
                    updateEvalWhite(&max_eval, &eval, &alpha);
                    --white->i;  // reset movement
                    --white->j;
                    if (beta <= alpha) return max_eval; 
                }

                if (white->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    ++white->i;
                    --white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
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
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
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
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
                    updateEvalWhite(&max_eval, &eval, &alpha);
                    --white->i;  // reset movement
                    --white->j;
                    if (beta <= alpha) return max_eval; 
                }

                if (white->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // diagonally down - right
                {
                    --white->i;
                    ++white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
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
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
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
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
                    updateEvalWhite(&max_eval, &eval, &alpha);
                    --white->i;  // reset movement
                    ++white->j;
                    if (beta <= alpha) return max_eval; 
                }

                if (white->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    --white->i;
                    --white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
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
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
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
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
                    updateEvalWhite(&max_eval, &eval, &alpha);
                    ++white->i;  // reset movement
                    --white->j;
                    if (beta <= alpha) return max_eval; 
                }

                if (white->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    --white->i;
                    --white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
                    updateEvalWhite(&max_eval, &eval, &alpha);
                    ++white->i;  // reset movement
                    ++white->j;
                    if (beta <= alpha) return max_eval; 
                }
            }
        }

        if (white->i < boardsize-1 && !wallAbove(white->i, white->j, wall_matrix, boardsize) && !(white->i + 1 == black->i && white->j == black->j))  // up
        {
            ++white->i;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
            updateEvalWhite(&max_eval, &eval, &alpha);
            --white->i;  // reset movement
            if (beta <= alpha) return max_eval;
        }

        if (white->j < boardsize - 1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize) && !(white->i == black->i && white->j + 1 == black->j))  // right
        {
            ++white->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
            updateEvalWhite(&max_eval, &eval, &alpha);
            --white->j;  // reset movement
            if (beta <= alpha) return max_eval;
        }
        
        if (white->j > 0 &&!wallOnTheLeft(white->i, white->j, wall_matrix, boardsize) && !(white->i == black->i && white->j - 1 == black->j))  // left
        {
            --white->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
            updateEvalWhite(&max_eval, &eval, &alpha);
            ++white->j;  // reset movement
            if (beta <= alpha) return max_eval;
        }
     
        if (white->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize) && !(white->i - 1 == black->i && white->j == black->j))  // down
        {
            --white->i;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
            updateEvalWhite(&max_eval, &eval, &alpha);
            ++white->i;  // reset movement
            if (beta <= alpha) return max_eval;
        }
        
        // last search of a pseudodepth check, only check wall placement near black's pawn
        if (depth == 1 && pseudo == 1)
        {
            start = 1;
            end = boardsize;
            if (black->i > 1) start = black->i-1;
            if (black->i < boardsize-1) end = black->i+1;
        }
        else 
        {
            end = black->i + range;
            if (end > boardsize) end = boardsize;

            start = black->i - range;
            if (start < 1) start = 1;
        }
        // check wall placement
        for (int i = start; i < end; i++)
        {
            if (beta <= alpha || white->walls == 0) return max_eval;
            for (int j = 0; j < boardsize-1; j++)
            {
                if (white->walls == 0) return max_eval;
                if (!thereIsAWall('b', wall_matrix, boardsize, i, j))
                {
                    placeWall(white, wall_matrix, 'b', i, j);  // place horizontal wall
                    en = 0;
                    if (there_is_a_path(wall_matrix, boardsize, white, black))
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
                        en = 1;
                        updateEvalWhite(&max_eval, &eval, &alpha);
                        resetWallPlacement(white, wall_matrix, i, j, 0);
                        if (beta <= alpha) return max_eval;
                    }
                    resetWallPlacement(white, wall_matrix, i, j, en);
                }
                if (!thereIsAWall('r', wall_matrix, boardsize, i, j))
                {
                    placeWall(white, wall_matrix, 'r', i, j);  // place vertical wall
                    en = 0;
                    if (there_is_a_path(wall_matrix, boardsize, white, black))
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
                        en = 1;
                        updateEvalWhite(&max_eval, &eval, &alpha);
                        resetWallPlacement(white, wall_matrix, i, j, 0);
                        if (beta <= alpha) return max_eval;
                    }
                    resetWallPlacement(white, wall_matrix, i, j, en);
                } 
            }
        }
        return max_eval;
    }
    else  // !maximizingPlayer - Black
    {
        // check each possible move

        // check pawn movement
        int min_eval = INFINITY;

        if (black->i-1 == white->i && black->j == white->j && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // white below black
        {
            if (white->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize))
            {
                (black->i)-=2;
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
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
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
                    updateEvalBlack(&min_eval, &eval, &beta);
                    ++black->i;  // reset movement
                    --black->j;
                    if (beta <= alpha) return min_eval; 
                }

                if (black->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    --black->i;
                    --black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
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
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
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
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
                    updateEvalBlack(&min_eval, &eval, &beta);
                    ++black->i;  // reset movement
                    ++black->j;
                    if (beta <= alpha) return min_eval; 
                }
                
                if (black->i < boardsize - 1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    ++black->i;
                    --black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
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
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
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
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
                    updateEvalBlack(&min_eval, &eval, &beta);
                    ++black->i;  // reset movement
                    --black->j;
                    if (beta <= alpha) return min_eval; 
                }

                if (black->i < boardsize-1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    ++black->i;
                    ++black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
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
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
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
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
                    updateEvalBlack(&min_eval, &eval, &beta);
                    --black->i;  // reset movement
                    --black->j;
                    if (beta <= alpha) return min_eval; 
                }

                if (black->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    ++black->i;
                    --black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
                    updateEvalBlack(&min_eval, &eval, &beta);
                    --black->i;  // reset movement
                    ++black->j;
                    if (beta <= alpha) return min_eval; 
                }
            }
        }
     
        if (black->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize) && !(black->i - 1 == white->i && black->j == white->j))  // down
        {
            --black->i;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
            updateEvalBlack(&min_eval, &eval, &beta);
            ++black->i;  // reset movement
            if (beta <= alpha) return min_eval;
        }
     
        if (black->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize) && !(black->i == white->i && black->j - 1 == white->j))  // left
        {
            --black->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
            updateEvalBlack(&min_eval, &eval, &beta);
            ++black->j;  // reset movement
            if (beta <= alpha) return min_eval;
        }

        if (black->j < boardsize-1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize) && !(black->i == white->i && black->j + 1 == white->j))  // right
        {
            ++black->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
            updateEvalBlack(&min_eval, &eval, &beta);
            --black->j;  // reset movement
            if (beta <= alpha) return min_eval;
        }
        
        if (black->i < boardsize-1 && !wallAbove(black->i, black->j, wall_matrix, boardsize) && !(black->i + 1 == white->i && black->j == white->j))  // up
        {
            ++black->i;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
            updateEvalBlack(&min_eval, &eval, &beta);
            --black->i;  // reset movement
            if (beta <= alpha) return min_eval;
        }

        // check wall placement
        
        // last search of a pseudodepth check, only check wall placement near white's pawn
        if (pseudo == 1 && depth == 1)
        {
            start = boardsize-1;
            end = 1;
            if (white->i < boardsize-1) start = white->i+1;
            if (white->i > 1) end = white->i-1;
        }
        else
        {
            start = white->i + range;
            if (start > boardsize-1) start = boardsize-1;

            end = white->i - range;
            if (end < 0) end = 0;
        }
        for (int i = start; i > end; i--)
        {
            if (beta <= alpha || black->walls == 0) return min_eval;
            for (int j = 0; j < boardsize-1; j++)
            {
                if (black->walls == 0) return min_eval;
                if (!thereIsAWall('b', wall_matrix, boardsize, i, j))
                {
                    en = 0;
                    placeWall(black, wall_matrix, 'b', i, j);  // place horizontal wall
                    if (there_is_a_path(wall_matrix, boardsize, white, black))
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
                        en = 1;
                        updateEvalBlack(&min_eval, &eval, &beta);
                        resetWallPlacement(black, wall_matrix, i, j, 0);
                        if (beta <= alpha) return min_eval;
                    }
                    resetWallPlacement(black, wall_matrix, i, j, en);
                }
                if (!thereIsAWall('r', wall_matrix, boardsize, i, j))
                {
                    placeWall(black, wall_matrix, 'r', i, j);  // place vertical wall
                    en = 0;
                    if (there_is_a_path(wall_matrix, boardsize, white, black))
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
                        en = 1;
                        updateEvalBlack(&min_eval, &eval, &beta);
                        resetWallPlacement(black, wall_matrix, i, j, 0);
                        if (beta <= alpha) return min_eval;
                    }
                    resetWallPlacement(black, wall_matrix, i, j, en);
                }
            }
        }
        return min_eval;
    }
}

// Helper functions

static void placeWall(player* pl, char** wall_matrix, char orientation, const int i, const int j)
{
    pl->walls--;
    wall_matrix[i][j] = orientation;
}

static void resetWallPlacement(player* pl, char** wall_matrix, const int i, const int j, char en)
{
    if (!en) pl->walls++;
    wall_matrix[i][j] = 0;
}

static void uwp_white(returningMove* evalMove, const char orientation, const int i, const int j, int* eval, int* max_eval)
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

static void uwp_black(returningMove* evalMove, const char orientation, const int i, const int j, int* eval, int* min_eval)
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

static void check_walls(char** wall_matrix, player* white, player* black, player* curr_player, const int boardsize, const small_int depth, const char pseudo, const int start, 
const int end, const float max_time, int* eval, int* best_eval, const clock_t t, returningMove* evalMove, QuitFunc quit, ChangeFunc change, const char cond, updateWallWallPlacement uwp)
{
    if (curr_player->walls == 0) return;  // player has no walls, return
    int i = start, range = boardsize/SCOPE;
    while(true)
    {
        if (!quit(i, end)) return;
        else if ((((double)clock() - t)/CLOCKS_PER_SEC) > max_time) return;
        for (int j = 0; j < boardsize-1; j++)
        {
            if (((double)(clock() - t))/CLOCKS_PER_SEC > max_time) return;  // ran out of time
            if (!thereIsAWall('b', wall_matrix, boardsize, i, j))
            {
                placeWall(curr_player, wall_matrix, 'b', i, j);  // place horizontal wall
                if (there_is_a_path(wall_matrix, boardsize, white, black))
                {
                    *eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, cond, pseudo, range);
                    uwp(evalMove, 'b', i, j, eval, best_eval);
                }
                resetWallPlacement(curr_player, wall_matrix, i, j, 0);  // reset placing the wall
            }
            if (((double)(clock() - t))/CLOCKS_PER_SEC > max_time) return;
            if (!thereIsAWall('r', wall_matrix, boardsize, i, j))
            {
                placeWall(curr_player, wall_matrix, 'r', i, j);  // place vertical wall
                if (there_is_a_path(wall_matrix, boardsize, white, black))
                {
                    *eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, cond, pseudo, range);
                    uwp(evalMove, 'r', i, j, eval, best_eval);
                }
                resetWallPlacement(curr_player, wall_matrix, i, j, 0);  // reset placing the wall
            }
        }
        change(&i);
    }
}
