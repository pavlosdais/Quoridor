#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "../../include/structs.h"
#include "../../include/typedefs.h"
#include "../../include/utilities.h"
#include "../../include/engine_utilities.h"
#include "../../include/bfs.h"
#include "../../include/dfs.h"
#include "../../include/pq.h"

// function prototype
static inline move create_move(cchar, csint, csint, int, char, cchar);

#define PAWN_MOVE 'm'
#define WALL_MOVE 'w'
#define NONE -1

static inline void generate_pawn_moves_white(gameState gs, PQ moves)
{
    int board_lim = gs->boardsize-1;
    move a;
    if (gs->white.i+1 == gs->black.i && gs->white.j == gs->black.j && !wallAbove(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize)) // black above white
    {
        if (gs->black.i < board_lim && !wallAbove(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))
        {
            a = create_move(WHITE, gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
            MOVE_UP2(a);
            pq_insert(moves, a);
        }
        else
        {
            if (gs->white.j < board_lim && !wallOnTheRight(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))  // diagonally up - right
            {
                a = create_move(WHITE, gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
                MOVE_UP(a); MOVE_RIGHT(a);
                pq_insert(moves, a);
            }

            if (gs->white.j > 0 && !wallOnTheLeft(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))  // diagonally up - left
            {
                a = create_move(WHITE, gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
                MOVE_UP(a); MOVE_LEFT(a);
                pq_insert(moves, a);
            }
        }
    }
    else if (gs->white.i == gs->black.i && gs->white.j+1 == gs->black.j && !wallOnTheRight(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))  // black on the right of white
    {
        if (gs->black.j < board_lim && !wallOnTheRight(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))
        {
            a = create_move(WHITE, gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
            MOVE_RIGHT2(a);
            pq_insert(moves, a);
        }
        else
        {
            if (gs->white.i < board_lim && !wallAbove(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))  // diagonally up - right
            {
                a = create_move(WHITE, gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
                MOVE_UP(a); MOVE_RIGHT(a);
                pq_insert(moves, a);
            }

            if (gs->white.i > 0 && !wallBelow(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))  // diagonally down - right
            {
                a = create_move(WHITE, gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
                MOVE_DOWN(a); MOVE_RIGHT(a);
                pq_insert(moves, a);
            }
        }
    }
    else if (gs->white.i == gs->black.i && gs->white.j-1 == gs->black.j && !wallOnTheLeft(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))  // black on the left of white
    {
        if (gs->black.j > 0 && !wallOnTheLeft(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))
        {
            a = create_move(WHITE, gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
            MOVE_LEFT2(a);
            pq_insert(moves, a);
        }
        else
        {
            if (gs->white.i < board_lim && !wallAbove(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))  // diagonally up - left
            {
                a = create_move(WHITE, gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
                MOVE_UP(a); MOVE_LEFT(a);
                pq_insert(moves, a);
            }
            if (gs->white.i > 0 && !wallBelow(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))  // diagonally down - left
            {
                a = create_move(WHITE, gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
                MOVE_DOWN(a); MOVE_LEFT(a);
                pq_insert(moves, a);
            }
        }
    }
    else if (gs->white.i-1 == gs->black.i && gs->white.j == gs->black.j && !wallBelow(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))  // black below white
    {
        if (gs->black.i > 0 && !wallBelow(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))
        {
            a = create_move(WHITE, gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
            MOVE_DOWN2(a);
            pq_insert(moves, a);
        }
    else
    {
        if (gs->white.j < board_lim && !wallOnTheRight(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))  // diagonally  down - right
        {
            a = create_move(WHITE, gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
            MOVE_DOWN(a); MOVE_RIGHT(a);
            pq_insert(moves, a);
        }
        if (gs->white.j > 0 && !wallOnTheLeft(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))  // diagonally down - left
        {
            a = create_move(WHITE, gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
            MOVE_DOWN(a); MOVE_LEFT(a);
            pq_insert(moves, a);
        }
    }
    }     
    // normal moves
    if (gs->white.i < board_lim && !wallAbove(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize) && !(gs->white.i + 1 == gs->black.i && gs->white.j == gs->black.j))  // up
    {
        a = create_move(WHITE, gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
        MOVE_UP(a);
        pq_insert(moves, a);
    }
    if (gs->white.j < board_lim && !wallOnTheRight(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize) && !(gs->white.i == gs->black.i && gs->white.j + 1 == gs->black.j))  // right
    {
        a = create_move(WHITE, gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
        MOVE_RIGHT(a);
        pq_insert(moves, a);
    }
    if (gs->white.j > 0 && !wallOnTheLeft(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize) && !(gs->white.i == gs->black.i && gs->white.j - 1 == gs->black.j))  // left
    {
        a = create_move(WHITE, gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
        MOVE_LEFT(a);
        pq_insert(moves, a);
    }
    if (gs->white.i > 0 && !wallBelow(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize) && !(gs->white.i - 1 == gs->black.i && gs->white.j == gs->black.j))  // down
    {
        a = create_move(WHITE, gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
        MOVE_DOWN(a);
        pq_insert(moves, a);
    }
}

static inline void generate_pawn_moves_black(gameState gs, PQ moves)
{
    int board_lim = gs->boardsize-1;
    move a;
    if (gs->black.i-1 == gs->white.i && gs->black.j == gs->white.j && !wallBelow(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))  // white below black
    {
        if (gs->white.i > 0 && !wallBelow(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))
        {
            a = create_move(BLACK, gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
            MOVE_DOWN2(a);
            pq_insert(moves, a);
        }
        else
        {
            if (gs->black.j < board_lim && !wallOnTheRight(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))  // diagonally  down - right
            {
                a = create_move(BLACK, gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
                MOVE_DOWN(a); MOVE_RIGHT(a);
                pq_insert(moves, a);
            }
            if (gs->black.j > 0 && !wallOnTheLeft(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))  // diagonally down - left
            {
                a = create_move(BLACK, gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
                MOVE_DOWN(a); MOVE_LEFT(a);
                pq_insert(moves, a);
            }
        }
    }
    else if (gs->black.i == gs->white.i && gs->black.j-1 == gs->white.j && !wallOnTheLeft(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))  // white on the left of black
    {
        if (gs->white.j > 0 && !wallOnTheLeft(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))
        {
            a = create_move(BLACK, gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
            MOVE_LEFT2(a);
            pq_insert(moves, a);
        }
        else
        {
            if (gs->black.i > 0 && !wallBelow(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))  // diagonally down - left
            {
                a = create_move(BLACK, gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
                MOVE_DOWN(a); MOVE_LEFT(a);
                pq_insert(moves, a);
            }
                
            if (gs->black.i < board_lim && !wallAbove(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))  // diagonally up - left
            {
                a = create_move(BLACK, gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
                MOVE_UP(a); MOVE_LEFT(a);
                pq_insert(moves, a);
            }
        }
    }
    else if (gs->black.i == gs->white.i && gs->black.j+1 == gs->white.j && !wallOnTheRight(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))  // white on the right of black
    {
        if (gs->white.j < board_lim && !wallOnTheRight(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))
        {
            a = create_move(BLACK, gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
            MOVE_RIGHT2(a);
            pq_insert(moves, a);
        }
        else
        {
            if (gs->black.i > 0 && !wallBelow(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))  // diagonally down - right
            {
                a = create_move(BLACK, gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
                MOVE_DOWN(a); MOVE_RIGHT(a);
                pq_insert(moves, a);
            }
            if (gs->black.i < board_lim && !wallAbove(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))  // diagonally up - right
            {
                a = create_move(BLACK, gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
                MOVE_UP(a); MOVE_RIGHT(a);
                pq_insert(moves, a);
            }
        }
    }
    else if (gs->black.i+1 == gs->white.i && gs->black.j == gs->white.j && !wallAbove(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))  // white above black
    {
        if (gs->white.i < board_lim && !wallAbove(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))
        {
            a = create_move(BLACK, gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
            MOVE_UP2(a);
            pq_insert(moves, a);
        }
        else
        {
            if (gs->black.j < board_lim && !wallOnTheRight(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))  // diagonally up - right
            {
                a = create_move(BLACK, gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
                MOVE_UP(a); MOVE_RIGHT(a);
                pq_insert(moves, a);
            }
            if (gs->black.j > 0 && !wallOnTheLeft(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))  // diagonally up - left
            {
                a = create_move(BLACK, gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
                MOVE_UP(a); MOVE_LEFT(a);
                pq_insert(moves, a);
            }
        }
    }
    // check normal moves
    if (gs->black.i > 0 && !wallBelow(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize) && !(gs->black.i - 1 == gs->white.i && gs->black.j == gs->white.j))  // down
    {
        a = create_move(BLACK, gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
        MOVE_DOWN(a);
        pq_insert(moves, a);
    }
    if (gs->black.j < board_lim && !wallOnTheRight(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize) && !(gs->black.i == gs->white.i && gs->black.j + 1 == gs->white.j))  // right
    {
        a = create_move(BLACK, gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
        MOVE_RIGHT(a);
        pq_insert(moves, a);
    }
    if (gs->black.j > 0 && !wallOnTheLeft(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize) && !(gs->black.i == gs->white.i && gs->black.j - 1 == gs->white.j))  // left
    {
        a = create_move(BLACK, gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
        MOVE_LEFT(a);
        pq_insert(moves, a);
    }      
    if (gs->black.i < board_lim && !wallAbove(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize) && !(gs->black.i + 1 == gs->white.i && gs->black.j == gs->white.j))  // up
    {
        a = create_move(BLACK, gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, PAWN_MOVE, NONE);
        MOVE_UP(a);
        pq_insert(moves, a);
    }
}

static inline void generate_walls_white(gameState gs, PQ moves)
{
    move a;
    for (small_int i = 1; i < gs->boardsize; i++)
    {
        int a = ABS(gs->white.i, i);
        for (small_int j = 0; j < gs->boardsize-1; j++)
        {
            int distance = a + ABS(gs->white.j, j) + i;
            if (!thereIsAWallHorizontally(gs, i, j))
            {
                // place horizontal wall
                gs->wall_matrix[i][j] = HORIZONTAL_WALL;

                if (there_is_a_path_white(gs))
                    pq_insert(moves, create_move(WHITE, i, j, distance, WALL_MOVE, HORIZONTAL_WALL));
                
                gs->wall_matrix[i][j] = 0;  // resest wall placement
            }
            if (!thereIsAWallVertically(gs, i, j))
            {
                // place vertical wall
                gs->wall_matrix[i][j] = VERTICAL_WALL;

                if (there_is_a_path_white(gs))
                    pq_insert(moves, create_move(WHITE, i, j, distance, WALL_MOVE, VERTICAL_WALL));
                
                gs->wall_matrix[i][j] = 0;  // resest wall placement
            }
        }
    }
}

static inline void generate_walls_black(gameState gs, PQ moves)
{
    move a;
    for (small_int i = gs->boardsize-1; i > 0; i--)
    {
        int a = ABS(gs->white.i, i);
        for (small_int j = 0; j < gs->boardsize-1; j++)
        {
            int distance = a + ABS(gs->white.j, j) + (gs->boardsize-1-i);
            if (!thereIsAWallHorizontally(gs, i, j))
            {
                // place horizontal wall
                gs->wall_matrix[i][j] = HORIZONTAL_WALL;

                if (there_is_a_path_black(gs))
                    pq_insert(moves, create_move(BLACK, i, j, distance, WALL_MOVE, HORIZONTAL_WALL));

                gs->wall_matrix[i][j] = 0;  // resest wall placement
            }
            if (!thereIsAWallVertically(gs, i, j))
            {
                // place vertical wall
                gs->wall_matrix[i][j] = VERTICAL_WALL;
                
                if (there_is_a_path_black(gs))
                    pq_insert(moves, create_move(BLACK, i, j, distance, WALL_MOVE, VERTICAL_WALL));

                gs->wall_matrix[i][j] = 0;  // resest wall placement
            }
        }
    }
}

PQ generate_moves_white(gameState gs)
{
    PQ moves;
    pq_init(&moves);

    // generate pawn moves
    generate_pawn_moves_white(gs, moves);
    
    // generate wall placement
    if (gs->white.walls != 0)
    {
        int pawn_moves = pq_size(moves);
        generate_walls_white(gs, moves);
        pq_sort(moves, pawn_moves-1);
    }
    return moves;
}

PQ generate_moves_black(gameState gs)
{
    PQ moves;
    pq_init(&moves);
    
    // generate pawn moves
    generate_pawn_moves_black(gs, moves);

    // generate wall placement
    if (gs->black.walls != 0)
    {
        int pawn_moves = pq_size(moves);
        generate_walls_black(gs, moves);
        pq_sort(moves, pawn_moves-1);
    }
    return moves;
}

static inline move create_move(cchar player, csint prev_i, csint prev_j, int eval, char m, cchar or)
{
    // allocate memory for the move
    move new_move = malloc(sizeof(*new_move));

    new_move->col = player;
    new_move->move = m;
    new_move->move_eval = eval;

    new_move->i = prev_i;
    new_move->j = prev_j;
    if (m == 'm')  // pawn movement
    {
        new_move->previ = prev_i;
        new_move->or_prevj = prev_j;
    }
    else  // wall placement
        new_move->or_prevj = or;

    return new_move;
}
