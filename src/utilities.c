#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "../include/typedefs.h"
#include "../include/structs.h"
#include "../include/bfs.h"
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

// returns the player with the corresponding color
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

// returns color's value
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

// returns b for horizontal, r for vertical and -1 for unknown orientation
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
    return -1; // error
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

// returns 1 if the path is not blocked for neither player, 0 if it's blocked for at least one player
char there_is_a_path(char** wall_matrix, const int boardsize, player* white, player* black, const char pl)
{
    int path_exists = dfs(boardsize, wall_matrix, white->i, white->j, boardsize-1, 'w');  // white wins if he gets to the last row
    if (!path_exists)
        return false;

    path_exists = dfs(boardsize, wall_matrix, black->i, black->j, 0, 'b');  // black wins if he gets to the first row
    if (!path_exists)
        return false;
    
    // the path is not blocked for neither player
    return true;
}

// returns 0 if there's a wall below cell (i,j)
char wallBelow(const int i, const int j, char** wall_matrix, const int boardsize)
{
    if (i==0) return false;
    return (wall_matrix[i][j]=='b' || (j>0 && wall_matrix[i][j-1]=='b'));
}

// returns 0 if there's a wall above cell (i,j)
char wallAbove(const int i, const int j, char **wall_matrix, const int boardsize)
{
    if (i==boardsize-1) return false;
    return wallBelow(i+1, j, wall_matrix, boardsize);
}

// returns 0 if there's a wall on the right of cell (i,j)
char wallOnTheRight(const int i, const int j, char **wall_matrix, const int boardsize)
{
    if (j==boardsize-1) return false;
    return (wall_matrix[i][j]=='r' || (i<boardsize-1 && wall_matrix[i+1][j]=='r'));
}

// returns 0 if there's a wall on the left of cell (i,j)
char wallOnTheLeft(const int i, const int j, char** wall_matrix, const int boardsize)
{
    if (j==0) return false;
    return wallOnTheRight(i, j-1, wall_matrix, boardsize);
}

char thereIsAWall(const char or, char** wall_matrix, const int boardsize, const int vertex_x, const int vertex_y)
{
    if (wall_matrix[vertex_x][vertex_y] != 0)
        return true;
    if (or == 'b')
    {
        if (wall_matrix[vertex_x][vertex_y+1] == 'b') return true;
        else if (vertex_y > 0 && wall_matrix[vertex_x][vertex_y-1] == 'b') return true;
    }
    else  // if (or == 'r')
    {
        if (wall_matrix[vertex_x-1][vertex_y] == 'r') return true;
        else if (vertex_x < boardsize-1 && wall_matrix[vertex_x+1][vertex_y] == 'r') return true;
    }
    return false;
}

char isValidWall(const int vertex_x, const int vertex_y, const int boardsize, char** wall_matrix, char orientation)
{
    if (!is_vertex_valid(vertex_x, boardsize) || !is_vertex_valid(vertex_y, boardsize) || 
    vertex_x == 0 || vertex_y == boardsize-1)  // orientation out of bounds
        return false;
    else if (thereIsAWall(orientation, wall_matrix, boardsize, vertex_x, vertex_y))  // there's already a wall there
        return false;
    
    // in any other case, a valid wall placement
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

/* positionEvaluation is a heuristic function used for the evaluation of the current position of the game.
It takes into account the distance each player needs in order to win (factor = 1), the number of steps each player
needs to get to the next row (factor = 0.6) as well as the number of walls each player has (factor = 0.8) in order
to give an advantage (or lack of) for a certain player. When it's positive it calculates that the position is
advantageous for white and when it's negative it calculates that black has an advantage. If it's 0 it means that
the position is equal so neither player has an advantage. */

#define WHITE_WINS 999999
#define BLACK_WINS -999999

int positionEvaluation(player black, player white, const int boardsize, char** wall_matrix)
{
    if (black.i == 0)  // black reached the end
        return BLACK_WINS;
    else if (white.i == boardsize-1)  // white reached the end
        return WHITE_WINS;

    // calculate the distance white needs to get to the end
    uint whiteDistanceFromWinning = bfs(boardsize, wall_matrix, white.i, white.j, boardsize-1);

    // calculate the distance black needs to get to the end
    uint blackDistanceFromWinning = bfs(boardsize, wall_matrix, black.i, black.j, 0);

    // calculate the distance white needs to get to the next row
    uint whiteDistanceFromNextRow = bfs(boardsize, wall_matrix, white.i, white.j, white.i+1);

    // calculate the distance black needs to get to the next row
    uint blackDistanceFromNextRow = bfs(boardsize, wall_matrix, black.i, black.j, black.i-1);

    return 10*(blackDistanceFromWinning-whiteDistanceFromWinning) + 6*(blackDistanceFromNextRow - whiteDistanceFromNextRow) + 7*(white.walls - black.walls);
}

/* the strength of the engine is basically determined here,
increase the depth numbers to make the engine more powerful, abeit slower */
small_int findDepth(const int boardsize, char* pseudo, float* max_time, const int total_moves, const int total_walls)
{
    /* pseudodepth is used to simulate a further search to make the depth even.
    If pseudodepth is true the last step will only check opponent's "most likely"
    responses instead of all possible answers. This means it will check pawn movement
    and wall placement near the enemy pawn to disrupt it's movement */

    *max_time = 9.2;

    // first few moves do not really need much depth, just search at depth 1
    if (total_moves + 3 < boardsize)
        return 1;
    
    small_int depth;
    if (boardsize < 6)  // boardsize = 5
    {
        *max_time = 12;
        depth = 8;
        if (total_walls < 5)
            depth = 10;
        if (total_walls < 3)
        {
            *max_time = 15;
            depth = 12;
        }
    }
    else if (boardsize < 8)  // boardsize = 7
    {
        depth = 4;
        if (total_walls > 1)
        {
            if (total_walls < 4)
               depth = 8; 
            else if (total_walls < 6)
            {
                *pseudo = 1;
                depth = 8;
            }
            else if (total_walls < 9)
                depth = 6;
            else if (total_walls < 13)
            {
                *pseudo = 1;
                depth = 6;
            }
        }
    }
    else if (boardsize < 10)  // boardsize = 9
    {
        depth = 4;
        if (total_walls > 1 && total_walls < 7)
        {
            // pseudo-depth 6
            depth = 6;
            *pseudo = 1;
        }
    }
    else if (boardsize < 12)  // boardsize = 11
    {
        *pseudo = 1;
        depth = 4;
        if (total_walls < 8)
            depth = 6;
        else if (total_walls < 16)
            *pseudo = 0;
    }
    else if (boardsize < 14)  // boardsize = 13
    {
        depth = 3;
        if (total_walls < 14)
        {
            *pseudo = 1;
            depth = 4;
        }
        else if (total_walls < 7)
            depth = 6;
    }
    else if (boardsize < 16)  // boardsize = 15
    {
        depth = 2;
        if (total_walls < 13)
        {
            *pseudo = 1;
            depth = 4;
        }
        else if (total_walls < 5)
            depth = 6;
    }
    else if (boardsize < 20)
        depth = 2;
    {
        *pseudo = 1;
        depth = 2;
    }
    
    return depth;
}
