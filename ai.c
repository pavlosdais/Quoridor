#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "utilities.h"

// Function Prototypes
int minimax(char** wall_matrix, int boardsize, unsigned char depth, int alpha, int beta, player* white, player* black, char Maximizing, char pseudo);
void updateWallWallPlacementWhite(returningMove* evalMove, char orientation, int i, int j, int* eval, int* max_eval);
void updateWallWallPlacementBlack(returningMove* evalMove, char orientation, int i, int j, int* eval, int* min_eval);
void updatePawnMovementWhite(player* white, returningMove* evalMove, int* eval, int* max_eval);
void updatePawnMovementBlack(player* black, returningMove* evalMove, int* eval, int* min_eval);
void placeWall(player* pl, char** wall_matrix, char orientation, int i, int j);
void resetWallPlacement(player* pl, char** wall_matrix, int i, int j, char en);
void updateEvalWhite(int* max_eval, int* eval, int* alpha);
void updateEvalBlack(int* max_eval, int* eval, int* beta);

/* For each potential move the corresponding player has, search the evaluated best responses for both players up to a certain
depth, or moves ahead, and return the move which is believed to be the most advantageous for the player who's playing assuming
optimal play for both sides. */

returningMove bestMove(char** wall_matrix, int boardsize, char pl, player* black, player* white, unsigned char depth, char pseudo)
{
    int eval;
    returningMove evalMove;
    // check pawn movement
    int max_eval = NEG_INFINITY;
    if (pl == 'w')  // white plays
    {
        // check each possible move

        // check pawn movement
        if (white->i+1 == black->i && white->j == black->j && !wallAbove(white->i, white->j, wall_matrix, boardsize)) // black above white
        {
            if (black->i < boardsize - 1 && !wallAbove(black->i, black->j, wall_matrix, boardsize))
            {
                (white->i)+=2;
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo);
                if (eval == INFINITY)  // allocation failure
                {
                    evalMove.move = -1;
                    return evalMove;
                }
                updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                (white->i)-=2;  // reset movement
            }
            else
            {
                if (white->j < boardsize-1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    ++white->i;
                    ++white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo);
                    if (eval == INFINITY)  // allocation failure
                    {
                        evalMove.move = -1;
                        return evalMove;
                    }
                    updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    --white->i;  // reset movement
                    --white->j;
                }

                if (white->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    ++white->i;
                    --white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo);
                    if (eval == INFINITY)  // allocation failure
                    {
                        evalMove.move = -1;
                        return evalMove;
                    }
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
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo);
                if (eval == INFINITY)  // allocation failure
                {
                    evalMove.move = -1;
                    return evalMove;
                }
                updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                (white->j)-=2;  // reset movement
            }
            else
            {
                if (white->i < boardsize-1 && !wallAbove(black->i, black->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    ++white->i;
                    ++white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo);
                    if (eval == INFINITY)  // allocation failure
                    {
                        evalMove.move = -1;
                        return evalMove;
                    }
                    updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    --white->i;  // reset movement
                    --white->j;
                }

                if (white->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // diagonally down - right
                {
                    --white->i;
                    ++white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo);
                    if (eval == INFINITY)  // allocation failure
                    {
                        evalMove.move = -1;
                        return evalMove;
                    }
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
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo);
                if (eval == INFINITY)  // allocation failure
                {
                    evalMove.move = -1;
                    return evalMove;
                }
                updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                (white->j)+=2;  // reset movement
            }
            else
            {
                if (white->i < boardsize-1 && !wallAbove(black->i, black->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    ++white->i;
                    --white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo);
                    if (eval == INFINITY)  // allocation failure
                    {
                        evalMove.move = -1;
                        return evalMove;
                    }
                    updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    --white->i;  // reset movement
                    ++white->j;
                }

                if (white->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    --white->i;
                    --white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo);
                    if (eval == INFINITY)  // allocation failure
                    {
                        evalMove.move = -1;
                        return evalMove;
                    }
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
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo);
                if (eval == INFINITY)  // allocation failure
                {
                    evalMove.move = -1;
                    return evalMove;
                }
                updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                (white->i)+=2;  // reset movement
            }
            else
            {
                if (white->j < boardsize-1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))  // diagonally  down - right
                {
                    --white->i;
                    ++white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo);
                    if (eval == INFINITY)  // allocation failure
                    {
                        evalMove.move = -1;
                        return evalMove;
                    }
                    updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    ++white->i;  // reset movement
                    --white->j;
                }

                if (white->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    --white->i;
                    --white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo);
                    if (eval == INFINITY)  // allocation failure
                    {
                        evalMove.move = -1;
                        return evalMove;
                    }
                    updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
                    ++white->i;  // reset movement
                    ++white->j;
                }
            }
        }
        
        if (white->i < boardsize - 1 && !wallAbove(white->i, white->j, wall_matrix, boardsize) && !(white->i + 1 == black->i && white->j == black->j))  // up
        {
            ++white->i;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo);
            if (eval == INFINITY)  // allocation failure
            {
                evalMove.move = -1;
                return evalMove;
            }
            updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
            --white->i;  // reset movement
        }

        if (white->j < boardsize - 1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize) && !(white->i == black->i && white->j + 1 == black->j))  // right
        {
            ++white->j;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo);
            if (eval == INFINITY)  // allocation failure
            {
                evalMove.move = -1;
                return evalMove;
            }
            updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
            --white->j;  // reset movement
        }

        if (white->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize) && !(white->i == black->i && white->j - 1 == black->j))  // left
        {
            --white->j;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo);
            if (eval == INFINITY)  // allocation failure
            {
                evalMove.move = -1;
                return evalMove;
            }
            updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
            ++white->j;  // reset movement
        }
        
        if (white->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize) && !(white->i - 1 == black->i && white->j == black->j))  // down
        {
            --white->i;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo);
            if (eval == INFINITY)  // allocation failure
            {
                evalMove.move = -1;
                return evalMove;
            }
            updatePawnMovementWhite(white, &evalMove, &eval, &max_eval);
            ++white->i;  // reset movement
        }

        // check wall placement
        for (int i = 1; i < boardsize; i++)
        {
            if (white->walls == 0) break;
            for (int j = 0; j < boardsize-1; j++)
            {
                char path;
                if (!thereIsAWall('b', wall_matrix, boardsize, i, j))
                {
                    placeWall(white, wall_matrix, 'b', i, j);  // place horizontal wall
                    path = there_is_a_path(wall_matrix, boardsize, white, black);
                    if (path == true)
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo);
                        if (eval == INFINITY)  // allocation failure
                        {
                            evalMove.move = -1;
                            return evalMove;
                        }
                        updateWallWallPlacementWhite(&evalMove, 'b', i, j, &eval, &max_eval);
                    }
                    else if (path == 2)  // allocation problem
                    {
                        evalMove.move = -1;
                        return evalMove;
                    }
                    resetWallPlacement(white, wall_matrix, i, j, 0);
                }
                
                if (!thereIsAWall('r', wall_matrix, boardsize, i, j))
                {
                    placeWall(white, wall_matrix, 'r', i, j);  // place vertical wall
                    path = there_is_a_path(wall_matrix, boardsize, white, black);
                    if (path == true)
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false, pseudo);
                        if (eval == INFINITY)  // allocation failure
                        {
                            evalMove.move = -1;
                            return evalMove;
                        }
                        updateWallWallPlacementWhite(&evalMove, 'r', i, j, &eval, &max_eval);
                    }
                    else if (path == 2)
                    {
                        evalMove.move = -1;
                        return evalMove;
                    }
                    resetWallPlacement(white, wall_matrix, i, j, 0);
                } 
            }
        }
    }
    else  // black plays
    {
        int min_eval = INFINITY;
        // check each possible move

        // check pawn movement
        if (black->i-1 == white->i && black->j == white->j && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // white below black
        {
            if (white->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize))
            {
                (black->i)-=2;
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo);
                if (eval == INFINITY)  // allocation failure
                {
                    evalMove.move = -1;
                    return evalMove;
                }
                updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                (black->i)+=2;  // reset movement
            }
            else
            {
                if (black->j < boardsize-1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))  // diagonally  down - right
                {
                    --black->i;
                    ++black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo);
                    if (eval == INFINITY)  // allocation failure
                    {
                        evalMove.move = -1;
                        return evalMove;
                    }
                    updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    ++black->i;  // reset movement
                    --black->j;
                }

                if (black->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    --black->i;
                    --black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo);
                    if (eval == INFINITY)  // allocation failure
                    {
                        evalMove.move = -1;
                        return evalMove;
                    }
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
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo);
                if (eval == INFINITY)  // allocation failure
                {
                    evalMove.move = -1;
                    return evalMove;
                }
                updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                (black->j)+=2;  // reset movement
            }
            else
            {
                if (black->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    --black->i;
                    --black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo);
                    if (eval == INFINITY)  // allocation failure
                    {
                        evalMove.move = -1;
                        return evalMove;
                    }
                    updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    ++black->i;  // reset movement
                    ++black->j; 
                }
                
                if (black->i < boardsize - 1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    ++black->i;
                    --black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo);
                    if (eval == INFINITY)  // allocation failure
                    {
                        evalMove.move = -1;
                        return evalMove;
                    }
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
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo);
                if (eval == INFINITY)  // allocation failure
                {
                    evalMove.move = -1;
                    return evalMove;
                }
                updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                (black->j)-=2;  // reset movement
            }
            else
            {
                if (black->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize))  // diagonally down - right
                {
                    --black->i;
                    ++black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo);
                    if (eval == INFINITY)  // allocation failure
                    {
                        evalMove.move = -1;
                        return evalMove;
                    }
                    updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    ++black->i;  // reset movement
                    --black->j; 
                }

                if (black->i < boardsize - 1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    ++black->i;
                    ++black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo);
                    if (eval == INFINITY)  // allocation failure
                    {
                        evalMove.move = -1;
                        return evalMove;
                    }
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
                eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo);
                if (eval == INFINITY)  // allocation failure
                {
                    evalMove.move = -1;
                    return evalMove;
                }
                updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                (black->i)-=2;  // reset movement
            }
            else
            {
                if (black->j < boardsize-1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    ++black->i;
                    ++black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo);
                    if (eval == INFINITY)  // allocation failure
                    {
                        evalMove.move = -1;
                        return evalMove;
                    }
                    updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    --black->i;  // reset movement
                    --black->j; 
                }

                if (black->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    ++black->i;
                    --black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo);
                    if (eval == INFINITY)  // allocation failure
                    {
                        evalMove.move = -1;
                        return evalMove;
                    }
                    updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
                    --black->i;  // reset movement
                    ++black->j;
                }
            }
        }
        if (black->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize) && !(black->i - 1 == white->i && black->j == white->j))  // down
        {
            --black->i;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo);
            if (eval == INFINITY)  // allocation failure
            {
                evalMove.move = -1;
                return evalMove;
            }
            updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
            ++black->i;  // reset movement
        }
        
        if (black->j < boardsize-1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize) && !(black->i == white->i && black->j + 1 == white->j))  // right
        {
            ++black->j;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo);
            if (eval == INFINITY)  // allocation failure
            {
                evalMove.move = -1;
                return evalMove;
            }
            updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
            --black->j;  // reset movement
        }

        if (black->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize) && !(black->i == white->i && black->j - 1 == white->j))  // left
        {
            --black->j;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo);
            if (eval == INFINITY)  // allocation failure
            {
                evalMove.move = -1;
                return evalMove;
            }
            updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
            ++black->j;  // reset movement
        }
        
        if (black->i < boardsize - 1 && !wallAbove(black->i, black->j, wall_matrix, boardsize) && !(black->i + 1 == white->i && black->j == white->j))  // up
        {
            ++black->i;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo);
            if (eval == INFINITY)  // allocation failure
            {
                evalMove.move = -1;
                return evalMove;
            }
            updatePawnMovementBlack(black, &evalMove, &eval, &min_eval);
            --black->i;  // reset movement
        }

        // check wall placement
        for (int i = boardsize-1; i > 0; i--)
        {
            if (black->walls == 0) break;
            for (int j = 0; j < boardsize-1; j++)
            {
                char path;
                if (!thereIsAWall('b', wall_matrix, boardsize, i, j))
                {
                    placeWall(black, wall_matrix, 'b', i, j);  // place horizontal wall
                    path = there_is_a_path(wall_matrix, boardsize, white, black);
                    if (path == true)
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo);
                        if (eval == INFINITY)  // allocation failure
                        {
                            evalMove.move = -1;
                            return evalMove;
                        }
                        updateWallWallPlacementBlack(&evalMove, 'b', i, j, &eval, &min_eval);
                    }
                    else if (path == 2)
                    {
                        evalMove.move = -1;
                        return evalMove;
                    }
                    resetWallPlacement(black, wall_matrix, i, j, 0);
                }
                if (!thereIsAWall('r', wall_matrix, boardsize, i, j))
                {
                    placeWall(black, wall_matrix, 'r', i, j);  // place vertical wall
                    path = there_is_a_path(wall_matrix, boardsize, white, black);
                    if (path == true)
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true, pseudo);
                        if (eval == INFINITY)  // allocation failure
                        {
                            evalMove.move = -1;
                            return evalMove;
                        }
                        updateWallWallPlacementBlack(&evalMove, 'r', i, j, &eval, &min_eval);
                    }
                    else if (path == 2)
                    {
                        evalMove.move = -1;
                        return evalMove;
                    }
                    resetWallPlacement(black, wall_matrix, i, j, 0);
                }
            }
        }
    }
    return evalMove;
}

