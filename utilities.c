#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "structs.h"
#include "bfs.h"

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

char is_vertex_valid(char hor, int boardsize)
{
    if (hor >= 0 && hor < boardsize)
        return 1;
    return 0;
}

player *check_color(char* p, player* black, player* white)
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

char colorValue(char* p)
{
    // returns the color's value
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
    if (argument == NULL)  // not a valid argument
    {
        printf("? %s\n\n", "invalid syntax");
        fflush(stdout);
        return 0;
    }
    // in any other case, a valid argument
    return 1;
}

char there_is_a_path(char** wall_matrix, int boardsize, player* white, player* black, char pl)
{
    /* returns 1 if the path is not blocked for neither player, 0 if it's blocked for at least
    one player and 2 if there's an error in allocating enough memory for the calculation */

    int steps = bfs(boardsize, wall_matrix, white->i, white->j, boardsize-1);  // white wins if he gets to the last row
    if (steps == -1)  // the path is blocked for white
        return 0;
    else if (steps == -2)  // allocation failure
        return 2;

    steps = bfs(boardsize, wall_matrix, black->i, black->j, 0);  // black wins if he gets to the first row
    if (steps == -1)  // the path is blocked for black
        return 0;  
    else if (steps == -2)  // allocation failure
        return 2;
    
    // the path is not blocked for neither player
    return 1;
}

char wallBelow(int i, int j, char** wall_matrix, int boardsize)
{
    if (i==0) return 0;
    return (wall_matrix[i][j]=='b' || (j>0 && wall_matrix[i][j-1]=='b'));
}

char wallAbove(int i, int j, char **wall_matrix, int boardsize)
{
    if (i==boardsize-1) return 0;
    return wallBelow(i+1, j, wall_matrix, boardsize);
}

char wallOnTheRight(int i, int j, char **wall_matrix, int boardsize)
{
    if (j==boardsize-1) return 0;
    return (wall_matrix[i][j]=='r' || (i<boardsize-1 && wall_matrix[i+1][j]=='r'));
}

char wallOnTheLeft(int i, int j, char** wall_matrix, int boardsize)
{
    if (j==0) return 0;
    return wallOnTheRight(i, j-1, wall_matrix, boardsize);
}

char thereIsAWall(char or, char** wall_matrix, int boardsize, int vertex_x, int vertex_y)
{
    if (wall_matrix[vertex_x][vertex_y] != 0) return 1;
    if (or == 'b')
    {
        if (wall_matrix[vertex_x][vertex_y+1] == 'b') return 1;
        else if (vertex_y > 0 && wall_matrix[vertex_x][vertex_y-1] == 'b') return 1;
    }
    else  // if (or == 'r')
    {
        if (wall_matrix[vertex_x-1][vertex_y] == 'r') return 1;
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
    
    // in any other case, a valid wall placement
    return 1;
}

char addMove(stackptr* last, int i, int j, char* type)
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

/* positionEvaluation is a heuristic function used for the evaluation of the current position of the game.
It takes into account the distance each player needs in order to win (factor = 1), the number of steps each player
needs to get to the next row (factor = 0.4) as well as the number of walls each player has (factor = 0.5) in order
to give an advantage (or lack of) for a certain player. When it's positive it calculates that the position is
advantageous for white and when it's negative it calculates that black has an advantage. If it's 0 it means that
the position is equal so neither player has an advantage. */

char positionEvaluation(player black, player white, int boardsize, char** wall_matrix, int* evaluation)
{
    if (black.i == 0)  // black wins
    {
        *evaluation = -999999;
        return 1;
    }
    else if (white.i == boardsize -1)  // white wins
    {
        *evaluation = 999999;
        return 1;
    }

    // calculate the distance white needs to get to the end
    int whiteDistanceFromWinning = bfs(boardsize, wall_matrix, white.i, white.j, boardsize-1);
    if (whiteDistanceFromWinning == -2) return 0;

    // calculate the distance black needs to get to the end
    int blackDistanceFromWinning = bfs(boardsize, wall_matrix, black.i, black.j, 0);
    if (blackDistanceFromWinning == -2) return 0;

    // calculate the distance white needs to get to the next row
    int whiteDistanceFromNextRow = bfs(boardsize, wall_matrix, white.i, white.j, white.i+1);
    if (whiteDistanceFromNextRow == -2) return 0;

    // calculate the distance black needs to get to the end
    int blackDistanceFromNextRow = bfs(boardsize, wall_matrix, black.i, black.j, black.i-1);
    if (blackDistanceFromNextRow == -2) return 0;

    *evaluation = 10*(blackDistanceFromWinning-whiteDistanceFromWinning) + 5*(blackDistanceFromNextRow - whiteDistanceFromNextRow) + 7*(white.walls - black.walls);
    return 1;
}

unsigned char findDepth(int boardsize, char* pseudo)
{
    /* pseudodepth is used to simulate a further search to make the depth even.
    If pseudodepth is true the last step will only check opponent's "most likely"
    responses instead of all possible answers. This means it will check pawn movement
    and wall placement near the enemy pawn to disrupt it's movement */
    if (boardsize <= 5)  // depth 6
    {
        *pseudo = 0;
        return 6;
    }
    else if (boardsize <= 7)  // depth 4
    {
        *pseudo = 0;
        return 4;
    }
    else if (boardsize <= 9)  // pseudo depth works at 4
    {
        *pseudo = 1;
        return 4;
    }
    else if (boardsize <= 11)  // depth 3
    {
        *pseudo = 0;
        return 3;
    }
    else if (boardsize <= 15)  // depth 3
    {
        *pseudo = 0;
        return 3;
    }
    else  // pseudodepth works at 2
    {
        *pseudo = 1;
        return 2;
    }
}
