#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <stdbool.h>
#include "../../include/structs.h"
#include "../../include/typedefs.h"
#include "../../include/utilities.h"
#include "../../include/engine_utilities.h"
#include "../../include/bfs.h"
#include "../../include/dfs.h"
#include "../../include/pq.h"

// function prototype
static move create_move(cchar player, csint prev_i, csint prev_j, int eval, char m, cchar or);

static void generate_pawn_moves_white(gameState gs, PQ moves)
{
    move a;
    if (gs->white.i+1 == gs->black.i && gs->white.j == gs->black.j && !wallAbove(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize)) // black above white
    {
        if (gs->black.i < gs->boardsize - 1 && !wallAbove(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))
        {
            a = create_move('w', gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, 'm', -1);
            MOVE_UP2(a);
            pq_insert(moves, a);
        }
        else
        {
            if (gs->white.j < gs->boardsize-1 && !wallOnTheRight(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))  // diagonally up - right
            {
                a = create_move('w', gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, 'm', -1);
                MOVE_UP(a); MOVE_RIGHT(a);
                pq_insert(moves, a);
            }

            if (gs->white.j > 0 && !wallOnTheLeft(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))  // diagonally up - left
            {
                a = create_move('w', gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, 'm', -1);
                MOVE_UP(a); MOVE_LEFT(a);
                pq_insert(moves, a);
            }
        }
    }
    else if (gs->white.i == gs->black.i && gs->white.j+1 == gs->black.j && !wallOnTheRight(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))  // black on the right of white
    {
        if (gs->black.j < gs->boardsize - 1 && !wallOnTheRight(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))
        {
            a = create_move('w', gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, 'm', -1);
            MOVE_RIGHT2(a);
            pq_insert(moves, a);
        }
        else
        {
            if (gs->white.i < gs->boardsize-1 && !wallAbove(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))  // diagonally up - right
            {
                a = create_move('w', gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, 'm', -1);
                MOVE_UP(a); MOVE_RIGHT(a);
                pq_insert(moves, a);
            }

            if (gs->white.i > 0 && !wallBelow(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))  // diagonally down - right
            {
                a = create_move('w', gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, 'm', -1);
                MOVE_DOWN(a); MOVE_RIGHT(a);
                pq_insert(moves, a);
            }
        }
    }
    else if (gs->white.i == gs->black.i && gs->white.j-1 == gs->black.j && !wallOnTheLeft(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))  // black on the left of white
    {
        if (gs->black.j > 0 && !wallOnTheLeft(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))
        {
            a = create_move('w', gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, 'm', -1);
            MOVE_LEFT2(a);
            pq_insert(moves, a);
        }
        else
        {
            if (gs->white.i < gs->boardsize-1 && !wallAbove(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))  // diagonally up - left
            {
                a = create_move('w', gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, 'm', -1);
                MOVE_UP(a); MOVE_LEFT(a);
                pq_insert(moves, a);
            }
            if (gs->white.i > 0 && !wallBelow(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))  // diagonally down - left
            {
                a = create_move('w', gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, 'm', -1);
                MOVE_DOWN(a); MOVE_LEFT(a);
                pq_insert(moves, a);
            }
        }
    }
    else if (gs->white.i-1 == gs->black.i && gs->white.j == gs->black.j && !wallBelow(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))  // black below white
    {
        if (gs->black.i > 0 && !wallBelow(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))
        {
            a = create_move('w', gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, 'm', -1);
            MOVE_DOWN2(a);
            pq_insert(moves, a);
        }
    else
    {
        if (gs->white.j < gs->boardsize-1 && !wallOnTheRight(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))  // diagonally  down - right
        {
            a = create_move('w', gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, 'm', -1);
            MOVE_DOWN(a); MOVE_RIGHT(a);
            pq_insert(moves, a);
        }
        if (gs->white.j > 0 && !wallOnTheLeft(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))  // diagonally down - left
        {
            a = create_move('w', gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, 'm', -1);
            MOVE_DOWN(a); MOVE_LEFT(a);
            pq_insert(moves, a);
        }
    }
    }     
    // normal moves
    if (gs->white.i < gs->boardsize-1 && !wallAbove(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize) && !(gs->white.i + 1 == gs->black.i && gs->white.j == gs->black.j))  // up
    {
        a = create_move('w', gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, 'm', -1);
        MOVE_UP(a);
        pq_insert(moves, a);
    }
    if (gs->white.j < gs->boardsize - 1 && !wallOnTheRight(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize) && !(gs->white.i == gs->black.i && gs->white.j + 1 == gs->black.j))  // right
    {
        a = create_move('w', gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, 'm', -1);
        MOVE_RIGHT(a);
        pq_insert(moves, a);
    }
    if (gs->white.j > 0 && !wallOnTheLeft(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize) && !(gs->white.i == gs->black.i && gs->white.j - 1 == gs->black.j))  // left
    {
        a = create_move('w', gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, 'm', -1);
        MOVE_LEFT(a);
        pq_insert(moves, a);
    }
    if (gs->white.i > 0 && !wallBelow(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize) && !(gs->white.i - 1 == gs->black.i && gs->white.j == gs->black.j))  // down
    {
        a = create_move('w', gs->white.i, gs->white.j, PAWN_MOVEMENT_EVAL, 'm', -1);
        MOVE_DOWN(a);
        pq_insert(moves, a);
    }
}

