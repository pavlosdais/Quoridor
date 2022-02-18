#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//#define ABS(a, b) (a-b > 0) ? a-b:b-a

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
    if (wallOnTheLeft(i, j, wall_matrix, boardsize) ) return 1;
    else if (wallOnTheLeft(i, j, wall_matrix, boardsize) ) return 1;
    else if (wallAbove(i, j, wall_matrix, boardsize) ) return 1;
    else if (wallBelow(i, j, wall_matrix, boardsize) ) return 1;
    else return 0;
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
char recursiveSolveBlack(int x, int y, char **maze, int size);
char recursiveSolveWhite(int x, int y, char **have_visited, char **maze, int size);
void free_grid(char **A, int size);

char there_is_a_path(char **wall_matrix, int boardsize, player *white, player *black)
{
    
    int i, j;

    /*
    char **have_visited = malloc(boardsize*sizeof(char*));

    for (i = 0; i < boardsize; i++)
    {
        have_visited[i] = calloc(boardsize, sizeof(char));
    }
    */
    char f = recursiveSolveBlack(black->i, black->j, wall_matrix, boardsize);
    
    /*
    if (boardsize - white->i - 1 <= black->i)
    {
        f = recursiveSolveWhite(white->i, white->j, have_visited, wall_matrix, boardsize);
    }
    else // black
    {
        f = recursiveSolveBlack(black->i, black->j, have_visited, wall_matrix,  boardsize);
    }
    */
    // free_grid(have_visited, boardsize);
    return f;
}

// a3 c3 e3 g3
// playwall white h4 vertical
// playwall white h5 horizontal

// depth-first search
char recursiveSolveBlack(int x, int y, char **maze, int size)
{
    // black wins when the pawn gets to the first row

    if (x == 0) return 1;  // black has reached the end
    
    printf("%d %d\n", x, y);
    // search up
    if (x != size - 1 && !wallAbove(x, y, maze, size))
    {
        return recursiveSolveBlack(x+1, y, maze, size);
    }
    // search left
    if (y!= 0 && !wallOnTheLeft(x, y, maze, size))
    {
        return recursiveSolveBlack(x, y-1,  maze, size);
    }
    // search right
    if (y != size - 1 && !wallOnTheRight(x, y, maze, size))
    {
        return recursiveSolveBlack(x, y+1, maze, size);
    }
    // search down
    if (!wallBelow(x, y, maze, size))
    {
        return recursiveSolveBlack(x-1, y, maze, size);
    }
    
    return 0;
}

// depth-first search
char recursiveSolveWhite(int x, int y, char **have_visited, char **maze, int size)
{
    // white wins when the pawn gets to the last row

    if (x == size - 1) return 1;  // white has reached the end
    if (have_visited[x][y]) return 0;

    have_visited[x][y] = 1;

    // search down
    if (y != size - 1)
    {
        return recursiveSolveWhite(x-1, y, have_visited, maze, size);
    }
    // search left
    if (x != 0)
    {
        return recursiveSolveWhite(x, y-1, have_visited, maze, size);
    }
    // search right
    if (recursiveSolveWhite(x+1, y, have_visited, maze, size))
    {
        return 1;
    }
    // search down
    if (y != size - 1)
    {
        if (recursiveSolveWhite(x, y+1, have_visited, maze, size))
        {
            return 1;
        }
    }

    return 0;
}

void free_grid(char **A, int size)
{
    for (int i = 0; i < size; i++)
    {
        free(A[i]);
    }
    free(A);
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
