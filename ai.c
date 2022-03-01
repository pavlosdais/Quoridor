#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "helper_commands.h"

#define INFINITY INT_MAX
#define NEG_INFINITY INT_MIN
#define true 1
#define false 0

// Function Prototype
int minimax(char** wall_matrix, int boardsize, unsigned char depth, float alpha, float beta, player* white, player* black, char Maximizing);

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
    char move;  // w for wall at (x,y), m for move at (x,y) and -1 for error
    char or;  // orientation of the wall
}
returningMove;

/* For each potential move the corresponding player has, search the evaluated best responses for both players up to a certain
depth, or moves ahead, and return the move which is believed to be the most advantageous for the player who's playing assuming
optimal play for both sides. */

returningMove bestMove(char** wall_matrix, int boardsize, char pl, player* black, player* white, unsigned char depth)
{
    int eval;
    returningMove evalMove;

    if (pl == 'w')  // white plays
    {
        // check each possible move

        // check pawn movement
        int max_eval = NEG_INFINITY;
        if (white->i < boardsize - 1 && !wallAbove(white->i, white->j, wall_matrix, boardsize) && !(white->i + 1 == black->i && white->j == black->j))  // up
        {
            ++white->i;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false);
            if (eval == INFINITY)  // allocation failure
            {
                evalMove.move = -1;
                return evalMove;
            }
            if (eval > max_eval)
            {
                max_eval = eval;
                evalMove.move = 'm';
                evalMove.x = white->i;
                evalMove.y = white->j;
            }
            --white->i;  // reset movement
        }

        if (white->j < boardsize - 1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize) && !(white->i == black->i && white->j + 1 == black->j))  // right
        {
            ++white->j;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false);
            if (eval == INFINITY)  // allocation failure
            {
                evalMove.move = -1;
                return evalMove;
            }
            if (eval > max_eval)
            {
                max_eval = eval;
                evalMove.move = 'm';
                evalMove.x = white->i;
                evalMove.y = white->j;
            }
            --white->j;  // reset movement
        }

        if (white->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize) && !(white->i == black->i && white->j - 1 == black->j))  // left
        {
            --white->j;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false);
            if (eval == INFINITY)  // allocation failure
            {
                evalMove.move = -1;
                return evalMove;
            }
            if (eval > max_eval)
            {
                max_eval = eval;
                evalMove.move = 'm';
                evalMove.x = white->i;
                evalMove.y = white->j;
            }
            ++white->j;  // reset movement
        }
        
        if (white->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize) && !(white->i - 1 == black->i && white->j == black->j))  // down
        {
            --white->i;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false);
            if (eval == INFINITY)  // allocation failure
            {
                evalMove.move = -1;
                return evalMove;
            }
            if (eval > max_eval)
            {
                max_eval = eval;
                evalMove.move = 'm';
                evalMove.x = white->i;
                evalMove.y = white->j;
            }
            ++white->i;  // reset movement
        }

        // check wall placement
        for (int i = 1; i < boardsize; i++)
        {
            if (white->walls == 0) break;
            for (int j = 0; j < boardsize-1; j++)
            {
                if (!thereIsAWall('b', wall_matrix, boardsize, i, j))
                {
                    wall_matrix[i][j] = 'b';  // place horizontal wall
                    white->walls--;
                    if (there_is_a_path(wall_matrix, boardsize, white, black))
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false);
                        if (eval == INFINITY)  // allocation failure
                        {
                            evalMove.move = -1;
                            return evalMove;
                        }
                        if (eval > max_eval)
                        {
                            max_eval = eval;
                            evalMove.move = 'w';
                            evalMove.or = 'b';
                            evalMove.x = i;
                            evalMove.y = j;
                        }
                    }
                    // reset placement
                    white->walls++;
                    wall_matrix[i][j] = 0;
                }
                
                if (!thereIsAWall('r', wall_matrix, boardsize, i, j))
                {
                    wall_matrix[i][j] = 'r';  // place vertical wall
                    white->walls--;
                    if (there_is_a_path(wall_matrix, boardsize, white, black))
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, false);
                        if (eval == INFINITY)  // allocation failure
                        {
                            evalMove.move = -1;
                            return evalMove;
                        }
                        if (eval > max_eval)
                        {
                            max_eval = eval;
                            evalMove.move = 'w';
                            evalMove.or = 'r';
                            evalMove.x = i;
                            evalMove.y = j;
                        }
                    }
                    // reset placement
                    white->walls++;
                    wall_matrix[i][j] = 0;
                } 
            }
        }
    }
    else  // black plays
    {
        int min_eval = INFINITY;
        // check each possible move

        // check pawn movement
        if (black->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize) && !(black->i - 1 == white->i && black->j == white->j))  // down
        {
            --black->i;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true);
            if (eval == INFINITY)  // allocation failure
            {
                evalMove.move = -1;
                return evalMove;
            }
            if (eval < min_eval)
            {
                min_eval = eval;
                evalMove.move = 'm';
                evalMove.x = black->i;
                evalMove.y = black->j;
            }
            ++black->i;  // reset movement
        }
        
        if (black->j < boardsize - 1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize) && !(black->i == white->i && black->j + 1 == white->j))  // right
        {
            ++black->j;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true);
            if (eval == INFINITY)  // allocation failure
            {
                evalMove.move = -1;
                return evalMove;
            }
            if (eval < min_eval)
            {
                min_eval = eval;
                evalMove.move = 'm';
                evalMove.x = black->i;
                evalMove.y = black->j;
            }
            --black->j;  // reset movement
        }

        if (black->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize) && !(black->i == white->i && black->j - 1 == white->j))  // left
        {
            --black->j;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true);
            if (eval == INFINITY)  // allocation failure
            {
                evalMove.move = -1;
                return evalMove;
            }
            if (eval < min_eval)
            {
                min_eval = eval;
                evalMove.move = 'm';
                evalMove.x = black->i;
                evalMove.y = black->j;
            }
            ++black->j;  // reset movement
        }
        
        if (black->i < boardsize - 1 && !wallAbove(black->i, black->j, wall_matrix, boardsize) && !(black->i + 1 == white->i && black->j == white->j))  // up
        {
            ++black->i;
            eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true);
            if (eval == INFINITY)  // allocation failure
            {
                evalMove.move = -1;
                return evalMove;
            }
            if (eval < min_eval)
            {
                min_eval = eval;
                evalMove.move = 'm';
                evalMove.x = black->i;
                evalMove.y = black->j;
            }
            --black->i;  // reset movement
        }

        // check wall placement
        for (int i = boardsize-1; i > 0; i--)
        {
            if (black->walls == 0) break;
            for (int j = 0; j < boardsize-1; j++)
            {
                if (!thereIsAWall('b', wall_matrix, boardsize, i, j))
                {
                    wall_matrix[i][j] = 'b';  // place horizontal wall
                    black->walls--;
                    if (there_is_a_path(wall_matrix, boardsize, white, black))
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true);
                        if (eval == INFINITY)  // allocation failure
                        {
                            evalMove.move = -1;
                            return evalMove;
                        }
                        if (eval < min_eval)
                        {
                            min_eval = eval;
                            evalMove.move = 'w';
                            evalMove.or = 'b';
                            evalMove.x = i;
                            evalMove.y = j;
                        }
                    }
                    // reset placement
                    black->walls++;
                    wall_matrix[i][j] = 0;
                }
                if (!thereIsAWall('r', wall_matrix, boardsize, i, j))
                {
                    wall_matrix[i][j] = 'r';  // place vertical wall 
                    black->walls--;
                    if (there_is_a_path(wall_matrix, boardsize, white, black))
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, NEG_INFINITY, INFINITY, white, black, true);
                        if (eval == INFINITY)  // allocation failure
                        {
                            evalMove.move = -1;
                            return evalMove;
                        }
                        if (eval < min_eval)
                        {
                            min_eval = eval;
                            evalMove.move = 'w';
                            evalMove.or = 'r';
                            evalMove.x = i;
                            evalMove.y = j;
                        }
                    }
                    // reset placement
                    wall_matrix[i][j] = 0;
                    black->walls++;
                }
            }
        }
    }
    return evalMove;
}

