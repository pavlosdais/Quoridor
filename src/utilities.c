#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
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

char is_vertex_valid(char hor, cint boardsize)
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

char wallBelow(cint i, cint j, char** wall_matrix, cint boardsize)
{
    if (i == 0) return false;
    return (wall_matrix[i][j]=='b' || (j>0 && wall_matrix[i][j-1]=='b'));
}

char wallAbove(cint i, cint j, char **wall_matrix, cint boardsize)
{
    if (i == boardsize-1) return false;
    return wallBelow(i+1, j, wall_matrix, boardsize);
}

char wallOnTheRight(cint i, cint j, char **wall_matrix, cint boardsize)
{
    if (j == boardsize-1) return false;
    return (wall_matrix[i][j]=='r' || (i<boardsize-1 && wall_matrix[i+1][j]=='r'));
}

char wallOnTheLeft(cint i, cint j, char** wall_matrix, cint boardsize)
{
    if (j == 0) return false;
    return wallOnTheRight(i, j-1, wall_matrix, boardsize);
}

char thereIsAWall(const char or, char** wall_matrix, cint boardsize, cint vertex_x, cint vertex_y)
{
    if (wall_matrix[vertex_x][vertex_y] != 0)
        return true;
    if (or == 'b')  // horizontal
    {
        if (wall_matrix[vertex_x][vertex_y+1] == or) return true;
        else if (vertex_y > 0 && wall_matrix[vertex_x][vertex_y-1] == or) return true;
    }
    else  // if (or == 'r') - vertical
    {
        if (wall_matrix[vertex_x-1][vertex_y] == or) return true;
        else if (vertex_x < boardsize-1 && wall_matrix[vertex_x+1][vertex_y] == or) return true;
    }
    return false;
}

char isValidWall(cint vertex_x, cint vertex_y, cint boardsize, char** wall_matrix, char orientation)
{
    if (!is_vertex_valid(vertex_x, boardsize) || !is_vertex_valid(vertex_y, boardsize) || 
         vertex_x == 0 || vertex_y == boardsize-1)  // orientation out of bounds
        return false;
    else if (thereIsAWall(orientation, wall_matrix, boardsize, vertex_x, vertex_y))  // there's already a wall there
        return false;
    
    // in any other case, a valid wall placement
    return true;
}

char there_is_a_path(char** wall_matrix, cint boardsize, player* white, player* black)
{
    if (!dfs(boardsize, wall_matrix, black->i, black->j, 0, 'b'))
        return false;
    
    if (!dfs(boardsize, wall_matrix, white->i, white->j, boardsize-1, 'w'))
        return false;

    // the path is not blocked for neither player
    return true;
}

char there_is_a_path_black(char** wall_matrix, cint boardsize, player* white, player* black)
{
    if (!dfs(boardsize, wall_matrix, white->i, white->j, boardsize-1, 'w'))
        return false;

    if (!dfs(boardsize, wall_matrix, black->i, black->j, 0, 'b'))
        return false;
    
    // the path is not blocked for neither player
    return true;
}

void addMove(stackptr* last, cint i, cint j, char* type)
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

int positionEvaluation(player black, player white, cint boardsize, char** wall_matrix)
{
    if (black.i == 0)  // black reached his goal row - win
        return BLACK_WINS;
    else if (white.i == boardsize-1)  // white reached his goal row - win
        return WHITE_WINS;

    // calculate the distance white needs to get to the goal row (win)
    uint whiteDistanceFromWinning = bfs(boardsize, wall_matrix, white.i, white.j, boardsize-1);

    // calculate the distance black needs to get to the goal row (win)
    uint blackDistanceFromWinning = bfs(boardsize, wall_matrix, black.i, black.j, 0);

    // calculate the distance white needs to get to the next row
    uint whiteDistanceFromNextRow = bfs(boardsize, wall_matrix, white.i, white.j, white.i+1);

    // calculate the distance black needs to get to the next row
    uint blackDistanceFromNextRow = bfs(boardsize, wall_matrix, black.i, black.j, black.i-1);

    return 10*(blackDistanceFromWinning-whiteDistanceFromWinning) + 6*(blackDistanceFromNextRow - whiteDistanceFromNextRow) + 7*(white.walls - black.walls);
}

