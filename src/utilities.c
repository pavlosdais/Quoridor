#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "../include/typedefs.h"
#include "../include/structs.h"
#include "../include/dfs.h"

char isNumber(char* n)
{
    int i = 0;
    if (n[0] == '-') i = 1;  // potentially a negative number
    while(n[i] != '\n' && n[i] != '\0')
    {
        if (!isdigit(n[i]))
            return 0;  // not a number
        i++;
    }
    if (n[0] == '-' && i == 1) return 0;  // not a number
    return 1;
}

char is_vertex_valid(char hor, const int boardsize)
{
    if (hor >= 0 && hor < boardsize)
        return true;
    return false;
}

player *check_color(char* p, player* black, player* white)
{
    if (strcmp(p, "white") == 0)
        return white;
    else if (strcmp(p, "w") == 0)
        return white;
    else if (strcmp(p, "black") == 0)
        return black;
    else if (strcmp(p, "b") == 0)
        return black;
    else  // error
        return NULL;
}

char colorValue(char* p)
{
    if (strcmp(p, "white") == 0)
        return 'w';
    else if (strcmp(p, "w") == 0)
        return 'w';
    else if (strcmp(p, "black") == 0)
        return 'b';
    else if (strcmp(p, "b") == 0)
        return 'b';
    else
        return -1;
}

char check_orientation(char* orientation)
{
    if (strcmp(orientation, "horizontal") == 0)
        return 'b';
    else if (strcmp(orientation, "h") == 0)
        return 'b';
    else if (strcmp(orientation, "vertical") == 0)
        return 'r';
    else if (strcmp(orientation, "v")  == 0)
        return 'r'; 
    return -1;  // error
}

char enough_arguments(char *argument)
{
    if (argument == NULL)  // not a valid argument
    {
        printf("? %s\n\n", "invalid syntax");
        fflush(stdout);
        return false;
    }
    // in any other case, a valid argument
    return true;
}

char wallBelow(const int i, const int j, char** wall_matrix, const int boardsize)
{
    if (i == 0) return false;
    return (wall_matrix[i][j]=='b' || (j>0 && wall_matrix[i][j-1]=='b'));
}

char wallAbove(const int i, const int j, char **wall_matrix, const int boardsize)
{
    if (i == boardsize-1) return false;
    return wallBelow(i+1, j, wall_matrix, boardsize);
}

char wallOnTheRight(const int i, const int j, char **wall_matrix, const int boardsize)
{
    if (j == boardsize-1) return false;
    return (wall_matrix[i][j]=='r' || (i<boardsize-1 && wall_matrix[i+1][j]=='r'));
}

char wallOnTheLeft(const int i, const int j, char** wall_matrix, const int boardsize)
{
    if (j == 0) return false;
    return wallOnTheRight(i, j-1, wall_matrix, boardsize);
}

static char thereIsAWall(const char or, gameState gs, const int vertex_x, const int vertex_y)
{
    if (gs->wall_matrix[vertex_x][vertex_y] != 0)
        return true;
    if (or == 'b')  // horizontal
    {
        if (gs->wall_matrix[vertex_x][vertex_y+1] == or) return true;
        else if (vertex_y > 0 && gs->wall_matrix[vertex_x][vertex_y-1] == or) return true;
    }
    else  // if (or == 'r') - vertical
    {
        if (gs->wall_matrix[vertex_x-1][vertex_y] == or) return true;
        else if (vertex_x < gs->boardsize-1 && gs->wall_matrix[vertex_x+1][vertex_y] == or) return true;
    }
    return false;
}

char thereIsAWallHorizontally(gameState gs, const int vertex_x, const int vertex_y)
{
    if (gs->wall_matrix[vertex_x][vertex_y] != 0)
        return true;
    if (gs->wall_matrix[vertex_x][vertex_y+1] == 'b') return true;
    else if (vertex_y > 0 && gs->wall_matrix[vertex_x][vertex_y-1] == 'b') return true;
    return false;
}