/* sources: https://en.wikipedia.org/wiki/Minimax
            https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning */

int minimax(char** wall_matrix, int boardsize, unsigned char depth, int alpha, int beta, player* white, player* black, char Maximizing, char pseudo)
{
    if (depth == 0 || white->i == boardsize-1 || black->i == 0)  // Base case
    {
        // Evaluate the position
        int positionEval;
        if(!positionEvaluation(*black, *white, boardsize, wall_matrix, &positionEval))
        {
            printf("? allocation failure\n\n");
            fflush(stdout);
            return INFINITY;
        }
        return positionEval;
    }

    int eval;
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
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo);
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
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo);
                    updateEvalWhite(&max_eval, &eval, &alpha);
                    --white->i;  // reset movement
                    --white->j;
                    if (beta <= alpha) return max_eval; 
                }

                if (white->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    ++white->i;
                    --white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo);
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
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo);
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
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo);
                    updateEvalWhite(&max_eval, &eval, &alpha);
                    --white->i;  // reset movement
                    --white->j;
                    if (beta <= alpha) return max_eval; 
                }

                if (white->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // diagonally down - right
                {
                    --white->i;
                    ++white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo);
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
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo);
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
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo);
                    updateEvalWhite(&max_eval, &eval, &alpha);
                    --white->i;  // reset movement
                    ++white->j;
                    if (beta <= alpha) return max_eval; 
                }

                if (white->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    --white->i;
                    --white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo);
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
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo);
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
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo);
                    updateEvalWhite(&max_eval, &eval, &alpha);
                    ++white->i;  // reset movement
                    --white->j;
                    if (beta <= alpha) return max_eval; 
                }

                if (white->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    --white->i;
                    --white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo);
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
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo);
            updateEvalWhite(&max_eval, &eval, &alpha);
            --white->i;  // reset movement
            if (beta <= alpha) return max_eval;
        }

        if (white->j < boardsize - 1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize) && !(white->i == black->i && white->j + 1 == black->j))  // right
        {
            ++white->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo);
            updateEvalWhite(&max_eval, &eval, &alpha);
            --white->j;  // reset movement
            if (beta <= alpha) return max_eval;
        }
        
        if (white->j > 0 &&!wallOnTheLeft(white->i, white->j, wall_matrix, boardsize) && !(white->i == black->i && white->j - 1 == black->j))  // left
        {
            --white->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo);
            updateEvalWhite(&max_eval, &eval, &alpha);
            ++white->j;  // reset movement
            if (beta <= alpha) return max_eval;
        }
     
        if (white->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize) && !(white->i - 1 == black->i && white->j == black->j))  // down
        {
            --white->i;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo);
            updateEvalWhite(&max_eval, &eval, &alpha);
            ++white->i;  // reset movement
            if (beta <= alpha) return max_eval;
        }
        
		int st = 1;
		int end = boardsize;
        // last search of a pseudodepth check, only check wall placement near black's pawn
		if (depth == 1 && pseudo == 1)
		{
			if (black->i > 1)
			{
				st = black->i-1;
				if (black->i < boardsize-1) end = black->i+1;
			}
			else if (black->i > 2)
			{
				st = black->i-2;
				end = black->i;
			}
		}
        // check wall placement
        for (int i = st; i < end; i++)
        {
            if (beta <= alpha || white->walls == 0) break;
            for (int j = 0; j < boardsize-1; j++)
            {
                if (white->walls == 0) break;
                if (!thereIsAWall('b', wall_matrix, boardsize, i, j))
                {
                    placeWall(white, wall_matrix, 'b', i, j);  // place horizontal wall
                    en = 0;
                    if (there_is_a_path(wall_matrix, boardsize, white, black))
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo);
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
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false, pseudo);
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
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo);
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
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo);
                    updateEvalBlack(&min_eval, &eval, &beta);
                    ++black->i;  // reset movement
                    --black->j;
                    if (beta <= alpha) return min_eval; 
                }

                if (black->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // diagonally down - left
                {
                    --black->i;
                    --black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo);
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
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo);
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
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo);
                    updateEvalBlack(&min_eval, &eval, &beta);
                    ++black->i;  // reset movement
                    ++black->j;
                    if (beta <= alpha) return min_eval; 
                }
                
                if (black->i < boardsize - 1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    ++black->i;
                    --black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo);
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
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo);
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
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo);
                    updateEvalBlack(&min_eval, &eval, &beta);
                    ++black->i;  // reset movement
                    --black->j;
                    if (beta <= alpha) return min_eval; 
                }

                if (black->i < boardsize-1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))  // diagonally up - right
                {
                    ++black->i;
                    ++black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo);
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
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo);
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
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo);
                    updateEvalBlack(&min_eval, &eval, &beta);
                    --black->i;  // reset movement
                    --black->j;
                    if (beta <= alpha) return min_eval; 
                }

                if (black->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  // diagonally up - left
                {
                    ++black->i;
                    --black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo);
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
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo);
            updateEvalBlack(&min_eval, &eval, &beta);
            ++black->i;  // reset movement
            if (beta <= alpha) return min_eval;
        }
     
        if (black->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize) && !(black->i == white->i && black->j - 1 == white->j))  // left
        {
            --black->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo);
            updateEvalBlack(&min_eval, &eval, &beta);
            ++black->j;  // reset movement
            if (beta <= alpha) return min_eval;
        }

        if (black->j < boardsize-1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize) && !(black->i == white->i && black->j + 1 == white->j))  // right
        {
            ++black->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo);
            updateEvalBlack(&min_eval, &eval, &beta);
            --black->j;  // reset movement
            if (beta <= alpha) return min_eval;
        }
        
        if (black->i < boardsize-1 && !wallAbove(black->i, black->j, wall_matrix, boardsize) && !(black->i + 1 == white->i && black->j == white->j))  // up
        {
            ++black->i;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo);
            updateEvalBlack(&min_eval, &eval, &beta);
            --black->i;  // reset movement
            if (beta <= alpha) return min_eval;
        }

        // check wall placement
		int st = boardsize-1;
		int end = 1;
        // last search of a pseudodepth check, only check wall placement near white's pawn
		if (depth == 1 && pseudo == 1)
		{
			if (white->i < boardsize-1)
			{
				st = white->i+1;
				if (white->i > 1) end = white->i-1;
			}
			else if (white->i < boardsize-2)
			{
				st = white->i+2;
				end = white->i;
			}
		}
        for (int i = st; i > end; i--)
        {
            if (beta <= alpha || black->walls == 0) break;
            for (int j = 0; j < boardsize-1; j++)
            {
                if (black->walls == 0) break;
                if (!thereIsAWall('b', wall_matrix, boardsize, i, j))
                {
                    en = 0;
                    placeWall(black, wall_matrix, 'b', i, j);  // place horizontal wall
                    if (there_is_a_path(wall_matrix, boardsize, white, black))
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo);
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
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true, pseudo);
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

void placeWall(player* pl, char** wall_matrix, char orientation, int i, int j)
{
    pl->walls--;
    wall_matrix[i][j] = orientation;
}

void resetWallPlacement(player* pl, char** wall_matrix, int i, int j, char en)
{
    if (!en) pl->walls++;
    wall_matrix[i][j] = 0;
}

void updateWallWallPlacementWhite(returningMove* evalMove, char orientation, int i, int j, int* eval, int* max_eval)
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

void updateWallWallPlacementBlack(returningMove* evalMove, char orientation, int i, int j, int* eval, int* min_eval)
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

void updateEvalWhite(int* max_eval, int* eval, int* alpha)
{
    *max_eval = *max_eval > *eval ? *max_eval:*eval;
    *alpha = *alpha > *eval ? *alpha:*eval;
}

void updateEvalBlack(int* min_eval, int* eval, int* beta)
{
    *min_eval = *min_eval < *eval ? *min_eval:*eval;
    *beta = *beta < *eval ? *beta:*eval;
}
