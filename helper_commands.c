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
    {
        return 1;
    }
    return 0;
}

player *check_color(char *p, player *black, player *white)
{
    // function returns the player with the corresponding color
    if (!strcmp(p, "white")) return white;
    else if (!strcmp(p, "w")) return white;
    else if (!strcmp(p, "black")) return black;
    else if (!strcmp(p, "b")) return white;
    else return NULL;
}

char check_orientation(char *orientation)
{
    // function returns b for horizontal, r for vertical and -1 for unknown orientation
    if (!strcmp(orientation, "horizontal")) return 'b';
    else if (!strcmp(orientation, "h")) return 'b';
    else if (!strcmp(orientation, "vertical")) return 'r';
    else if (!strcmp(orientation, "v")) return 'r';
    return -1;
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
    char steps = bfs(boardsize, wall_matrix, black->i, black->j, 0);  // black wins if he gets to the first row
    if (steps == -1) return 0;

    steps = bfs(boardsize, wall_matrix, white->i, white->j, boardsize-1);  // white wins if he gets to the last row
    if (steps == -1) return 0;

    return 1;
}

void addMove(stackptr *last, int i, int j, char *type)
{
    stackptr temp = *last;
    *last = malloc(sizeof(struct stacknode));
    (*last)->i = i;
    (*last)->j = j;
    (*last)->type = type;
    (*last)->next = temp;
}