/* sources: https://en.wikipedia.org/wiki/Minimax
            https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning */

int minimax(char** wall_matrix, int boardsize, unsigned char depth, float alpha, float beta, player* white, player* black, char Maximizing)
{
    if (depth == 0 || white->i == boardsize-1 || black->i == 0)  // Base case
    {
        // Evaluate the position
        int positionEval;
        if(!positionEvaluation(black, white, boardsize, wall_matrix, &positionEval))
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

        if (white->i + 1 == black->i && white->j == black->j && !wallAbove(white->i, white->j, wall_matrix, boardsize)) //black above white
        {
            if (black->i < boardsize - 1 && !wallAbove(black->i, black->j, wall_matrix, boardsize))
            {
                (white->i)+=2;
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false);
                max_eval = max_eval > eval ? max_eval:eval;
                alpha = alpha > eval ? alpha:eval;
                (white->i)-=2;  // reset movement
                if (beta <= alpha) return max_eval;
            }
            else
            {
                if (white->j < boardsize - 1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))  //diagonally up - right
                {
                    ++white->i;
                    ++white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false);
                    max_eval = max_eval > eval ? max_eval:eval;
                    alpha = alpha > eval ? alpha:eval;
                    --white->i;  // reset movement
                    --white->j;
                    if (beta <= alpha) return max_eval; 
                }

                if (white->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  //diagonally up - left
                {
                    ++white->i;
                    --white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false);
                    max_eval = max_eval > eval ? max_eval:eval;
                    alpha = alpha > eval ? alpha:eval;
                    --white->i;  // reset movement
                    ++white->j;
                    if (beta <= alpha) return max_eval; 
                }
            }
        }
        else if (white->i == black->i && white->j + 1 == black->j && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize)) //black on the right of white
        {
            if (black->j < boardsize - 1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))
            {
                (white->j)+=2;
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false);
                max_eval = max_eval > eval ? max_eval:eval;
                alpha = alpha > eval ? alpha:eval;
                (white->j)-=2;  // reset movement
                if (beta <= alpha) return max_eval;
            }
            else
            {
                if (white->i < boardsize - 1 && !wallAbove(black->i, black->j, wall_matrix, boardsize))  //diagonally up - right
                {
                    ++white->i;
                    ++white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false);
                    max_eval = max_eval > eval ? max_eval:eval;
                    alpha = alpha > eval ? alpha:eval;
                    --white->i;  // reset movement
                    --white->j;
                    if (beta <= alpha) return max_eval; 
                }

                if (white->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize)) //diagonally down - right
                {
                    --white->i;
                    ++white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false);
                    max_eval = max_eval > eval ? max_eval:eval;
                    alpha = alpha > eval ? alpha:eval;
                    ++white->i;  // reset movement
                    --white->j;
                    if (beta <= alpha) return max_eval; 
                }
            }
        }
        else if (white->i == black->i && white->j - 1 == black->j && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize)) //black on the left of white
        {
            if (black->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))
            {
                (white->j)-=2;
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false);
                max_eval = max_eval > eval ? max_eval:eval;
                alpha = alpha > eval ? alpha:eval;
                (white->j)+=2;  // reset movement
                if (beta <= alpha) return max_eval;
            }
            else
            {
                if (white->i < boardsize - 1 && !wallAbove(black->i, black->j, wall_matrix, boardsize))  //diagonally up - left
                {
                    ++white->i;
                    --white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false);
                    max_eval = max_eval > eval ? max_eval:eval;
                    alpha = alpha > eval ? alpha:eval;
                    --white->i;  // reset movement
                    ++white->j;
                    if (beta <= alpha) return max_eval; 
                }

                if (white->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize)) //diagonally down - left
                {
                    --white->i;
                    --white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false);
                    max_eval = max_eval > eval ? max_eval:eval;
                    alpha = alpha > eval ? alpha:eval;
                    --white->i;  // reset movement
                    --white->j;
                    if (beta <= alpha) return max_eval; 
                }
            }
        }
        else if (white->i - 1 == black->i && white->j == black->j && !wallBelow(white->i, white->j, wall_matrix, boardsize)) //black below white
        {
            if (black->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize))
            {
                (white->i)-=2;
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false);
                max_eval = max_eval > eval ? max_eval:eval;
                alpha = alpha > eval ? alpha:eval;
                (white->i)+=2;  // reset movement
                if (beta <= alpha) return max_eval;
            }
            else
            {
                if (white->j < boardsize - 1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize))  //diagonally  down - right
                {
                    --white->i;
                    ++white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false);
                    max_eval = max_eval > eval ? max_eval:eval;
                    alpha = alpha > eval ? alpha:eval;
                    ++white->i;  // reset movement
                    --white->j;
                    if (beta <= alpha) return max_eval; 
                }

                if (white->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize))  //diagonally down - left
                {
                    --white->i;
                    --white->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false);
                    max_eval = max_eval > eval ? max_eval:eval;
                    alpha = alpha > eval ? alpha:eval;
                    --white->i;  // reset movement
                    --white->j;
                    if (beta <= alpha) return max_eval; 
                }
            }
        }

        if (white->i < boardsize - 1 && !wallAbove(white->i, white->j, wall_matrix, boardsize) && !(white->i + 1 == black->i && white->j == black->j)) // up
        {
            ++white->i;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false);
            max_eval = max_eval > eval ? max_eval:eval;
            alpha = alpha > eval ? alpha:eval;
            --white->i;  // reset movement
            if (beta <= alpha) return max_eval;
        }

        if (white->j < boardsize - 1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize) && !(white->i == black->i && white->j + 1 == black->j))  // right
        {
            ++white->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false);
            max_eval = max_eval > eval ? max_eval:eval;
            alpha = alpha > eval ? alpha:eval;
            --white->j;  // reset movement
            if (beta <= alpha) return max_eval;
        }
        
        if (white->j > 0 &&!wallOnTheLeft(white->i, white->j, wall_matrix, boardsize) && !(white->i == black->i && white->j - 1 == black->j))  // left
        {
            --white->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false);
            max_eval = max_eval > eval ? max_eval:eval;
            alpha = alpha > eval ? alpha:eval;
            ++white->j;  // reset movement
            if (beta <= alpha) return max_eval;
        }
     
        if (white->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize) && !(white->i - 1 == black->i && white->j == black->j))  // down
        {
            --white->i;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false);
            max_eval = max_eval > eval ? max_eval:eval;
            alpha = alpha > eval ? alpha:eval;
            ++white->i;  // reset movement
            if (beta <= alpha) return max_eval;
        }

        // check wall placement
        for (int i = 1; i < boardsize; i++)
        {
            //if (depth == 1) break;
            if (white->walls == 0 || beta <= alpha) break;
            for (int j = 0; j < boardsize-1; j++)
            {
                if (white->walls == 0) break;
                if (!thereIsAWall('b', wall_matrix, boardsize, i, j))
                {
                    white->walls--;
                    wall_matrix[i][j] = 'b';  // place horizontal wall
                    en = 0;
                    if (there_is_a_path(wall_matrix, boardsize, white, black))
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false);
                        en = 1;
                        max_eval = max_eval > eval ? max_eval:eval;
                        alpha = alpha > eval ? alpha:eval;
                        wall_matrix[i][j] = 0;
                        white->walls++;
                        if (beta <= alpha) return max_eval;
                    }
                    // reset placement
                    wall_matrix[i][j] = 0;
                    if (!en) white->walls++;
                }
                if (!thereIsAWall('r', wall_matrix, boardsize, i, j))
                {
                    white->walls--;
                    wall_matrix[i][j] = 'r';  // place vertical wall
                    en = 0;
                    if (there_is_a_path(wall_matrix, boardsize, white, black))
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, false);
                        en = 1;
                        max_eval = max_eval > eval ? max_eval:eval;
                        alpha = alpha > eval ? alpha:eval;
                        wall_matrix[i][j] = 0;
                        white->walls++;
                        
                        if (beta <= alpha) return max_eval;
                    }
                    // reset placement
                    wall_matrix[i][j] = 0;
                    if (!en) white->walls++;
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

        if (black->i - 1 == white->i && black->j == white->j && !wallBelow(black->i, black->j, wall_matrix, boardsize)) //white below black
        {
            if (white->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize))
            {
                (black->i)-=2;
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true);
                min_eval = min_eval < eval ? min_eval:eval;
                beta = beta < eval ? beta:eval;
                (black->i)+=2;  // reset movement
                if (beta <= alpha) return min_eval;
            }
            else
            {
                if (black->j < boardsize - 1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))  //diagonally  down - right
                {
                    --black->i;
                    ++black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true);
                    min_eval = min_eval < eval ? min_eval:eval;
                    beta = beta < eval ? beta:eval;
                    ++black->i;  // reset movement
                    --black->j;
                    if (beta <= alpha) return min_eval; 
                }

                if (black->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  //diagonally down - left
                {
                    --black->i;
                    --black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true);
                    min_eval = min_eval < eval ? min_eval:eval;
                    beta = beta < eval ? beta:eval;
                    --black->i;  // reset movement
                    --black->j;
                    if (beta <= alpha) return min_eval; 
                }
            }
        }
        else if (black->i == white->i && black->j - 1 == white->j && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize)) //white on the left of black
        {
            if (white->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))
            {
                (black->j)-=2;
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true);
                min_eval = min_eval < eval ? min_eval:eval;
                beta = beta < eval ? beta:eval;
                (black->j)+=2;  // reset movement
                if (beta <= alpha) return min_eval;
            }
            else
            {
                if (black->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize)) //diagonally down - left
                {
                    --black->i;
                    --black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true);
                    min_eval = min_eval < eval ? min_eval:eval;
                    beta = beta < eval ? beta:eval;
                    --black->i;  // reset movement
                    --black->j;
                    if (beta <= alpha) return min_eval; 
                }
                
                if (black->i < boardsize - 1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))  //diagonally up - left
                {
                    ++black->i;
                    --black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true);
                    min_eval = min_eval < eval ? min_eval:eval;
                    beta = beta < eval ? beta:eval;
                    --black->i;  // reset movement
                    ++black->j;
                    if (beta <= alpha) return min_eval; 
                }
            }
        }
        else if (black->i == white->i && black->j + 1 == white->j && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize)) //white on the right of black
        {
            if (white->j < boardsize - 1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))
            {
                (black->j)+=2;
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true);
                min_eval = min_eval < eval ? min_eval:eval;
                beta = beta < eval ? beta:eval;
                (black->j)-=2;  // reset movement
                if (beta <= alpha) return min_eval;
            }
            else
            {
                if (black->i > 0 && !wallBelow(white->i, white->j, wall_matrix, boardsize)) //diagonally down - right
                {
                    --black->i;
                    ++black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true);
                    min_eval = min_eval < eval ? min_eval:eval;
                    beta = beta < eval ? beta:eval;
                    ++black->i;  // reset movement
                    --black->j;
                    if (beta <= alpha) return min_eval; 
                }

                if (black->i < boardsize - 1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))  //diagonally up - right
                {
                    ++black->i;
                    ++black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true);
                    min_eval = min_eval < eval ? min_eval:eval;
                    beta = beta < eval ? beta:eval;
                    --black->i;  // reset movement
                    --black->j;
                    if (beta <= alpha) return min_eval; 
                }
            }
        }
        else if (black->i + 1 == white->i && black->j == white->j && !wallAbove(black->i, black->j, wall_matrix, boardsize)) //white above black
        {
            if (white->i < boardsize - 1 && !wallAbove(white->i, white->j, wall_matrix, boardsize))
            {
                (black->i)+=2;
                eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true);
                min_eval = min_eval < eval ? min_eval:eval;
                beta = beta < eval ? beta:eval;
                (black->i)-=2;  // reset movement
                if (beta <= alpha) return min_eval;
            }
            else
            {
                if (black->j < boardsize - 1 && !wallOnTheRight(white->i, white->j, wall_matrix, boardsize))  //diagonally up - right
                {
                    ++black->i;
                    ++black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true);
                    min_eval = min_eval < eval ? min_eval:eval;
                    beta = beta < eval ? beta:eval;
                    --black->i;  // reset movement
                    --black->j;
                    if (beta <= alpha) return min_eval; 
                }

                if (black->j > 0 && !wallOnTheLeft(white->i, white->j, wall_matrix, boardsize))  //diagonally up - left
                {
                    ++black->i;
                    --black->j;
                    eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true);
                    min_eval = min_eval < eval ? min_eval:eval;
                    beta = beta < eval ? beta:eval;
                    --black->i;  // reset movement
                    ++black->j;
                    if (beta <= alpha) return min_eval; 
                }
            }
        }
     
        if (black->i > 0 && !wallBelow(black->i, black->j, wall_matrix, boardsize) && !(black->i - 1 == white->i && black->j == white->j))  // down
        {
            --black->i;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true);
            min_eval = min_eval < eval ? min_eval:eval;
            beta = beta < eval ? beta:eval;
            ++black->i;  // reset movement
            if (beta <= alpha) return min_eval;
        }
     
        if (black->j > 0 && !wallOnTheLeft(black->i, black->j, wall_matrix, boardsize) && !(black->i == white->i && black->j - 1 == white->j))  // left
        {
            --black->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true);
            min_eval = min_eval < eval ? min_eval:eval;
            beta = beta < eval ? beta:eval;
            ++black->j;  // reset movement
            if (beta <= alpha) return min_eval;
        }

        if (black->j < boardsize - 1 && !wallOnTheRight(black->i, black->j, wall_matrix, boardsize) && !(black->i == white->i && black->j + 1 == white->j))  // right
        {
            ++black->j;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true);
            min_eval = min_eval < eval ? min_eval:eval;
            beta = beta < eval ? beta:eval;
            --black->j;  // reset movement
            if (beta <= alpha) return min_eval;
        }
        
        if (black->i < boardsize - 1 && !wallAbove(black->i, black->j, wall_matrix, boardsize) && !(black->i + 1 == white->i && black->j == white->j))  // up
        {
            ++black->i;
            eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true);
            min_eval = min_eval < eval ? min_eval:eval;
            beta = beta < eval ? beta:eval;
            --black->i;  // reset movement
            if (beta <= alpha) return min_eval;
        }

        // check wall placement
        for (int i = boardsize-1; i > 0; i--)
        {
            //if (depth == 1) break;
            if (black->walls == 0 || beta <= alpha) break;
            for (int j = 0; j < boardsize-1; j++)
            {
                if (black->walls == 0) break;
                if (!thereIsAWall('b', wall_matrix, boardsize, i, j))
                {
                    black->walls--;
                    wall_matrix[i][j] = 'b';  // place horizontal wall
                    en = 0;
                    if (there_is_a_path(wall_matrix, boardsize, white, black)) 
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true);
                        en = 1;
                        min_eval = min_eval < eval ? min_eval:eval;
                        beta = beta < eval ? beta:eval;
                        wall_matrix[i][j] = 0;
                        black->walls++;
                        if (beta <= alpha) return min_eval;
                    }
                    // reset placement
                    wall_matrix[i][j] = 0;
                    if (!en) black->walls++;
                }
                if (!thereIsAWall('r', wall_matrix, boardsize, i, j))
                {
                    en = 0;
                    black->walls--;
                    wall_matrix[i][j] = 'r';  // place vertical wall 
                    if (there_is_a_path(wall_matrix, boardsize, white, black))
                    {
                        eval = minimax(wall_matrix, boardsize, depth-1, alpha, beta, white, black, true);
                        en = 1;
                        min_eval = min_eval < eval ? min_eval:eval;
                        beta = beta < eval ? beta:eval;
                        wall_matrix[i][j] = 0;
                        black->walls++;
                        if (beta <= alpha) return min_eval;
                    }
                    // reset placement
                    wall_matrix[i][j] = 0; 
                    if (!en) black->walls++;
                }
            }
        }
        return min_eval;
    }
}