#define SCOPE 1.5

small_int findBoardsize5(char* pseudo, float* max_time, float* scope, cint total_walls)
{
    *scope = 1.2;
    *max_time = 12;

    if (total_walls < 3)
        return 12;
    else if (total_walls < 5)
        return 10;

    return 8;
}

small_int findBoardsize7(char* pseudo, float* max_time, float* scope, cint total_walls)
{
    *scope = 1;
    *max_time = 11;  // max branch, search time

    if (total_walls < 5)
    {
        return 8;
    }
    else if (total_walls < 8)
    {
        return 6;
    }
    else if (total_walls < 11)
    {
        *max_time = 12;  // max branch, search time
        *pseudo = 1;
        return 6;
    }
    return 4;
}

small_int findBoardsize9(char* pseudo, float* max_time, float* scope, cint total_walls)
{
    *scope = SCOPE;
    *max_time = 9.2;  // max branch, search time

    if (total_walls > 1 && total_walls < 7)
    {
        *pseudo = true;
        return 6;
    }
    return 4;
}

small_int findBoardsize11(char* pseudo, float* max_time, float* scope, cint total_walls)
{
    *scope = SCOPE;
    *max_time = 9;  // max branch, search time

    *pseudo = true;
    if (total_walls < 8)
    {
        return 6;
    }
    else if (total_walls < 16)
    {
        *pseudo = 0;
    }
    
    return 4;
}

small_int findBoardsize13(char* pseudo, float* max_time, float* scope, cint total_walls)
{
    *scope = SCOPE;
    *max_time = 8.5;  // max branch, search time

    if (total_walls < 14)
    {
        *pseudo = true;
        return 4;
    }
    else if (total_walls < 7)
    {
        return 6;
    }
    
    return 3;
}

small_int findBoardsize15(char* pseudo, float* max_time, float* scope, cint total_walls)
{
    *scope = SCOPE;
    *max_time = 8;  // max branch, search time

    if (total_walls < 13)
    {
        *pseudo = true;
        return 4;
    }
    else if (total_walls < 5)
    {
        return 6;
    }

    return 2;
}

small_int findBoardsizeOther(char* pseudo, float* max_time, float* scope)
{
    *scope = SCOPE;
    *max_time = 9.2;  // max branch, search time

    *pseudo = true;
    return 2;
}

/* The depth of the engine is calculated based on 3 parameters. The board size, the number of total moves played and the total number of available walls.
The strength of the engine is basically determined here, increase the depth numbers to make the engine more powerful, abeit slower */
small_int findDepth(cint boardsize, char* pseudo, float* max_time, float* scope, cint total_moves, cint total_walls)
{
    // first few moves do not really require much depth, just search at depth 1
    if (total_moves + 3 < boardsize)
    {
        *scope = SCOPE;
        return 1;
    }
    
    /* pseudodepth is used to simulate a further search to make the depth even.
    If pseudodepth is true the last step will only check opponent's "most likely"
    responses instead of all possible answers. This means it will check pawn movement
    and wall placement near the enemy pawn to disrupt it's movement */
    
    if (boardsize < 6)
        return findBoardsize5(pseudo, max_time, scope, total_walls);
    
    else if (boardsize < 8)
        return findBoardsize7(pseudo, max_time, scope, total_walls);
    
    else if (boardsize < 10)
        return findBoardsize9(pseudo, max_time, scope, total_walls);
    
    else if (boardsize < 12)
        return findBoardsize11(pseudo, max_time, scope, total_walls);
    
    else if (boardsize < 14)
        return findBoardsize13(pseudo, max_time, scope, total_walls);
    
    else if (boardsize < 16)
        return findBoardsize15(pseudo, max_time, scope, total_walls);
    
    else
        return findBoardsizeOther(pseudo, max_time, scope);
}