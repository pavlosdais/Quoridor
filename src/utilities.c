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

char thereIsAWall(const char or, char** wall_matrix, const int boardsize, const int vertex_x, const int vertex_y)
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

char there_is_a_path(char** wall_matrix, const int boardsize, player* white, player* black)
{
    small_int walls_played = 3.5*boardsize - 11.5 - white->walls - black->walls;  // total number of walls placed

    if (walls_played < 4)  // if there are less than 4 walls played, there is no way a path is being blocked for either player
        return true;
    
    // check to see if a path for the goal row exists for both players
    char path_exists = dfs(boardsize, wall_matrix, white->i, white->j, boardsize-1, 'w');  // white wins if he gets to the last row
    if (!path_exists)
        return false;

    path_exists = dfs(boardsize, wall_matrix, black->i, black->j, 0, 'b');  // black wins if he gets to the first row
    if (!path_exists)
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
