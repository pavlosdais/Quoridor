#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "../../include/dfs.h"
#include "../../include/typedefs.h"
#include "../../include/structs.h"
#include "../../include/utilities.h"
#include "../../include/engine_utilities.h"

/* For each potential move the corresponding player has, search the evaluated best responses for both players up to a certain
depth, or moves ahead, and return the move which is believed to be the most advantageous for the player who's playing assuming
optimal play for both sides. */

returningMove bestMove(char** wall_matrix, cint boardsize, cchar pl, player* black, player* white, small_int depth, char pseudo, cfloat max_time)
{
    int eval, start, end, range = boardsize/SCOPE;
    returningMove evalMove;
    
    // check pawn movement
    int max_eval = NEG_INFINITY;
    clock_t t = clock();;
    if (pl == 'w')  // white plays
    {
        // check each possible move
        
        // -check pawn movement-

        // diagonal moves
        if (white->i+1 == black->i && white->j == black->j && !wallAbove(white->i, white->j, wall_matrix, boardsize)) // black above white
        {
            if (black->i < boardsize - 1 && !wallAbove(black->i, black->j, wall_matrix, boardsize))
            {
                MOVE_UP2(white);
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
                if (eval > max_eval) updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                MOVE_DOWN2(white);
            }
            else
            {
                if (white->j < boardsize-1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    MOVE_UP(white); MOVE_RIGHT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
                    if (eval > max_eval) updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    MOVE_DOWN(white); MOVE_LEFT(white);   
                }

                if (white->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    MOVE_UP(white); MOVE_LEFT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
                    if (eval > max_eval) updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    MOVE_DOWN(white); MOVE_RIGHT(white);   
                }
            }
        }
        else if (white->i == black->i && white->j+1 == black->j && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))  // black on the right of white
        {
            if (black->j < boardsize - 1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))
            {
                MOVE_RIGHT2(white);
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
                if (eval > max_eval) updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                MOVE_LEFT2(white);
            }
            else
            {
                if (white->i < boardsize-1 && !wallAbove(black->i, black->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    MOVE_UP(white); MOVE_RIGHT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
                    if (eval > max_eval) updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    MOVE_DOWN(white); MOVE_LEFT(white);
                }

                if (white->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // diagonally down - right
                {
                    MOVE_DOWN(white); MOVE_RIGHT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
                    if (eval > max_eval) updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    MOVE_UP(white); MOVE_LEFT(white);
                }
            }
        }
        else if (white->i == black->i && white->j-1 == black->j && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // black on the left of white
        {
            if (black->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))
            {
                MOVE_LEFT2(white);
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
                if (eval > max_eval) updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                MOVE_RIGHT2(white);
            }
            else
            {
                if (white->i < boardsize-1 && !wallAbove(black->i, black->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    MOVE_UP(white); MOVE_LEFT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
                    if (eval > max_eval) updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    MOVE_DOWN(white); MOVE_RIGHT(white);
                }
                if (white->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    MOVE_DOWN(white); MOVE_LEFT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
                    if (eval > max_eval) updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    MOVE_UP(white); MOVE_RIGHT(white);
                }
            }
        }
        else if (white->i-1 == black->i && white->j == black->j && !wallBelow(white->i, white->j, wall_matrix, boardsize))  // black below white
        {
            if (black->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize))
            {
                MOVE_DOWN2(white);
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
                if (eval > max_eval) updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                MOVE_UP2(white);
            }
            else
            {
                if (white->j < boardsize-1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))  // diagonally  down - right
                {
                    MOVE_DOWN(white); MOVE_RIGHT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
                    if (eval > max_eval) updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    MOVE_UP(white); MOVE_LEFT(white);   
                }
                if (white->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    MOVE_DOWN(white); MOVE_LEFT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
                    if (eval > max_eval) updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    MOVE_UP(white); MOVE_RIGHT(white);
                }
            }
        }     
        // normal moves
        if (white->i < boardsize-1 && !wallAbove(white->i, white->j, wall_matrix, boardsize) && !(white->i + 1 == black->i && white->j == black->j))  // up
        {
            MOVE_UP(white);
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
            if (eval > max_eval) updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
            MOVE_DOWN(white);
        }
        if (white->j < boardsize - 1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize) && !(white->i == black->i && white->j + 1 == black->j))  // right
        {
            MOVE_RIGHT(white);
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
            if (eval > max_eval) updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
            MOVE_LEFT(white);
        }
        if (white->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize) && !(white->i == black->i && white->j - 1 == black->j))  // left
        {
            MOVE_LEFT(white);
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
            if (eval > max_eval) updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
            MOVE_RIGHT(white);
        }
        if (white->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize) && !(white->i - 1 == black->i && white->j == black->j))  // down
        {
            MOVE_DOWN(white);
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo, range);
            if (eval > max_eval) updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
            MOVE_UP(white);
        }

        if (white->walls == 0) return evalMove;  // white has no walls left

        // -check wall placement-
        end = black->i + range;
        if (end > boardsize) end = boardsize;

        start = black->i - range;
        if (start < 1) start = 1;

        check_walls(wall_matrix, white, black, white, boardsize, depth, pseudo, start, end, max_time, &eval, &max_eval, t, &evalMove,
        compare_white, change_white, false, uwp_white, 'w');
    }
    else  // black plays
    {
        // check each possible move
        int min_eval = INFINITY;

        // -check pawn movement-

        // diagonal moves
        if (black->i-1 == white->i && black->j == white->j && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // white below black
        {
            if (white->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize))
            {
                MOVE_DOWN2(black);
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);
                if (eval < min_eval) updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                MOVE_UP2(black);
            }
            else
            {
                if (black->j < boardsize-1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))  // diagonally  down - right
                {
                    MOVE_DOWN(black); MOVE_RIGHT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);
                    if (eval < min_eval) updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    MOVE_UP(black); MOVE_LEFT(black);
                    
                }
                if (black->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    MOVE_DOWN(black); MOVE_LEFT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);
                    if (eval < min_eval) updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    MOVE_UP(black); MOVE_RIGHT(black);
                }
            }
        }
        else if (black->i == white->i && black->j-1 == white->j && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // white on the left of black
        {
            if (white->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))
            {
                MOVE_LEFT2(black);
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);
                if (eval < min_eval) updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                MOVE_RIGHT2(black);
            }
            else
            {
                if (black->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    MOVE_DOWN(black); MOVE_LEFT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);
                    if (eval < min_eval) updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    MOVE_UP(black); MOVE_RIGHT(black);
                }
                
                if (black->i < boardsize - 1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    MOVE_UP(black); MOVE_LEFT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);
                    if (eval < min_eval) updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    MOVE_DOWN(black); MOVE_RIGHT(black);
                }
            }
        }
        else if (black->i == white->i && black->j+1 == white->j && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))  // white on the right of black
        {
            if (white->j < boardsize - 1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))
            {
                MOVE_RIGHT2(black);
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);
                if (eval < min_eval) updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                MOVE_LEFT2(black);
            }
            else
            {
                if (black->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize))  // diagonally down - right
                {
                    MOVE_DOWN(black); MOVE_RIGHT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);
                    if (eval < min_eval) updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    MOVE_UP(black); MOVE_LEFT(black); 
                }
                if (black->i < boardsize - 1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    MOVE_UP(black); MOVE_RIGHT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);
                    if (eval < min_eval) updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    MOVE_DOWN(black); MOVE_LEFT(black);   
                }
            }
        }
        else if (black->i+1 == white->i && black->j == white->j && !wallAbove(black->i, black->j, wall_matrix, boardsize))  // white above black
        {
            if (white->i < boardsize - 1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))
            {
                MOVE_UP2(black);
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);
                if (eval < min_eval) updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                MOVE_DOWN2(black);
            }
            else
            {
                if (black->j < boardsize-1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    MOVE_UP(black); MOVE_RIGHT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);
                    if (eval < min_eval) updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    MOVE_DOWN(black); MOVE_LEFT(black);
                }
                if (black->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    MOVE_UP(black); MOVE_LEFT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);

                    if (eval < min_eval) updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    MOVE_DOWN(black); MOVE_RIGHT(black);
                }
            }
        }
        // check normal moves
        if (black->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize) && !(black->i - 1 == white->i && black->j == white->j))  // down
        {
            MOVE_DOWN(black);
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);
            if (eval < min_eval) updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
            MOVE_UP(black);
        }
        if (black->j < boardsize-1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize) && !(black->i == white->i && black->j + 1 == white->j))  // right
        {
            MOVE_RIGHT(black);
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);
            if (eval < min_eval) updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
            MOVE_LEFT(black);
        }
        if (black->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize) && !(black->i == white->i && black->j - 1 == white->j))  // left
        {
            MOVE_LEFT(black);
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);
            if (eval < min_eval) updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
            MOVE_RIGHT(black);
        }      
        if (black->i < boardsize - 1 && !wallAbove(black->i, black->j, wall_matrix, boardsize) && !(black->i + 1 == white->i && black->j == white->j))  // up
        {
            MOVE_UP(black);
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo, range);
            if (eval < min_eval) updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
            MOVE_DOWN(black);
        }

        // -check wall placement-
        if (black->walls == 0) return evalMove;  // black has no walls left
        
        start = white->i + range;
        if (start > boardsize-1) start = boardsize-1;

        end = white->i - range;
        if (end < 1) end = 1;

        check_walls(wall_matrix, white, black, black, boardsize, depth, pseudo, start, end, max_time, &eval, &min_eval, t, &evalMove, 
        compare_black, change_black, true, uwp_black, 'b');
    }

    return evalMove;
}

