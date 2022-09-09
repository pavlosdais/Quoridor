#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "../../include/typedefs.h"
#include "../../include/structs.h"
#include "../../include/utilities.h"
#include "../../include/eng_helper.h"

// Functions needed for wall placement

/* For each potential move the corresponding player has, search the evaluated best responses for both players up to a certain
depth, or moves ahead, and return the move which is believed to be the most advantageous for the player who's playing assuming
optimal play for both sides. */

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
                MOVE_UP2(white);
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
                updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                MOVE_DOWN2(white);
            }
            else
            {
                if (white->j < boardsize-1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    MOVE_UP(white); MOVE_RIGHT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
                    updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    MOVE_DOWN(white); MOVE_LEFT(white);   
                }

                if (white->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    MOVE_UP(white); MOVE_LEFT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
                    updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    MOVE_DOWN(white); MOVE_RIGHT(white);   
                }
            }
        }
        else if (white->i == black->i && white->j+1 == black->j && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))  // black on the right of white
        {
            if (black->j < boardsize - 1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))
            {
                MOVE_RIGHT2(white);
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
                updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                MOVE_LEFT2(white);
            }
            else
            {
                if (white->i < boardsize-1 && !wallAbove(black->i, black->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    MOVE_UP(white); MOVE_RIGHT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
                    updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    MOVE_DOWN(white); MOVE_LEFT(white);
                }

                if (white->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // diagonally down - right
                {
                    MOVE_DOWN(white); MOVE_RIGHT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
                    updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    MOVE_UP(white); MOVE_LEFT(white);
                }
            }
        }
        else if (white->i == black->i && white->j-1 == black->j && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // black on the left of white
        {
            if (black->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))
            {
                MOVE_LEFT2(white);
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
                updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                MOVE_RIGHT2(white);
            }
            else
            {
                if (white->i < boardsize-1 && !wallAbove(black->i, black->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    MOVE_UP(white); MOVE_LEFT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
                    updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    MOVE_DOWN(white); MOVE_RIGHT(white);
                }

                if (white->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    MOVE_DOWN(white); MOVE_LEFT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
                    updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    MOVE_UP(white); MOVE_RIGHT(white);
                }
            }
        }
        else if (white->i-1 == black->i && white->j == black->j && !wallBelow(white->i, white->j, wall_matrix, boardsize))  // black below white
        {
            if (black->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize))
            {
                MOVE_DOWN2(white);
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
                updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                MOVE_UP2(white);
            }
            else
            {
                if (white->j < boardsize-1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))  // diagonally  down - right
                {
                    MOVE_DOWN(white); MOVE_RIGHT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
                    updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    MOVE_UP(white); MOVE_LEFT(white);   
                }

                if (white->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    MOVE_DOWN(white); MOVE_LEFT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
                    updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    MOVE_UP(white); MOVE_RIGHT(white);
                }
            }
        }     
        // check normal moves
        if (white->i < boardsize-1 && !wallAbove(white->i, white->j, wall_matrix, boardsize) && !(white->i + 1 == black->i && white->j == black->j))  // up
        {
            MOVE_UP(white);
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
            updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
            MOVE_DOWN(white);
        }
        if (white->j < boardsize - 1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize) && !(white->i == black->i && white->j + 1 == black->j))  // right
        {
            MOVE_RIGHT(white);
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
            updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
            MOVE_LEFT(white);
        }
        if (white->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize) && !(white->i == black->i && white->j - 1 == black->j))  // left
        {
            MOVE_LEFT(white);
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
            updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
            MOVE_RIGHT(white);
        }
        if (white->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize) && !(white->i - 1 == black->i && white->j == black->j))  // down
        {
            MOVE_DOWN(white);
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, start_range, end_range);
            updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
            MOVE_UP(white);
        }

        // -check wall placement-
        if (white->walls != 0) 
            check_walls_white(wall_matrix, white, black, boardsize, depth-1, pseudo, start_range, end_range, max_time, &eval, &max_eval, t, &evalMove, scope);
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
                MOVE_DOWN2(black);
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);
                updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                MOVE_UP2(black);
            }
            else
            {
                if (black->j < boardsize-1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))  // diagonally  down - right
                {
                    MOVE_DOWN(black); MOVE_RIGHT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);
                    updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    MOVE_UP(black); MOVE_LEFT(black);
                    
                }

                if (black->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    MOVE_DOWN(black); MOVE_LEFT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);
                    updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    MOVE_UP(black); MOVE_RIGHT(black);
                }
            }
        }
        else if (black->i == white->i && black->j-1 == white->j && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // white on the left of black
        {
            if (white->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))
            {
                MOVE_LEFT2(black);
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);
                updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                MOVE_RIGHT2(black);
            }
            else
            {
                if (black->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    MOVE_DOWN(black); MOVE_LEFT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);
                    updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    MOVE_UP(black); MOVE_RIGHT(black);
                }
                
                if (black->i < boardsize - 1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    MOVE_UP(black); MOVE_LEFT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);
                    updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    MOVE_DOWN(black); MOVE_RIGHT(black);
                }
            }
        }
        else if (black->i == white->i && black->j+1 == white->j && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))  // white on the right of black
        {
            if (white->j < boardsize - 1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))
            {
                MOVE_RIGHT2(black);
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);
                updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                MOVE_LEFT2(black);
            }
            else
            {
                if (black->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize))  // diagonally down - right
                {
                    MOVE_DOWN(black); MOVE_RIGHT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);
                    updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    MOVE_UP(black); MOVE_LEFT(black); 
                }
                if (black->i < boardsize - 1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    MOVE_UP(black); MOVE_RIGHT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);
                    updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    MOVE_DOWN(black); MOVE_LEFT(black);   
                }
            }
        }
        else if (black->i+1 == white->i && black->j == white->j && !wallAbove(black->i, black->j, wall_matrix, boardsize))  // white above black
        {
            if (white->i < boardsize - 1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))
            {
                MOVE_UP2(black);
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);
                updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                MOVE_DOWN2(black);
            }
            else
            {
                if (black->j < boardsize-1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    MOVE_UP(black); MOVE_RIGHT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);
                    updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    MOVE_DOWN(black); MOVE_LEFT(black);
                }

                if (black->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    MOVE_UP(black); MOVE_LEFT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);

                    updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    MOVE_DOWN(black); MOVE_RIGHT(black);
                }
            }
        }
        // check normal moves
        if (black->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize) && !(black->i - 1 == white->i && black->j == white->j))  // down
        {
            MOVE_DOWN(black);
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);
            updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
            MOVE_UP(black);
        }
        if (black->j < boardsize-1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize) && !(black->i == white->i && black->j + 1 == white->j))  // right
        {
            MOVE_RIGHT(black);
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);
            updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
            MOVE_LEFT(black);
        }
        if (black->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize) && !(black->i == white->i && black->j - 1 == white->j))  // left
        {
            MOVE_LEFT(black);
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);
            updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
            MOVE_RIGHT(black);
        }      
        if (black->i < boardsize - 1 && !wallAbove(black->i, black->j, wall_matrix, boardsize) && !(black->i + 1 == white->i && black->j == white->j))  // up
        {
            MOVE_UP(black);
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, start_range, end_range);
            updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
            MOVE_DOWN(black);
        }

        // -check wall placement-
        if (black->walls != 0)
            check_walls_black(wall_matrix, white, black, boardsize, depth-1, pseudo, start_range, end_range, max_time, &eval, &min_eval, t, &evalMove, scope);
    }

    return evalMove;
}

