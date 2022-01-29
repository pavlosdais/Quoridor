#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct player {
    int i;
    int j;
    int walls;
} player;

char isnumber(char *n)
{
    int i = 0;
    if (n[0] == '-') i = 1;
    while(n[i] != '\n' && n[i] != '\0')
    {
        if (!isdigit(n[i]))
        {
            return 0;
        }
        i++;
    }
    if (n[0] == '-' && i == 1) return 0;
    return 1;
}


void reset_pawns(int boardsize, player *white, player *black)
{
    white->i = 0;
    white->j = boardsize / 2;
    black->j = boardsize / 2;
    black->i = boardsize - 1;
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

char is_x_available(char hor, player white)
{
    if (hor >= 'A' && hor < (white.j + 'A'))
    {
        return 1;
    }
    else if (hor >= 'a' && hor < (white.j + 'a'))
    {
        return 1;
    }
    return 0;
}

char is_y_availabe(char hor, player black)
{
    if (hor >= 0 && hor < black.i)
    {
        return 1;
    }
    return 0;
}

player *check_color(char *p, player *black, player *white)
{
    // function returns 1 for white, 0 for black and -1 for unknown color

    if (strcmp(p, "white") == 0) return white;
    else if (strcmp(p, "w") == 0) return white;
    else if (strcmp(p, "black") == 0) return black;
    else if (strcmp(p, "b") == 0) return white;
    else return NULL;
}

char check_orientation(char *orientation)
{
    // function returns 1 for horizontal, 0 for vertical and -1 for uknown orientation
    if (strcmp(orientation, "horizontal")) return 1;
    else if (strcmp(orientation, "vertical")) return 0;
    else return -1;
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
