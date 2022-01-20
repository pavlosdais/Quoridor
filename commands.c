#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 50

struct position
{
    /*
    i and j will follow the matrix numbering, from 0 to n-1, and will refer to the cell (i+1,j+1)
    eg if black.i is 3 and black.j is 6, it means that the black pawn is on (4,7) or else H4
    */
    int i;
    int j;
};

typedef struct position position;

char command_num(char *ans)
{
    if (strcmp("name", ans) == 0) return 1;
    else if (strcmp("known_command", ans) == 0) return 2;
    else if (strcmp("list_commands", ans) == 0) return 3;
    else if (strcmp("quit", ans) == 0) return 4;
    else if (strcmp("boardsize", ans) == 0) return 5;
    else if (strcmp("clear_board", ans) == 0) return 6;
    else if (strcmp("walls", ans) == 0) return 7;
    else if (strcmp("playmove", ans) == 0) return 8;
    else if (strcmp("playwall", ans) == 0) return 9;
    else if (strcmp("genmove", ans) == 0) return 10;
    else if (strcmp("undo", ans) == 0) return 11;
    else if (strcmp("winner", ans) == 0) return 12;
    else if (strcmp("showboard", ans) == 0) return 13;
    else return 14;
}

void unsuccessful_response(char *msg)
{
    printf("? %s\n\n", msg);
    fflush(stdout);
}

void list_commands()
{
    printf("=\nname\nknown_command\nlist_commands\nquit\nboardsize\n");
    fflush(stdout);
    printf("clear_board\nwalls\nplaymove\nplaywall\ngenmove\nundo\nwinner\nshowboard\n\n");
    fflush(stdout);
}

void successful_command(char *msg)
{
    printf("=%s\n\n", msg);
    fflush(stdout);
}

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

void known_command(char *command)
{
    char temp[BUFFER_SIZE];
    int i = 0;
    while (command[i] != '\n' && command[i] != '\0')
    {
        temp[i] = command[i];
        i++;
    }
    temp[i] = '\0';

    char m = command_num(temp);
    if(m >= 1 && m <= 13)
    {
        successful_command(" true");
    }
    else
    {
        unsuccessful_response("false");
    }
}

void showboard(char **w_mtx, int boardsize, int black_walls, int white_walls, position *black, position *white)
{
    /*min field width is the greatest power of 10 in which when 10 is raised gives a result less than or equal to boardsize
    More simply, it is the number of digits of boardsize, eg. boardsize 9 -> mfw 1, boardsize 10 -> mfw 2*/
    int mfw = 0;
    int tempb = boardsize;
    while (tempb>0)
    {
        mfw++;
        tempb/=10;
    }
    
    int i, j, ch;
    // letters above
    for (i = 1; i <= mfw+1; i++) putchar(' ');
    for (i = 1; i <= boardsize; i++) printf("  %c ", 'A'+i-1);
    putchar('\n');
    
    // top edge
    for (i=1; i<= mfw+1; i++) putchar(' ');
    for (i=1; i<=boardsize; i++) printf("+---");
    printf("+\n");
    
    // main board
    for (i = boardsize-1; i >= 0; i--)
    {
        // printing the cell contents and the seperating lines/walls
        printf("%*d |", mfw, i+1);
        for (j = 0; j <= boardsize-1; j++)
        {
            // the cell content
            putchar(' ');
            if (black->i == i && black->j == j) putchar('B');
            else if (white->i == i && white->j == j) putchar('W');
            else putchar(' ');
            putchar(' ');
            
            if (j==boardsize-1) break;
            
            // the vertical seperating line/wall
            if (w_mtx[i][j]=='r') putchar('H');
            else if (i<boardsize-1 && w_mtx[i+1][j]=='r') putchar('H');
            else putchar('|');
        }
        printf("| %-*d  ", mfw, i+1);
        if (i==boardsize-1) printf("Black walls: %d", black_walls);
        else if (i==boardsize-2) printf("White walls: %d", white_walls);
        putchar('\n');
        
        if (i==0) break;  // so that the bottom edge is printed without checking for walls
        
        // printing grid lines 
        for (j = 1; j <= mfw+1; j++) putchar(' ');
        putchar('+');
        for (j = 0; j <= boardsize-1; j++)
        {
            //the horizontal seperating lines/walls
            if (w_mtx[i][j]=='b') ch = '=';
            else if (j>0 && w_mtx[i][j-1]=='b') ch = '=';
            else ch = '-';
            printf("%c%c%c", ch, ch, ch);
            
            if (j==boardsize-1) break;
            
            // the intersection of grid lines
            if(w_mtx[i][j]=='b') putchar('=');
            else if (w_mtx[i][j]=='r') putchar('H');
            else putchar('+');
        }
        printf("+\n");
    }
    
    // bottom edge
    for (i=1; i<= mfw+1; i++) putchar(' ');
    for (i=1; i<=boardsize; i++) printf("+---");
    printf("+\n");
    
    // letters below
    for (i=1; i<= mfw+1; i++) putchar(' ');
    for (i=1; i<=boardsize; i++) printf("  %c ", 'A'+i-1);
    printf("\n\n");
}

char **allocate_memory(int boardsize)
{
    char **A = malloc(boardsize*sizeof(char *));
    if(A == NULL) return NULL;  // error allocating memory, exit
    for (int i = 0; i < boardsize; i++)
    {
        A[i] = calloc(boardsize, sizeof(char));  // error allocating memory, exit
        if (A[i] == NULL) return NULL;
    }
    return A;
}

void free_array(char **A, int boardsize)
{
    for (int i = 0; i < boardsize; i++) free(A[i]);
    free(A);
}

void reset_pawns(int boardsize, position *white, position *black)
{
    white->i = 0;
    white->j = boardsize / 2;
    black->j = boardsize / 2;
    black->i = boardsize - 1;
}

void clear_board(int boardsize, char **wall_matrix, position *white, position *black)
{
    for (int i = 0; i < boardsize; i++) 
        for (int j = 0; j < boardsize; j++) 
            wall_matrix[i][j] = 0;
    
    // white's and black's pawns return to their starting position
    reset_pawns(boardsize, white, black);
}

void update_boardsize(char* p, int *boardsize, int *prev_boardsize)
{
    if (*boardsize > 0) *prev_boardsize = *boardsize;
    *boardsize = atoi(p);
}

void update_walls(char *p, int *black_walls, int *white_walls, int* number_of_walls)
{
    if (isnumber(p))
    {
        *number_of_walls = atoi(p);

        *black_walls = *number_of_walls;
        *white_walls = *number_of_walls;
    }
    else
    {
        unsuccessful_response("invalid syntax");
    }
}