char thereIsAWallVertically(gameState gs, const int vertex_x, const int vertex_y)
{
    if (gs->wall_matrix[vertex_x][vertex_y] != 0)
        return true;
    if (gs->wall_matrix[vertex_x-1][vertex_y] == 'r') return true;
    else if (vertex_x < gs->boardsize-1 && gs->wall_matrix[vertex_x+1][vertex_y] == 'r') return true;
    return false;
}

char isValidWall(const int vertex_x, const int vertex_y, gameState gs, char orientation)
{
    if (!is_vertex_valid(vertex_x, gs->boardsize) || !is_vertex_valid(vertex_y, gs->boardsize) || vertex_x == 0 || vertex_y == gs->boardsize-1)  // orientation out of bounds
        return false;
    else if (thereIsAWall(orientation, gs, vertex_x, vertex_y))  // there's already a wall there
        return false;
    
    // in any other case, a valid wall placement
    return true;
}

char there_is_a_path(gameState gs)
{
    if (!dfs(gs->boardsize, gs->wall_matrix, gs->black.i, gs->black.j, 0, 'b'))
        return false;
    
    if (!dfs(gs->boardsize, gs->wall_matrix, gs->white.i, gs->white.j, gs->boardsize-1, 'w'))
        return false;
    
    // the path is not blocked for neither player
    return true;
}

void addMove(stackptr* last, const int i, const int j, char* type)
{
    stackptr temp = *last;
    *last = malloc(sizeof(struct stacknode));  // allocation failure
    assert(*last != NULL);

    (*last)->i = i;
    (*last)->j = j;
    (*last)->type = type;
    (*last)->next = temp;
}

void playGenMove(gameState gs, char pl, returningMove evalMove, stackptr* lastaddr)
{
    // engine evaluated a wall placement
    if (evalMove.move == 'w')
    {
        // hash wall placement
        if (evalMove.or == 'b')  // horizontal
            gs->position_id ^= gs->zobrist_values->horizontal_walls[evalMove.x][evalMove.y];
        else  // vertical
            gs->position_id ^= gs->zobrist_values->vertical_walls[evalMove.x][evalMove.y];
        
        if (pl == 'w')  // white
        {
            addMove(lastaddr, evalMove.x, evalMove.y, "ww");  // add wall placement to history

            // hash wall value
            gs->position_id ^= gs->zobrist_values->white_walls[gs->white.walls];
            gs->position_id ^= gs->zobrist_values->white_walls[--(gs->white.walls)];
        }
        else  // black
        {
            addMove(lastaddr, evalMove.x, evalMove.y, "bw");  // add wall placement to history

            // hash wall value
            gs->position_id ^= gs->zobrist_values->black_walls[gs->black.walls];
            gs->position_id ^= gs->zobrist_values->black_walls[--(gs->black.walls)];
        }

        // place wall
        gs->wall_matrix[evalMove.x][evalMove.y] = evalMove.or;

        // print the move
        printf("= %c%d %c\n\n", 'A'+evalMove.y, evalMove.x+1, (evalMove.or == 'b')? 'h' : 'v');
    }

    // engine evaluated a pawn movement
    else
    {
        if (pl == 'w')  // white
        {
            addMove(lastaddr, gs->white.i, gs->white.j, "wm");  // add move into history

            // hash pawn movement
            gs->position_id ^= gs->zobrist_values->white_pawn[gs->white.i][gs->white.j];
            gs->white.i = evalMove.x;  gs->white.j = evalMove.y;  // move pawn
            gs->position_id ^= gs->zobrist_values->white_pawn[gs->white.i][gs->white.j];
        }
        else  // black
        {
            addMove(lastaddr, gs->black.i, gs->black.j, "bm");  // add move into history
            
            // hash pawn movement
            gs->position_id ^= gs->zobrist_values->black_pawn[gs->black.i][gs->black.j];
            gs->black.i = evalMove.x; gs->black.j = evalMove.y;  // move pawn
            gs->position_id ^= gs->zobrist_values->black_pawn[gs->black.i][gs->black.j];
        }
        
        // print the move
        printf("= %c%d\n\n", 'A'+evalMove.y, evalMove.x+1);
    }
    fflush(stdout);
}