/* sources: https://en.wikipedia.org/wiki/Minimax
            https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning */

const char wall_types[] = {'b', 'r'};
int minimax(char** wall_matrix, cint boardsize, const small_int depth, int alpha, int beta, player* white, player* black, char Maximizing, cchar pseudo, cint range)
{
    if (depth == 0 || white->i == boardsize-1 || black->i == 0)  // Base case
        return positionEvaluation(*black, *white, boardsize, wall_matrix);
    
    int eval;
    if (Maximizing)  // Maximizing - White
    {
        // check each possible move
        int max_eval = NEG_INFINITY;
    
        // -check pawn movement-
        
        // diagonal moves
        if (white->i+1 == black->i && white->j == black->j && !wallAbove(white->i, white->j, wall_matrix, boardsize))  // black above white
        {
            if (black->i < boardsize-1 && !wallAbove(black->i, black->j, wall_matrix, boardsize))
            {
                MOVE_UP2(white);
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
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
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
                    max_eval = MAX(max_eval, eval);
                    alpha = MAX(alpha, eval);
                    MOVE_DOWN(white); MOVE_LEFT(white);
                    if (beta <= alpha) return max_eval; 
                }

                if (white->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    MOVE_UP(white); MOVE_LEFT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
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
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
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
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
                    max_eval = MAX(max_eval, eval);
                    alpha = MAX(alpha, eval);
                    MOVE_DOWN(white); MOVE_LEFT(white);
                    if (beta <= alpha) return max_eval; 
                }

                if (white->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // diagonally down - right
                {
                    MOVE_DOWN(white); MOVE_RIGHT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
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
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
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
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
                    max_eval = MAX(max_eval, eval);
                    alpha = MAX(alpha, eval);
                    MOVE_DOWN(white); MOVE_RIGHT(white);
                    if (beta <= alpha) return max_eval; 
                }

                if (white->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    MOVE_DOWN(white); MOVE_LEFT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
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
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
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
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
                    max_eval = MAX(max_eval, eval);
                    alpha = MAX(alpha, eval);
                    MOVE_UP(white); MOVE_LEFT(white);
                    if (beta <= alpha) return max_eval; 
                }

                if (white->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    MOVE_DOWN(white); MOVE_LEFT(white);
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
                    max_eval = MAX(max_eval, eval);
                    alpha = MAX(alpha, eval);
                    MOVE_UP(white); MOVE_RIGHT(white);
                    if (beta <= alpha) return max_eval; 
                }
            }
        }
        // normal moves
        if (white->i < boardsize-1 && !wallAbove(white->i, white->j, wall_matrix, boardsize) && !(white->i + 1 == black->i && white->j == black->j))  // up
        {
            MOVE_UP(white);
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
            max_eval = MAX(max_eval, eval);
            alpha = MAX(alpha, eval);
            MOVE_DOWN(white);
            if (beta <= alpha) return max_eval;
        }
        if (white->j < boardsize - 1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize) && !(white->i == black->i && white->j + 1 == black->j))  // right
        {
            MOVE_RIGHT(white);
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
            max_eval = MAX(max_eval, eval);
            alpha = MAX(alpha, eval);
            MOVE_LEFT(white);
            if (beta <= alpha) return max_eval;
        }    
        if (white->j > 0 &&!wallOnTheLeft(white->i, white->j, wall_matrix, boardsize) && !(white->i == black->i && white->j - 1 == black->j))  // left
        {
            MOVE_LEFT(white);
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
            max_eval = MAX(max_eval, eval);
            alpha = MAX(alpha, eval);
            MOVE_RIGHT(white);
            if (beta <= alpha) return max_eval;
        } 
        if (white->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize) && !(white->i - 1 == black->i && white->j == black->j))  // down
        {
            MOVE_DOWN(white);
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
            max_eval = MAX(max_eval, eval);
            alpha = MAX(alpha, eval);
            MOVE_UP(white);
            if (beta <= alpha) return max_eval;
        }
        
        // -check wall placement-
        if (white->walls == 0) return max_eval;  // white has no walls left
        
        int start, end;
        char path_found;

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
            end = black->i + range;
            if (end > boardsize) end = boardsize;

            start = black->i - range;
            if (start < 1) start = 1;
        }
        
        
        for (int i = start; i < end; i++)
        {
            for (int j = 0; j < boardsize-1; j++)
            {
                for (small_int k = 0; k < 2; k++)
                {
                    if (!thereIsAWall(wall_types[k], wall_matrix, boardsize, i, j))
                    {
                        placeWall(white, wall_matrix, wall_types[k], i, j);
                        path_found = false;
                        if (there_is_a_path(wall_matrix, boardsize, white, black))
                        {
                            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo, range);
                            path_found = true;
                            max_eval = MAX(max_eval, eval);
                            alpha = MAX(alpha, eval);
                            resetWallPlacement(white, wall_matrix, i, j);  // reset placing the wall
                            if (beta <= alpha) return max_eval;
                        }
                        if(!path_found) resetWallPlacement(white, wall_matrix, i, j);  // reset placing the wall
                    }
                }
            }
        }
        return max_eval;
    }
    else  // Minimizing - Black
    {
        // check each possible move

        // check pawn movement
        int min_eval = INFINITY;

        // diagonal moves
        if (black->i-1 == white->i && black->j == white->j && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // white below black
        {
            if (white->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize))
            {
                MOVE_DOWN2(black);
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
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
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
                    min_eval = MIN(min_eval, eval);
                    beta = MIN(beta, eval);
                    MOVE_UP(black); MOVE_LEFT(black);
                    if (beta <= alpha) return min_eval; 
                }

                if (black->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    MOVE_DOWN(black); MOVE_LEFT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
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
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
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
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
                    min_eval = MIN(min_eval, eval);
                    beta = MIN(beta, eval);
                    MOVE_UP(black); MOVE_RIGHT(black);
                    if (beta <= alpha) return min_eval; 
                }
                
                if (black->i < boardsize - 1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    MOVE_UP(black); MOVE_LEFT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
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
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
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
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
                    min_eval = MIN(min_eval, eval);
                    beta = MIN(beta, eval);
                    MOVE_UP(black); MOVE_LEFT(black);
                    if (beta <= alpha) return min_eval; 
                }

                if (black->i < boardsize-1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    MOVE_UP(black); MOVE_RIGHT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
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
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
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
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
                    min_eval = MIN(min_eval, eval);
                    beta = MIN(beta, eval);
                    MOVE_DOWN(black); MOVE_LEFT(black);
                    if (beta <= alpha) return min_eval; 
                }
                if (black->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    MOVE_UP(black); MOVE_LEFT(black);
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
                    min_eval = MIN(min_eval, eval);
                    beta = MIN(beta, eval);
                    MOVE_DOWN(black); MOVE_RIGHT(black);
                    if (beta <= alpha) return min_eval; 
                }
            }
        }
        // normal moves
        if (black->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize) && !(black->i - 1 == white->i && black->j == white->j))  // down
        {
            MOVE_DOWN(black);
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
            min_eval = MIN(min_eval, eval);
            beta = MIN(beta, eval);
            MOVE_UP(black);
            if (beta <= alpha) return min_eval;
        }
        if (black->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize) && !(black->i == white->i && black->j - 1 == white->j))  // left
        {
            MOVE_LEFT(black);
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
            min_eval = MIN(min_eval, eval);
            beta = MIN(beta, eval);
            MOVE_RIGHT(black);
            if (beta <= alpha) return min_eval;
        }
        if (black->j < boardsize-1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize) && !(black->i == white->i && black->j + 1 == white->j))  // right
        {
            MOVE_RIGHT(black);
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
            min_eval = MIN(min_eval, eval);
            beta = MIN(beta, eval);
            MOVE_LEFT(black);
            if (beta <= alpha) return min_eval;
        }      
        if (black->i < boardsize-1 && !wallAbove(black->i, black->j, wall_matrix, boardsize) && !(black->i + 1 == white->i && black->j == white->j))  // up
        {
            MOVE_UP(black);
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
            min_eval = MIN(min_eval, eval);
            beta = MIN(beta, eval);
            MOVE_DOWN(black);
            if (beta <= alpha) return min_eval;
        }

        // -check wall placement-
        if (black->walls == 0) return min_eval;  // black has no walls left

        int start, end;
        char path_found;

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
            start = white->i + range;
            if (start > boardsize-1) start = boardsize-1;

            end = white->i - range;
            if (end < 0) end = 0;
        }

        for (int i = start; i > end; i--)
        {
            for (int j = 0; j < boardsize-1; j++)
            {
                for (small_int k = 0; k < 2; k++)
                {
                    if (!thereIsAWall(wall_types[k], wall_matrix, boardsize, i, j))
                    {
                        path_found = false;
                        placeWall(black, wall_matrix, wall_types[k], i, j);
                        if (there_is_a_path_black(wall_matrix, boardsize, white, black))
                        {
                            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo, range);
                            path_found = true;
                            min_eval = MIN(min_eval, eval);
                            beta = MIN(beta, eval);
                            resetWallPlacement(black, wall_matrix, i, j);
                            if (beta <= alpha) return min_eval;
                        }
                        if (!path_found) resetWallPlacement(black, wall_matrix, i, j);  // reset placing the wall
                    }
                }
            }
        }
        return min_eval;
    }
}