/* sources: https://en.wikipedia.org/wiki/Minimax
            https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning */
int minimax(char** wall_matrix, cint boardsize, csint depth, int alpha, int beta, player* white, player* black, char Maximizing, cchar pseudo, cint start_range, cint end_range)
{
    if (depth == 0 || white->i == boardsize-1 || black->i == 0)  // Base case
        return positionEvaluation(*black, *white, boardsize, wall_matrix);

    int eval;
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
                MOVE_UP2(white);
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                max_eval = MAX(max_eval, eval);
                alpha = MAX(alpha, eval);
                MOVE_DOWN2(white);
                if (beta <= alpha) return max_eval;
            }
            else
            {
                if (white->j < boardsize - 1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    MOVE_UP(white); MOVE_RIGHT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                    max_eval = MAX(max_eval, eval);
                    alpha = MAX(alpha, eval);
                    MOVE_DOWN(white); MOVE_LEFT(white);
                    if (beta <= alpha) return max_eval; 
                }

                if (white->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    MOVE_UP(white); MOVE_LEFT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                    max_eval = MAX(max_eval, eval);
                    alpha = MAX(alpha, eval);
                    MOVE_DOWN(white); MOVE_RIGHT(white);
                    if (beta <= alpha) return max_eval; 
                }
            }
        }
        else if (white->i == black->i && white->j+1 == black->j && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))  // black on the right of white
        {
            if (black->j < boardsize-1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))
            {
                MOVE_RIGHT2(white);
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                max_eval = MAX(max_eval, eval);
                alpha = MAX(alpha, eval);
                MOVE_LEFT2(white);
                if (beta <= alpha) return max_eval;
            }
            else
            {
                if (white->i < boardsize-1 && !wallAbove(black->i, black->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    MOVE_UP(white); MOVE_RIGHT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                    max_eval = MAX(max_eval, eval);
                    alpha = MAX(alpha, eval);
                    MOVE_DOWN(white); MOVE_LEFT(white);
                    if (beta <= alpha) return max_eval; 
                }

                if (white->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // diagonally down - right
                {
                    MOVE_DOWN(white); MOVE_RIGHT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                    max_eval = MAX(max_eval, eval);
                    alpha = MAX(alpha, eval);
                    MOVE_UP(white); MOVE_LEFT(white);
                    if (beta <= alpha) return max_eval; 
                }
            }
        }
        else if (white->i == black->i && white->j - 1 == black->j && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // black on the left of white
        {
            if (black->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))
            {
                MOVE_LEFT2(white);
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                max_eval = MAX(max_eval, eval);
                alpha = MAX(alpha, eval);
                MOVE_RIGHT2(white);
                if (beta <= alpha) return max_eval;
            }
            else
            {
                if (white->i < boardsize-1 && !wallAbove(black->i, black->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    MOVE_UP(white); MOVE_LEFT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                    max_eval = MAX(max_eval, eval);
                    alpha = MAX(alpha, eval);
                    MOVE_DOWN(white); MOVE_RIGHT(white);
                    if (beta <= alpha) return max_eval; 
                }

                if (white->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    MOVE_DOWN(white); MOVE_LEFT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                    max_eval = MAX(max_eval, eval);
                    alpha = MAX(alpha, eval);
                    MOVE_UP(white); MOVE_RIGHT(white);
                    if (beta <= alpha) return max_eval; 
                }
            }
        }
        else if (white->i-1 == black->i && white->j == black->j && !wallBelow(white->i, white->j, wall_matrix, boardsize))  // black below white
        {
            if (black->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize))
            {
                MOVE_DOWN2(white);
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                max_eval = MAX(max_eval, eval);
                alpha = MAX(alpha, eval);
                MOVE_UP2(white);
                if (beta <= alpha) return max_eval;
            }
            else
            {
                if (white->j < boardsize-1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))  // diagonally down - right
                {
                    MOVE_DOWN(white); MOVE_RIGHT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                    max_eval = MAX(max_eval, eval);
                    alpha = MAX(alpha, eval);
                    MOVE_UP(white); MOVE_LEFT(white);
                    if (beta <= alpha) return max_eval; 
                }

                if (white->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    MOVE_DOWN(white); MOVE_LEFT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                    max_eval = MAX(max_eval, eval);
                    alpha = MAX(alpha, eval);
                    MOVE_UP(white); MOVE_RIGHT(white);
                    if (beta <= alpha) return max_eval; 
                }
            }
        }
        // check normal moves
        if (white->i < boardsize-1 && !wallAbove(white->i, white->j, wall_matrix, boardsize) && !(white->i + 1 == black->i && white->j == black->j))  // up
        {
            MOVE_UP(white);
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
            max_eval = MAX(max_eval, eval);
            alpha = MAX(alpha, eval);
            MOVE_DOWN(white);
            if (beta <= alpha) return max_eval;
        }
        if (white->j < boardsize - 1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize) && !(white->i == black->i && white->j + 1 == black->j))  // right
        {
            MOVE_RIGHT(white);
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
            max_eval = MAX(max_eval, eval);
            alpha = MAX(alpha, eval);
            MOVE_LEFT(white);
            if (beta <= alpha) return max_eval;
        }    
        if (white->j > 0 &&!wallOnTheLeft(white->i, white->j, wall_matrix, boardsize) && !(white->i == black->i && white->j - 1 == black->j))  // left
        {
            MOVE_LEFT(white);
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
            max_eval = MAX(max_eval, eval);
            alpha = MAX(alpha, eval);
            MOVE_RIGHT(white);
            if (beta <= alpha) return max_eval;
        } 
        if (white->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize) && !(white->i - 1 == black->i && white->j == black->j))  // down
        {
            MOVE_DOWN(white);
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
            max_eval = MAX(max_eval, eval);
            alpha = MAX(alpha, eval);
            MOVE_UP(white);
            if (beta <= alpha) return max_eval;
        }
        
        // -check wall placement-
        if (white->walls == 0) return max_eval;  // white does not have sufficient walls to place

        if (depth == 1 && pseudo == true)
        {
            // last search of a pseudodepth check, only check wall placement near black's pawn
            int start = 1;
            if (black->i > 1) start = black->i-1;

            int end = boardsize;
            if (black->i < boardsize-1) end = black->i+1;
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
                            max_eval = MAX(max_eval, eval);
                            alpha = MAX(alpha, eval);
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
                            max_eval = MAX(max_eval, eval);
                            alpha = MAX(alpha, eval);
                            resetWallPlacement(white, wall_matrix, i, j);
                            if (beta <= alpha) return max_eval;
                        }
                        if (!path_exists) resetWallPlacement(white, wall_matrix, i, j);
                    }
                    if (white->walls == 0) return max_eval;
                }
            }
        }
        else 
        {
            if (!thereIsAWall('b', wall_matrix, boardsize, black->i, black->j))
            {
                path_exists = false;
                placeWall(white, wall_matrix, 'b', black->i, black->j);
                if (there_is_a_path(wall_matrix, boardsize, white, black))
                {
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                    path_exists = true;
                    max_eval = MAX(max_eval, eval);
                    alpha = MAX(alpha, eval);
                    resetWallPlacement(white, wall_matrix, black->i, black->j);  // reset placing the wall
                    if (beta <= alpha) return max_eval;
                }
                if(!path_exists) resetWallPlacement(white, wall_matrix, black->i, black->j);  // reset placing the wall
            }
            if (!thereIsAWall('r', wall_matrix, boardsize, black->i, black->j))
            {
                placeWall(white, wall_matrix, 'r', black->i, black->j);
                path_exists = false;
                if (there_is_a_path(wall_matrix, boardsize, white, black))
                {
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                    path_exists = true;
                    max_eval = MAX(max_eval, eval);
                    alpha = MAX(alpha, eval);
                    resetWallPlacement(white, wall_matrix, black->i, black->j);  // reset placing the wall
                    if (beta <= alpha) return max_eval;
                }
                if(!path_exists) resetWallPlacement(white, wall_matrix, black->i, black->j);  // reset placing the wall
            }

            int s = MIN(black->i, start_range)+1, a = boardsize - black->i, e = MIN(a, end_range)+1, end = 8;
            for (small_int j = 1; j <= s; j++)
            {
                if (e == 0) end = PRIMARY;
                else  e--;
                for (small_int i = 0; i < end; i++)
                {
                    int rr = black->i + j*drwhite[i], cc = black->j + j*dcwhite[i];
                    if (!(rr >= 0 && rr < boardsize) || !(cc >= 0 && cc < boardsize) || rr == 0 || cc >= boardsize-1)  // orientation out of bounds
                        continue;
                    
                    if (!thereIsAWall('b', wall_matrix, boardsize, rr, cc))
                    {
                        placeWall(white, wall_matrix, 'b', rr, cc);
                        path_exists = false;
                        if (there_is_a_path(wall_matrix, boardsize, white, black))
                        {
                            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                            path_exists = true;
                            max_eval = MAX(max_eval, eval);
                            alpha = MAX(alpha, eval);
                            resetWallPlacement(white, wall_matrix, rr, cc);  // reset placing the wall
                            if (beta <= alpha) return max_eval;
                        }
                        if(!path_exists) resetWallPlacement(white, wall_matrix, rr, cc);  // reset placing the wall
                    }

                    // place vertical wall
                    if (!thereIsAWall('r', wall_matrix, boardsize, rr, cc))
                    {
                        placeWall(white, wall_matrix, 'r', rr, cc);
                        path_exists = false;
                        if (there_is_a_path(wall_matrix, boardsize, white, black))
                        {
                            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, start_range, end_range);
                            path_exists = true;
                            max_eval = MAX(max_eval, eval);
                            alpha = MAX(alpha, eval);
                            resetWallPlacement(white, wall_matrix, rr, cc);  // reset placing the wall
                            if (beta <= alpha) return max_eval;
                        }
                        if (!path_exists) resetWallPlacement(white, wall_matrix, rr, cc);  // reset placing the wall
                    }
                }
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
                MOVE_DOWN2(black);
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                min_eval = MIN(min_eval, eval);
                beta = MIN(beta, eval);
                MOVE_UP2(black);
                if (beta <= alpha) return min_eval;
            }
            else
            {
                if (black->j < boardsize-1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))  // diagonally  down - right
                {
                    MOVE_DOWN(black); MOVE_RIGHT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                    min_eval = MIN(min_eval, eval);
                    beta = MIN(beta, eval);
                    MOVE_UP(black); MOVE_LEFT(black);
                    if (beta <= alpha) return min_eval; 
                }

                if (black->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    MOVE_DOWN(black); MOVE_LEFT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                    min_eval = MIN(min_eval, eval);
                    beta = MIN(beta, eval);
                    MOVE_UP(black); MOVE_RIGHT(black);
                    if (beta <= alpha) return min_eval;
                }
            }
        }
        else if (black->i == white->i && black->j-1 == white->j && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // white on the left of black
        {
            if (white->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))
            {
                MOVE_LEFT2(black);
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                min_eval = MIN(min_eval, eval);
                beta = MIN(beta, eval);
                MOVE_RIGHT2(black);
                if (beta <= alpha) return min_eval;
            }
            else
            {
                if (black->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    MOVE_DOWN(black); MOVE_LEFT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                    min_eval = MIN(min_eval, eval);
                    beta = MIN(beta, eval);
                    MOVE_UP(black); MOVE_RIGHT(black);
                    if (beta <= alpha) return min_eval; 
                }
                
                if (black->i < boardsize - 1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    MOVE_UP(black); MOVE_LEFT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                    min_eval = MIN(min_eval, eval);
                    beta = MIN(beta, eval);
                    MOVE_DOWN(black); MOVE_RIGHT(black);
                    if (beta <= alpha) return min_eval; 
                }
            }
        }
        else if (black->i == white->i && black->j+1 == white->j && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))  // white on the right of black
        {
            if (white->j < boardsize-1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))
            {
                MOVE_RIGHT2(black);
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                min_eval = MIN(min_eval, eval);
                beta = MIN(beta, eval);
                MOVE_LEFT2(black);
                if (beta <= alpha) return min_eval;
            }
            else
            {
                if (black->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize))  // diagonally down - right
                {
                    MOVE_DOWN(black); MOVE_RIGHT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                    min_eval = MIN(min_eval, eval);
                    beta = MIN(beta, eval);
                    MOVE_UP(black); MOVE_LEFT(black);
                    if (beta <= alpha) return min_eval; 
                }

                if (black->i < boardsize-1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    MOVE_UP(black); MOVE_RIGHT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                    min_eval = MIN(min_eval, eval);
                    beta = MIN(beta, eval);
                    MOVE_DOWN(black); MOVE_LEFT(black);
                    if (beta <= alpha) return min_eval; 
                }
            }
        } 
        else if (black->i+1 == white->i && black->j == white->j && !wallAbove(black->i, black->j, wall_matrix, boardsize))  // white above black
        {
            if (white->i < boardsize-1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))
            {
                MOVE_UP2(black);
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                min_eval = MIN(min_eval, eval);
                beta = MIN(beta, eval);
                MOVE_DOWN2(black);
                if (beta <= alpha) return min_eval;
            }
            else
            {
                if (black->j < boardsize-1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    MOVE_UP(black); MOVE_RIGHT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                    min_eval = MIN(min_eval, eval);
                    beta = MIN(beta, eval);
                    MOVE_DOWN(black); MOVE_LEFT(black);
                    if (beta <= alpha) return min_eval; 
                }
                if (black->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    MOVE_UP(black); MOVE_LEFT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                    min_eval = MIN(min_eval, eval);
                    beta = MIN(beta, eval);
                    MOVE_DOWN(black); MOVE_RIGHT(black);
                    if (beta <= alpha) return min_eval; 
                }
            }
        }
        // check normal moves
        if (black->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize) && !(black->i - 1 == white->i && black->j == white->j))  // down
        {
            MOVE_DOWN(black);
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
            min_eval = MIN(min_eval, eval);
            beta = MIN(beta, eval);
            MOVE_UP(black);
            if (beta <= alpha) return min_eval;
        }
        if (black->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize) && !(black->i == white->i && black->j - 1 == white->j))  // left
        {
            MOVE_LEFT(black);
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
            min_eval = MIN(min_eval, eval);
            beta = MIN(beta, eval);
            MOVE_RIGHT(black);
            if (beta <= alpha) return min_eval;
        }
        if (black->j < boardsize-1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize) && !(black->i == white->i && black->j + 1 == white->j))  // right
        {
            MOVE_RIGHT(black);
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
            min_eval = MIN(min_eval, eval);
            beta = MIN(beta, eval);
            MOVE_LEFT(black);
            if (beta <= alpha) return min_eval;
        }      
        if (black->i < boardsize-1 && !wallAbove(black->i, black->j, wall_matrix, boardsize) && !(black->i + 1 == white->i && black->j == white->j))  // up
        {
            MOVE_UP(black);
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
            min_eval = MIN(min_eval, eval);
            beta = MIN(beta, eval);
            MOVE_DOWN(black);
            if (beta <= alpha) return min_eval;
        }

        // -check wall placement-
        if (black->walls == 0) return min_eval;  // black does not have sufficient walls to place
        
        if (depth == 1 && pseudo == true)
        {
            // last search of a pseudodepth check, only check wall placement near white's pawn
            int start = boardsize-1;
            if (white->i < boardsize-1) start = white->i+1;

            int end = 1;
            if (white->i > 1) end = white->i-1;
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
                            min_eval = MIN(min_eval, eval);
                            beta = MIN(beta, eval);
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
                            min_eval = MIN(min_eval, eval);
                            beta = MIN(beta, eval);
                            resetWallPlacement(black, wall_matrix, i, j);
                            if (beta <= alpha) return min_eval;
                        }
                        if (!path_exists) resetWallPlacement(black, wall_matrix, i, j);
                    }
                }
            }
        }
        else
        {
            if (isValidWall(white->i, white->j, boardsize, wall_matrix, 'b'))
            {
                path_exists = false;
                placeWall(black, wall_matrix, 'b', white->i, white->j);
                if (there_is_a_path_black(wall_matrix, boardsize, white, black))
                {
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                    path_exists = true;
                    min_eval = MIN(min_eval, eval);
                    beta = MIN(beta, eval);
                    resetWallPlacement(black, wall_matrix, white->i, white->j);
                    if (beta <= alpha) return min_eval;
                }
                if (!path_exists) resetWallPlacement(black, wall_matrix, white->i, white->j);  // reset placing the wall
            }
            if (isValidWall(white->i, white->j, boardsize, wall_matrix, 'r'))
            {
                path_exists = false;
                placeWall(black, wall_matrix, 'r', white->i, white->j);
                if (there_is_a_path_black(wall_matrix, boardsize, white, black))
                {
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                    path_exists = true;
                    min_eval = MIN(min_eval, eval);
                    beta = MIN(beta, eval);
                    resetWallPlacement(black, wall_matrix, white->i, white->j);
                    if (beta <= alpha) return min_eval;
                }
                if (!path_exists) resetWallPlacement(black, wall_matrix, white->i, white->j);  // reset placing the wall
            }
            int a = boardsize - white->i, s = MIN(a, start_range)+1, e = end_range+1, end = 8;
            if (white->i + end_range > boardsize) e = boardsize - white->i+1;

            for (small_int j = 1; j <= s; j++, e--)
            {
                if (e == 0) end = PRIMARY;
                for (small_int i = 0; i < end; i++)
                {
                    int rr = white->i + j*drblack[i], cc = white->j + j*dcblack[i];
                    
                    if (!(rr >= 0 && rr < boardsize) || !(cc >= 0 && cc < boardsize) || rr == 0 || cc >= boardsize-1)  // orientation out of bounds
                        continue;
                    
                    if (!thereIsAWall('b', wall_matrix, boardsize, rr, cc))
                    {
                        path_exists = false;
                        placeWall(black, wall_matrix, 'b', rr, cc);
                        if (there_is_a_path_black(wall_matrix, boardsize, white, black))
                        {
                            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                            path_exists = true;
                            min_eval = MIN(min_eval, eval);
                            beta = MIN(beta, eval);
                            resetWallPlacement(black, wall_matrix, rr, cc);
                            if (beta <= alpha) return min_eval;
                        }
                        if (!path_exists) resetWallPlacement(black, wall_matrix, rr, cc);  // reset placing the wall
                    }
                    // place vertical wall
                    if (!thereIsAWall('r', wall_matrix, boardsize, rr, cc))
                    {
                        path_exists = false;
                        placeWall(black, wall_matrix, 'r', rr, cc);
                        if (there_is_a_path_black(wall_matrix, boardsize, white, black))
                        {
                            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, start_range, end_range);
                            path_exists = true;
                            min_eval = MIN(min_eval, eval);
                            beta = MIN(beta, eval);
                            resetWallPlacement(black, wall_matrix, rr, cc);
                            if (beta <= alpha) return min_eval;
                        }
                        if (!path_exists) resetWallPlacement(black, wall_matrix, rr, cc);  // reset placing the wall
                    }
                }
            }
        }
        return min_eval;
    }
}
