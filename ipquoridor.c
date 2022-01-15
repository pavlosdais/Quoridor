#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function Prototypes
int mode(char *ans);
char valid_number_of_walls(int walls);
void unsuccessful_response(char *msg);
void list_commands();
void showboard(int walls_matrix, int boardsize, int black_walls, int white_walls);

struct position
{
    /*i and j will follow the matrix numbering, from 0 to n-1, and will refer to the cell (i+1,j+1)
    eg if black.i is 3 and black.j is 6, it means that the black pawn is on (4,7) or else H4*/
    int i;
    int j;
}

/*
    Commands:
    -Adminstrative
    1 - name
    2 - known_command
    3 - list_commands
    4 - quit
    -Setup
    5 - boardsize
    6 - clear_board
    7 - walls
    -Core Play
    8 - playmove
    9 - playwall
    10 - genmove
    11 - undo
    -Tournament
    12 - winner
    -Debug
    13 - showboard
*/
int main(void)
{
    char winner, buff[80], *p, m, *parameters;
    
    //default values
    int black_walls = 10, white_walls = 10
    int boardsize = 9, walls = 10;
    struct position black;
    struct position white;
    white.i = 0; //row with number 1
    white.j = 4; //column with number 5 (letter 'E')
    black.i = 8; //row with number 9
    black.j = 4; //column with number 5 (letter 'E')
    
    /*wall_matrix is used to represent if a wall begins next to a specific cell. for example wall_matrix[2][5] informs us
    about whether or not a wall starts next to the cell (3,6) or else F6. For that purpose we use a character.
    If the character is 'b' it means that a horizontal wall of length 2 has been placed below the specific cell and the cell on its right.
    If the character is 'r' it means that a vertical wall of length 2 has been placed on the right of the specific cell and the cell below.
    If not the character is the one with ascii code 0, as initialized by the following calloc. Since no walls cannot stack on top of another 
    or somehow cross, it is not possible that there are walls starting both horizontally below and vertically on the right of the specific cell
    Some examples:
        1.  wall_matrix[3][7]== 'r' means that no horizontal wall starts below H4, but a vertical one does on its right. So there is a vertical 
            wall between H4 and I4, which keeps going between H3 and I3.
        2.  wall_matrix[5][2]== 'b' means that no vertical wall starts on the right of C6, but a horizontal one does below it. So there is a
            horizontal wall between C6 and C5, which keeps going between D6 and D5.
    !It is important to notice that if a cell is 0 it does not mean that there might not be walls below or on its right. For example, even though
    wall_matrix[3][3] might be 0, it does NOT necessarily mean that no wall EXISTS below or on the right of D4, but simply a wall does not START
    there. If wall_matrix[3][2] is 'b' the wall starting at */
    
    int i;
    char **wall_matrix = malloc(9, sizeof(char *));
    for (i = 0; i < 9; i++)
    {
        wall_matrix[i] = calloc(9, sizeof(char))
    }
    
    while (1)
    {
        scanf("%s", buff);
        p = strtok_r(buff, " ", &parameters);
        m = mode(p);
        if (m == 1)  // name
        {
            printf("Entered 1\n");
        }
        else if (m == 2)  // known_command
        {
            printf("Entered 2\n");
        }
        else if (m == 3)  // list_commands
        {
            printf("Entered 3\n");
            list_commands();
        }
        else if (m == 4)  // quit
        {
            break;
        }
        else if (m == 5)  // boardsize
        {
            printf("Entered 5\n");
        }
        else if (m == 6)  // clear_board
        {
            printf("Entered 6\n");
        }
        else if (m == 7)  // walls
        {
            printf("Entered 7\n");
            while(p != NULL)
            {
                printf("%s\n", p);
                p = strtok(NULL, " ");
            }
        }
        else if (m == 8)  // playmove
        {
            printf("Entered 8\n");
        }
        else if (m == 9)  // playwall
        {
            printf("Entered 9\n");
        }
        else if (m == 10)  // genmove
        {
            printf("Entered 10\n");
        }
        else if (m == 11)  // undo
        {
            printf("Entered 11\n");
        }
        else if (m == 12)  // winner
        {
            printf("Entered 12\n");
        }
        else if (m == 13)  // showboard
        {
            printf("Entered 13\n");
            showboard(wall_matrix, boardsize, black_walls, white_walls, &black, &white);
            fflush(stdout);
        }
        else  // command not recognized
        {
            unsuccessful_response("unknown command");
        }

        /*
        while(p != NULL)
        {
            //printf("%s\n", p);
            p = strtok(NULL, " ");
        }
        */
    }
    return 0;
}

