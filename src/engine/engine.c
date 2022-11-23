#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <time.h>
#include <stdbool.h>
#include "../../include/typedefs.h"
#include "../../include/structs.h"
#include "../../include/pq.h"
#include "../../include/transposition_table.h"
#include "../../include/generate_moves.h"
#include "../../include/engine_utilities.h"

#define MAXIMIZING 1
#define MINIMIZING -1

clock_t start_time; float timeout_time;
bool timeOut;

returningMove iterativeDeepening(gameState gs, cchar pl)
{
    find_end_time(gs, timeout_time)
    timeOut = false;

    returningMove moveFound, best_move;

    // find the depth (for the first few moves we search at depth 1)
    small_int max_depth = (gs->totalmoves + 3 < gs->boardsize || ( (gs->white.walls == 0) && (gs->black.walls == 0) ))? 1 : gs->boardsize*6;
    
    // start the timer
    start_time = clock();
    for (small_int current_depth = 1; current_depth <= max_depth; current_depth++)
    {
        best_move = moveFound;  // save previous move

        moveFound = bestMove(gs, pl, current_depth);
        if (timeOut)  // ran out of time before finishing the search, return the previously found move
            return best_move;
    }
    
    return moveFound;
}

// check and evaluate and evaluate every legal move for the player specified
returningMove bestMove(gameState gs, cchar pl, small_int depth)
{
    returningMove evalMove;
    move action;
    PQ moves;

    int best_eval;

    if (pl == WHITE)  // white plays
    {
        best_eval = NEG_INFINITY;
        
        moves = generate_moves_white(gs);  // generate white's legal moves

        int alpha = NEG_INFINITY;
        while(!is_pq_empty(moves))
        {
            if (timeOut || RAN_OUT_OF_TIME())  // ran out of time
            {
                timeOut = true;
                break;
            }
            action = pq_remove(moves);
            playMoveWhite(gs, action)

            int eval = -minimax(gs, depth-1, alpha, INFINITY, MINIMIZING);
            undoMoveWhite(gs, action)

            if (eval > best_eval)
            {
                if (action->move == 'm') updatePawnMovement(&evalMove, action, &eval, &best_eval);
                else updateWallPlacement(&evalMove, action, &eval, &best_eval);
            }

            if (best_eval > alpha) alpha = best_eval;
        }
    }
    else  // black plays
    {
        moves = generate_moves_black(gs);  // generate white's legal moves

        best_eval = INFINITY;

        int beta = INFINITY;
        while(!is_pq_empty(moves))
        {
            if (timeOut || RAN_OUT_OF_TIME())  // ran out of time
            {
                timeOut = true;
                break;
            }
            action = pq_remove(moves);
            playMoveBlack(gs, action)

            int eval = minimax(gs, depth-1, NEG_INFINITY, beta, MAXIMIZING);

            undoMoveBlack(gs, action)

            if (eval < best_eval)
            {
                if (action->move == 'm') updatePawnMovement(&evalMove, action, &eval, &best_eval);
                else updateWallPlacement(&evalMove, action, &eval, &best_eval);
            }

            if (beta < best_eval) beta = best_eval;
        }
    }
    pq_destroy(moves);
    return evalMove;
}

/* sources: https://en.wikipedia.org/wiki/Minimax
            https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning 
            https://en.wikipedia.org/wiki/Negamax#Negamax_with_alpha_beta_pruning_and_transposition_tables
*/

int minimax(gameState gs, small_int depth, int alpha, int beta, char maximizing)
{
    if (RAN_OUT_OF_TIME())  // ran out of time - stop searching
    {
        timeOut = true;
        return alpha;
    }

    int eval, original_alpha = alpha;

    // if the position has already been searched (hence has a value)
    // return the evaluation for this move without searching it
    if ((eval = tt_search(gs->game_tt, &alpha, &beta, depth, gs->position_id)) != UNKNOWN)
        return eval;
    
    // base case
    if (depth == 0 || gs->white.i == gs->boardsize-1 || gs->black.i == 0)
        return maximizing * positionEvaluation(gs);

    // generate all the legal moves for the player specified
    PQ legal_moves = generateMoves(gs, maximizing);

    int max_eval = NEG_INFINITY;
    while(!is_pq_empty(legal_moves))
    {
        // play the move
        move action = pq_remove(legal_moves);
        playMove(gs, action, maximizing)

        eval = -minimax(gs, depth-1, -beta, -alpha, -maximizing);
        
        // undo the move
        undoMove(gs, action, maximizing)

        if (eval > max_eval) max_eval = eval;
        if (max_eval > alpha) alpha = max_eval;

        if (alpha >= beta)  // cut off
            break;
    }

    if (max_eval <= original_alpha)
        tt_insert(gs->game_tt, alpha, LOWERBOUND, depth, gs->position_id);
    else if (max_eval >= beta)
        tt_insert(gs->game_tt, beta, UPPERBOUND, depth, gs->position_id);
    else
        tt_insert(gs->game_tt, max_eval, EXACT, depth, gs->position_id);
    
    pq_destroy(legal_moves);
    return max_eval;
}