static void generate_pawn_moves_black(gameState gs, PQ moves)
{
    move a;
    if (gs->black.i-1 == gs->white.i && gs->black.j == gs->white.j && !wallBelow(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))  // white below black
    {
        if (gs->white.i > 0 && !wallBelow(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))
        {
            a = create_move('b', gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, 'm', -1);
            MOVE_DOWN2(a);
            pq_insert(moves, a);
        }
        else
        {
            if (gs->black.j < gs->boardsize-1 && !wallOnTheRight(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))  // diagonally  down - right
            {
                a = create_move('b', gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, 'm', -1);
                MOVE_DOWN(a); MOVE_RIGHT(a);
                pq_insert(moves, a);
            }
            if (gs->black.j > 0 && !wallOnTheLeft(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))  // diagonally down - left
            {
                a = create_move('b', gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, 'm', -1);
                MOVE_DOWN(a); MOVE_LEFT(a);
                pq_insert(moves, a);
            }
        }
    }
    else if (gs->black.i == gs->white.i && gs->black.j-1 == gs->white.j && !wallOnTheLeft(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))  // white on the left of black
    {
        if (gs->white.j > 0 && !wallOnTheLeft(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))
        {
            a = create_move('b', gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, 'm', -1);
            MOVE_LEFT2(a);
            pq_insert(moves, a);
        }
        else
        {
            if (gs->black.i > 0 && !wallBelow(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))  // diagonally down - left
            {
                a = create_move('b', gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, 'm', -1);
                MOVE_DOWN(a); MOVE_LEFT(a);
                pq_insert(moves, a);
            }
                
            if (gs->black.i < gs->boardsize - 1 && !wallAbove(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))  // diagonally up - left
            {
                a = create_move('b', gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, 'm', -1);
                MOVE_UP(a); MOVE_LEFT(a);
                pq_insert(moves, a);
            }
        }
    }
    else if (gs->black.i == gs->white.i && gs->black.j+1 == gs->white.j && !wallOnTheRight(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))  // white on the right of black
    {
        if (gs->white.j < gs->boardsize - 1 && !wallOnTheRight(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))
        {
            a = create_move('b', gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, 'm', -1);
            MOVE_RIGHT2(a);
            pq_insert(moves, a);
        }
        else
        {
            if (gs->black.i > 0 && !wallBelow(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))  // diagonally down - right
            {
                a = create_move('b', gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, 'm', -1);
                MOVE_DOWN(a); MOVE_RIGHT(a);
                pq_insert(moves, a);
            }
            if (gs->black.i < gs->boardsize - 1 && !wallAbove(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))  // diagonally up - right
            {
                a = create_move('b', gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, 'm', -1);
                MOVE_UP(a); MOVE_RIGHT(a);
                pq_insert(moves, a);
            }
        }
    }
    else if (gs->black.i+1 == gs->white.i && gs->black.j == gs->white.j && !wallAbove(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize))  // white above black
    {
        if (gs->white.i < gs->boardsize - 1 && !wallAbove(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))
        {
            a = create_move('b', gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, 'm', -1);
            MOVE_UP2(a);
            pq_insert(moves, a);
        }
        else
        {
            if (gs->black.j < gs->boardsize-1 && !wallOnTheRight(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))  // diagonally up - right
            {
                a = create_move('b', gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, 'm', -1);
                MOVE_UP(a); MOVE_RIGHT(a);
                pq_insert(moves, a);
            }
            if (gs->black.j > 0 && !wallOnTheLeft(gs->white.i, gs->white.j, gs->wall_matrix, gs->boardsize))  // diagonally up - left
            {
                a = create_move('b', gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, 'm', -1);
                MOVE_UP(a); MOVE_LEFT(a);
                pq_insert(moves, a);
            }
        }
    }
    // check normal moves
    if (gs->black.i > 0 && !wallBelow(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize) && !(gs->black.i - 1 == gs->white.i && gs->black.j == gs->white.j))  // down
    {
        a = create_move('b', gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, 'm', -1);
        MOVE_DOWN(a);
        pq_insert(moves, a);
    }
    if (gs->black.j < gs->boardsize-1 && !wallOnTheRight(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize) && !(gs->black.i == gs->white.i && gs->black.j + 1 == gs->white.j))  // right
    {
        a = create_move('b', gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, 'm', -1);
        MOVE_RIGHT(a);
        pq_insert(moves, a);
    }
    if (gs->black.j > 0 && !wallOnTheLeft(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize) && !(gs->black.i == gs->white.i && gs->black.j - 1 == gs->white.j))  // left
    {
        a = create_move('b', gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, 'm', -1);
        MOVE_LEFT(a);
        pq_insert(moves, a);
    }      
    if (gs->black.i < gs->boardsize - 1 && !wallAbove(gs->black.i, gs->black.j, gs->wall_matrix, gs->boardsize) && !(gs->black.i + 1 == gs->white.i && gs->black.j == gs->white.j))  // up
    {
        a = create_move('b', gs->black.i, gs->black.j, PAWN_MOVEMENT_EVAL, 'm', -1);
        MOVE_UP(a);
        pq_insert(moves, a);
    }
}

