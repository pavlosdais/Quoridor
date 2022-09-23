#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/typedefs.h"
#include "../include/structs.h"

// hash the quoridor board using zobrist hashing
// source: https://en.wikipedia.org/wiki/Zobrist_hashing

// -the hashing of the board uses 4 pieces of information:
// the horizontal & vertical walls
// the placement of white & black pawn
// the number of the walls of white & black


// pseudo random 64-byte number generator
uint random_state = 1804289383;

static uint get_random_U32_number()
{
    // get current state
    uint number = random_state;
    
    // XOR shift algorithm
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;
    
    // update random number state
    random_state = number;
    
    // return random number
    return number;
}

static U64 get_random_U64_number()
{
    // initialize "random" numbers slicing 16 bits from MS1B side
    U64 n1 = (U64)(get_random_U32_number()) & 0xFFFF;
    U64 n2 = (U64)(get_random_U32_number()) & 0xFFFF;
    U64 n3 = (U64)(get_random_U32_number()) & 0xFFFF;
    U64 n4 = (U64)(get_random_U32_number()) & 0xFFFF;
    
    // return number
    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

void createBoardKeys(gameState gs)
{
    // assert( != NULL);  // allocation failure
    random_state = 1804289383;
    gs->zobrist_values->horizontal_walls = malloc(sizeof(U64*) * gs->boardsize);
    assert(gs->zobrist_values->horizontal_walls != NULL);  // allocation failure

    gs->zobrist_values->vertical_walls = malloc(sizeof(U64*) * gs->boardsize);
    assert(gs->zobrist_values->vertical_walls != NULL);  // allocation failure

    gs->zobrist_values->white_pawn = malloc(sizeof(U64*) * gs->boardsize);
    assert(gs->zobrist_values->white_pawn != NULL);  // allocation failure

    gs->zobrist_values->black_pawn = malloc(sizeof(U64*) * gs->boardsize);
    assert(gs->zobrist_values->black_pawn != NULL);  // allocation failure

    for (unsigned char i = 0; i < gs->boardsize; i++)
    {
        gs->zobrist_values->horizontal_walls[i] = malloc(sizeof(U64) * gs->boardsize);
        assert(gs->zobrist_values->horizontal_walls[i] != NULL);  // allocation failure

        gs->zobrist_values->vertical_walls[i] = malloc(sizeof(U64) * gs->boardsize);
        assert(gs->zobrist_values->vertical_walls[i] != NULL);  // allocation failure

        gs->zobrist_values->white_pawn[i] = malloc(sizeof(U64) * gs->boardsize);
        assert(gs->zobrist_values->white_pawn[i] != NULL);  // allocation failure

        gs->zobrist_values->black_pawn[i] = malloc(sizeof(U64) * gs->boardsize);
        assert(gs->zobrist_values->black_pawn[i] != NULL);  // allocation failure

        for (unsigned char j = 0; j < gs->boardsize; j++)
        {
            gs->zobrist_values->horizontal_walls[i][j] = get_random_U64_number();
            gs->zobrist_values->vertical_walls[i][j] = get_random_U64_number();
            gs->zobrist_values->white_pawn[i][j] = get_random_U64_number();
            gs->zobrist_values->black_pawn[i][j] = get_random_U64_number();
        }
    }
}

void createWallKeys(gameState gs)
{
    unsigned char num_of_walls = gs->white.walls+1;

    gs->zobrist_values->white_walls = malloc(num_of_walls * sizeof(U64));
    assert(gs->zobrist_values->white_walls != NULL);  // allocation failure

    gs->zobrist_values->black_walls = malloc(num_of_walls * sizeof(U64));
    assert(gs->zobrist_values->black_walls != NULL);  // allocation failure

    for (unsigned char i = 0; i < num_of_walls; i++)
    {
        gs->zobrist_values->white_walls[i] = get_random_U64_number();
        gs->zobrist_values->black_walls[i] = get_random_U64_number();
    }
}

void createZobristKeys(gameState gs)
{
    gs->zobrist_values = malloc(sizeof(*(gs->zobrist_values)));
    assert(gs->zobrist_values != NULL);  // allocation failure

    // we need board and wall keys
    createBoardKeys(gs);
    createWallKeys(gs);
}

void destroyBoardKeys(gameState gs)
{
    for (unsigned char i = 0; i < gs->boardsize; i++)
    {
        free(gs->zobrist_values->horizontal_walls[i]);
        free(gs->zobrist_values->vertical_walls[i]);
        free(gs->zobrist_values->white_pawn[i]);
        free(gs->zobrist_values->black_pawn[i]);
    }
    free(gs->zobrist_values->horizontal_walls);
    free(gs->zobrist_values->vertical_walls);
    free(gs->zobrist_values->white_pawn);
    free(gs->zobrist_values->black_pawn);
}

void destroyWallKeys(gameState gs)
{
    free(gs->zobrist_values->white_walls);
    free(gs->zobrist_values->black_walls);
}

void destroyZobristKeys(gameState gs)
{
    destroyBoardKeys(gs);
    destroyWallKeys(gs);
}

void generateStartingPositionID(gameState gs)
{
    // generate starting position hash id
    gs->position_id = 0ULL;

    // hash pawns
    gs->position_id ^= gs->zobrist_values->white_pawn[gs->white.i][gs->white.j];
    gs->position_id ^= gs->zobrist_values->black_pawn[gs->black.i][gs->black.j];

    // hash walls
    gs->position_id ^= gs->zobrist_values->white_walls[gs->white.walls];
    gs->position_id ^= gs->zobrist_values->black_walls[gs->white.walls];
}

void generatePositionID(gameState gs)
{
    generateStartingPositionID(gs);
    for (unsigned char i = 0; i < gs->boardsize; i++)
    {
        for (unsigned char j = 0; j < gs->boardsize; j++)
        {
            if (gs->wall_matrix[i][j] == 'b')  // horizontal
                gs->position_id ^= gs->zobrist_values->horizontal_walls[i][j];
            else  // vertical
                gs->position_id ^= gs->zobrist_values->vertical_walls[i][j];
        }
    }
}
