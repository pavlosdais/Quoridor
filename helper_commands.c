#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bfs.h"

typedef struct player {
    int i;
    int j;
    int walls;
} player;

typedef struct stacknode *stackptr;
struct stacknode {
    int i,j;
    char *type; // 'b': black player left (i,j), 'w': white player left (i,j), 'n': new wall placed at (i,j)
    stackptr next;
};

char isNumber(char *n)
{
    int i = 0;
    if (n[0] == '-') i = 1;
    while(n[i] != '\n' && n[i] != '\0')
    {
        if (!isdigit(n[i]))
            return 0;  // not a number
        i++;
    }
    if (n[0] == '-' && i == 1) return 0;  // not a number
    return 1;
}

void swap_boardsize(char* p, int *boardsize, int *prev_boardsize)
{
    if (*boardsize > 0) 
    {
        *prev_boardsize = *boardsize;
        printf("=");
        fflush(stdout);
    }
    
    *boardsize = atoi(p);
}

char is_vertex_valid(char hor, int boardsize)
{
    if (hor >= 0 && hor < boardsize)
        return 1;
    return 0;
}

player *check_color(char *p, player *black, player *white)
{
    // returns the player with the corresponding color
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

char check_orientation(char *orientation)
{
    // returns b for horizontal, r for vertical and -1 for unknown orientation
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
    if (argument == NULL)   // not a valid argument
    {
        printf("? %s\n\n", "invalid syntax");
        fflush(stdout);
        return 0;
    }
    // in any other case, a valid argument
    return 1;
}

char there_is_a_path(char **wall_matrix, int boardsize, player *white, player *black, char pl)
{
    /* returns 1 if the path is not being blocked for neither player, -1 if it's blocked for at least one 
    player and -2 if there's an error in allocating enough memory for the calculation */

    int steps = bfs(boardsize, wall_matrix, black->i, black->j, 0);  // black wins if he gets to the first row
    if (steps == -1)
        return 0;  
    else if (steps == -2)
        return -1;
    steps = bfs(boardsize, wall_matrix, white->i, white->j, boardsize-1);  // white wins if he gets to the last row
    if (steps == -1)
        return 0;
    else if (steps == -2)
        return -1;

    return 1;
}

char addMove(stackptr *last, int i, int j, char *type)
{
    stackptr temp = *last;
    *last = malloc(sizeof(struct stacknode));
    if (*last == NULL)
    {
        printf("? allocation failure\n\n");
        fflush(stdout);
        return 0;
    }
    (*last)->i = i;
    (*last)->j = j;
    (*last)->type = type;
    (*last)->next = temp;
    return 1;
}

/*
positionEvaluation is a heuristic function is used for the evaluation of the current situation of the game.
It takes into account the distance each player from the end in order to win as well as the number of walls each player
has in order to give an advantage (or lack of) for a certain player. When it's positive it calculates that the position
iσ advantageous for white and when it's negative it calculates that black has an advantage. If it's 0 it means that
the position is equal so neither player has an advantage.
*/

char positionEvaluation(player* black, player* white, int boardsize, char** wall_matrix, float *evaluation)
{
    // calculate the distance white needs to get to the end
    int whiteDistance = bfs(boardsize, wall_matrix, white->i, white->j, boardsize-1);
    if (whiteDistance == -2)
    {
        printf("? allocation failure\n\n");
        fflush(stdout);
        return 0;
    }
    // calculate the distance black needs to get to the end
    int blackDistance = bfs(boardsize, wall_matrix, black->i, black->j, 0);
    if (blackDistance == -2)
    {
        printf("? allocation failure\n\n");
        fflush(stdout);
        return 0;
    }
    *evaluation = blackDistance-whiteDistance + 0.2*(white->walls-black->walls);
    return 1;
}

char wallBelow(int i, int j, char **w_mtx, int boardsize) {
    if (i==0) return 0;
    return (w_mtx[i][j]=='b' || (j>0 && w_mtx[i][j-1]=='b'));
}

char wallAbove(int i, int j, char **w_mtx, int boardsize) {
    if (i==boardsize-1) return 0;
    return wallBelow(i+1, j, w_mtx, boardsize);
}

char wallOnTheRight(int i, int j, char **w_mtx, int boardsize) {
    if (j==boardsize-1) return 0;
    return (w_mtx[i][j]=='r' || (i<boardsize-1 && w_mtx[i+1][j]=='r'));
}

char wallOnTheLeft(int i, int j, char **w_mtx, int boardsize) {
    if (j==0) return 0;
    return wallOnTheRight(i, j-1, w_mtx, boardsize);
}

char thereIsAWall(char or, char** wall_matrix, int boardsize, int vertex_x, int vertex_y)
{
    if (or == 'b')
    {
        if (wall_matrix[vertex_x][vertex_y] != 0) return 1;
        else if (wall_matrix[vertex_x][vertex_y+1] == 'b') return 1;
        else if (vertex_y > 0 &&wall_matrix[vertex_x][vertex_y-1] == 'b') return 1;
    }
    else // if (or == 'r')
    {
        if (wall_matrix[vertex_x][vertex_y] != 0) return 1;
        else if (wall_matrix[vertex_x-1][vertex_y] == 'r') return 1;
        else if (vertex_x < boardsize-1 && wall_matrix[vertex_x+1][vertex_y] == 'r') return 1;
    }
    return 0;
}

char isValidWall(int vertex_x, int vertex_y, int boardsize, char** wall_matrix, char orientation)
{
    if (!is_vertex_valid(vertex_x, boardsize) || !is_vertex_valid(vertex_y, boardsize) || vertex_x == 0 || vertex_y == boardsize-1)  // orientation out of bounds
        return 0;
    else if (thereIsAWall(orientation, wall_matrix, boardsize, vertex_x, vertex_y))  // there's already a wall there
        return 0;
    
    return 1;
}

float max(int a, int b)
{
    return a>b ? a:b;
}

float min(int a, int b)
{
    return a<b ? a:b;
}