static void generate_walls_white(gameState gs, PQ moves)
{
    move a;
    for (unsigned char i = 1; i < gs->boardsize; i++)
    {
        int a = ABS(gs->white.i, i);
        for (unsigned char j = 0; j < gs->boardsize-1; j++)
        {
            int distance = a + ABS(gs->white.j, j);
            if (!thereIsAWallHorizontally(gs, i, j))
            {
                // place horizontal wall
                gs->wall_matrix[i][j] = 'b';

                if (there_is_a_path_white(gs))
                    pq_insert(moves, create_move('w', i, j, distance, 'w', 'b'));
                
                gs->wall_matrix[i][j] = 0;  // resest wall placement
            }
            if (!thereIsAWallVertically(gs, i, j))
            {
                // place vertical wall
                gs->wall_matrix[i][j] = VERTICAL_WALL;

                if (there_is_a_path_white(gs))
                    pq_insert(moves, create_move('w', i, j, distance, 'w', VERTICAL_WALL));
                
                gs->wall_matrix[i][j] = 0;  // resest wall placement
            }
        }
    }
}

static void generate_walls_black(gameState gs, PQ moves)
{
    move a;
    for (unsigned char i = gs->boardsize-1; i > 0; i--)
    {
        int a = ABS(gs->white.i, i);
        for (unsigned char j = 0; j < gs->boardsize-1; j++)
        {
            int distance = a + ABS(gs->white.j, j);
            if (!thereIsAWallHorizontally(gs, i, j))
            {
                // place horizontal wall
                gs->wall_matrix[i][j] = 'b';

                if (there_is_a_path_black(gs))
                    pq_insert(moves, create_move('b', i, j, distance, 'w', 'b'));

                gs->wall_matrix[i][j] = 0;  // resest wall placement
            }
            if (!thereIsAWallVertically(gs, i, j))
            {
                // place vertical wall
                gs->wall_matrix[i][j] = VERTICAL_WALL;
                
                if (there_is_a_path_black(gs))
                    pq_insert(moves, create_move('b', i, j, distance, 'w', VERTICAL_WALL));

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

void playMoveWhite(gameState gs, move m)
{
    if (m->move == 'm')  // pawn move
    {
        gs->position_id ^= gs->zobrist_values->white_pawn[gs->white.i][gs->white.j];
        gs->white.i = m->i; gs->white.j = m->j;  // move the pawn
        gs->position_id ^= gs->zobrist_values->white_pawn[gs->white.i][gs->white.j];
    }
    else  // wall placement
    {
        if (m->or_prevj == 'b')  // horizontal wall
            gs->position_id ^= gs->zobrist_values->horizontal_walls[m->i][m->j];
        else  // vertical wall
            gs->position_id ^= gs->zobrist_values->vertical_walls[m->i][m->j];
        
        gs->wall_matrix[m->i][m->j] = m->or_prevj;  // place the wall

        gs->position_id ^= gs->zobrist_values->white_walls[gs->white.walls];
        gs->position_id ^= gs->zobrist_values->white_walls[--(gs->white.walls)];

    }
}

void playMoveBlack(gameState gs, move m)
{
    if (m->move == 'm')  // pawn move
    {
        gs->position_id ^= gs->zobrist_values->black_pawn[gs->black.i][gs->black.j];
        gs->black.i = m->i; gs->black.j = m->j;  // move the pawn
        gs->position_id ^= gs->zobrist_values->black_pawn[gs->black.i][gs->black.j];
    }
    else  // wall placement
    {
        if (m->or_prevj == 'b')  // horizontal wall
            gs->position_id ^= gs->zobrist_values->horizontal_walls[m->i][m->j];
        else  // vertical wall
            gs->position_id ^= gs->zobrist_values->vertical_walls[m->i][m->j];
        
        gs->wall_matrix[m->i][m->j] = m->or_prevj;  // place the wall

        gs->position_id ^= gs->zobrist_values->black_walls[gs->black.walls];
        gs->position_id ^= gs->zobrist_values->black_walls[--(gs->black.walls)];
    }
}

void undoMoveBlack(gameState gs, move m)
{
    if (m->move == 'm')  // pawn move
    {
        gs->position_id ^= gs->zobrist_values->black_pawn[gs->black.i][gs->black.j];
        gs->black.i = m->previ; gs->black.j = m->or_prevj;  // move the pawn back into its original position
        gs->position_id ^= gs->zobrist_values->black_pawn[gs->black.i][gs->black.j];
    }
    else  // wall placement
    {
        if (m->or_prevj == 'b')  // horizontal wall
            gs->position_id ^= gs->zobrist_values->horizontal_walls[m->i][m->j];
        else  // vertical wall
            gs->position_id ^= gs->zobrist_values->vertical_walls[m->i][m->j];
        
        gs->wall_matrix[m->i][m->j] = 0;  // place the wall

        gs->position_id ^= gs->zobrist_values->black_walls[gs->black.walls];
        gs->position_id ^= gs->zobrist_values->black_walls[++(gs->black.walls)];
    }
}

void undoMoveWhite(gameState gs, move m)
{
    if (m->move == 'm')  // pawn move
    {
        gs->position_id ^= gs->zobrist_values->white_pawn[gs->white.i][gs->white.j];
        gs->white.i = m->previ; gs->white.j = m->or_prevj;  // move the pawn back into its original position
        gs->position_id ^= gs->zobrist_values->white_pawn[gs->white.i][gs->white.j];
    }
    else  // wall placement
    {
        if (m->or_prevj == 'b')  // horizontal wall
            gs->position_id ^= gs->zobrist_values->horizontal_walls[m->i][m->j];
        else  // vertical wall
            gs->position_id ^= gs->zobrist_values->vertical_walls[m->i][m->j];
        
        gs->wall_matrix[m->i][m->j] = 0;  // place the wall

        gs->position_id ^= gs->zobrist_values->white_walls[gs->white.walls];
        gs->position_id ^= gs->zobrist_values->white_walls[++(gs->white.walls)];
    }
}

void playMove(gameState gs, move m, char player)
{
    if (player == 1)  // player is white
        playMoveWhite(gs, m);
    else  // player is black
        playMoveBlack(gs, m);
}

void undoMove(gameState gs, move m, char player)
{
    if (player == 1)  // player is white
        undoMoveWhite(gs, m);
    else  // player is black
        undoMoveBlack(gs, m);
}

PQ generateMoves(gameState gs, char player)
{
    return ( (player == 1)? generate_moves_white(gs) : generate_moves_black(gs));
}

static move create_move(cchar player, csint prev_i, csint prev_j, int eval, char m, cchar or)
{
    // allocate memory for the move
    move new_move = malloc(sizeof(*new_move));
    assert(new_move != NULL);

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