int mode(char *ans)
{
    if (strcmp("name", ans) == 0) return 1;
    else if (strcmp("known_command", ans) == 0) return 2;
    else if (strcmp("list_commands", ans) == 0) return 3;
    else if (strcmp("quit", ans) == 0) return 4;
    else if (strcmp("boardsize", ans) == 0) return 5;
    else if (strcmp("playwall", ans) == 0) return 6;
    else if (strcmp("walls", ans) == 0) return 7;
    else if (strcmp("playmove", ans) == 0) return 8;
    else if (strcmp("playwall", ans) == 0) return 9;
    else if (strcmp("genmove", ans) == 0) return 10;
    else if (strcmp("undo", ans) == 0) return 11;
    else if (strcmp("winner", ans) == 0) return 12;
    else if (strcmp("showboard", ans) == 0) return 13;
    else return -1;
}

char valid_number_of_walls(int walls)
{
    if (walls % 2 == 1)  // valid
        return 1; 
    return 0;  // non-valid
}

void unsuccessful_response(char *msg)
{
    printf("? %s\n\n", msg);
    fflush(stdout);
}

void list_commands()
{
    printf("protocol_version\nname\nversion\nknown_command\nlist_commands\nquit\nboardsize\n");
    printf("clear_board\nwalls\nplaymove\nplaywall\ngenmove\nundo\nwinner\nshowboard\n");
}

showboard(int w_mtx, int boardsize, int black_walls, int white_walls, struct position *black, struct position *white)
{
    /*min field width is the greatest power of 10 in which when 10 is raised gives a result less than or equal to boardsize
    More simply, it is the number of digits of boardsize, eg. boardsize 9 -> mfw 1, boardsize 10 -> mfw 2*/
    int pow = 1;
    int mfw = 0;
    while (boardsize%pow == 0)
    {
        mfw++;
        pow*= 10;
    }
    
    int i, j, ch;
    //letters above
    for (i = 1; i <= mfw+1; i++) putchar(' ');
    for (i = 1; i <= boardsize; i++) printf("  %c", 'A'+i-1);
    putchar('\n');
    
    //top edge
    for (i=1; i<= mfw+1; i++) putchar(' ');
    for (i=1; i<=boardsize; i++) printf("+---", 'A'+i-1);
    printf("+\n");
    
    //main board
    for (i = boardsize-1; i >= 0; i--)
    {
        //printing the cell contents and the seperating lines/walls
        printf("%*d |", mfw, i+1);
        for (j = 0; j <= boardsize-1; j++)
        {
            //the cell content
            putchar(' ');
            if (black->i == i && black->j == j) putchar('B');
            else if (white->i == i && white->j == j) putchar('W');
            else putchar(' ');
            putchar(' ');
            
            //the vertical seperating line/wall
            if (w_mtx[i][j]=='r') putchar('H');
            else if (i<boardsize-1 && w_mtx[i+1][j]=='r') putchar('H');
            else putchar('|');
        }
        printf(" %-*d  ", mfw, i+1);
        if (i==boardsize-1) printf("Black walls: %d", black_walls);
        else if (i==boardsize-2) printf("White walls: %d", white_walls);
        putchar('\n');
        
        if (i==0) break; //so that the bottom edge is printed without checking for walls
        
        //printing grid lines 
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
            
            //the intersection of grid lines
            if(w_mtx[i][j]=='b') putchar('=');
            else if (w_mtx[i][j]=='r') putchar('H');
            else putchar('+');
        }
        printf("+\n");
    }
    
    //bottom edge
    for (i=1; i<= mfw+1; i++) putchar(' ');
    for (i=1; i<=boardsize; i++) printf("+---", 'A'+i-1);
    printf("+\n");
    
    //letters below
    for (i=1; i<= mfw+1; i++) putchar(' ');
    for (i=1; i<=boardsize; i++) printf("  %c", 'A'+i-1);
    printf("\n\n");
}    
