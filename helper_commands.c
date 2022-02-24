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
    if (hor > 0 && hor < boardsize)
        return 1;
    return 0;
}

player *check_color(char *p, player *black, player *white)
{
    // function returns the player with the corresponding color
    if (!strcmp(p, "white")) 
        return white;
    else if (!strcmp(p, "w"))
        return white;
    else if (!strcmp(p, "black"))
        return black;
    else if (!strcmp(p, "b"))
        return white;
    else  // error
        return NULL;
}

char check_orientation(char *orientation)
{
    // function returns b for horizontal, r for vertical and -1 for unknown orientation
    if (!strcmp(orientation, "horizontal"))
        return 'b';
    else if (!strcmp(orientation, "h"))
        return 'b';
    else if (!strcmp(orientation, "vertical"))
        return 'r';
    else if (!strcmp(orientation, "v"))
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

char there_is_a_path(char **wall_matrix, int boardsize, player *white, player *black)
{
    int steps = bfs(boardsize, wall_matrix, black->i, black->j, 0);  // black wins if he gets to the first row
    if (steps == -1)
        return 0;  // path is blocked for black
    else if (steps == -2)
        return -1;  //allocation failed
    
    steps = bfs(boardsize, wall_matrix, white->i, white->j, boardsize-1);  // white wins if he gets to the last row
    if (steps == -1)
        return 0;  // path is blocked for white
    else if (steps == -2)
        return -1;  //allocation failed
    
    return 1;  // the path is not being blocked
}

void addMove(stackptr *last, int i, int j, char *type)
{
    stackptr temp = *last;
    *last = malloc(sizeof(struct stacknode));
    if (*last == NULL)
    {
        unsuccessful_response("allocation failure");
        return;
    }
    (*last)->i = i;
    (*last)->j = j;
    (*last)->type = type;
    (*last)->next = temp;
}

float positionEvaluation(player* black, player* white, int boardsize, char** wall_matrix)
{
    int whiteDistance = bfs(boardsize, wall_matrix, white->i, white->j, boardsize-1);
    int blackDistance = bfs(boardsize, wall_matrix, black->i, black->j, 0);
    if (whiteDistance == -2 || blackDistance == -2)
    {
        unsuccessful_response("allocation failure");
        return;
    }
    return whiteDistance-blackDistance + 0.2*(white->walls - black->walls);
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

char there_is_a_wall(int i, int j, char **wall_matrix, int boardsize)
{
    if (wallOnTheLeft(i, j, wall_matrix, boardsize)) return 1;
    else if (wallOnTheLeft(i, j, wall_matrix, boardsize)) return 1;
    else if (wallAbove(i, j, wall_matrix, boardsize)) return 1;
    else if (wallBelow(i, j, wall_matrix, boardsize)) return 1;
    else return 0;
}
