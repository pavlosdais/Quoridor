#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function Prototypes
char command_num(char *ans);
void unsuccessful_response(char *msg);
void list_commands();
void successful_command(char *msg);
char isnumber(char *n);
void known_command(char *command);
struct position;
void showboard(char **walls_matrix, int boardsize, int black_walls, int white_walls, struct position *black, struct position *white);
void reset(int boardsize, struct position *white, struct position *black);
char **allocate_memory(int boardsize);
void free_array(char **A, int boardsize);

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

struct position
{
    /*i and j will follow the matrix numbering, from 0 to n-1, and will refer to the cell (i+1,j+1)
    eg if black.i is 3 and black.j is 6, it means that the black pawn is on (4,7) or else H4*/
    int i;
    int j;
};

int main(void)
{
    char winner, buff[80], *p, m;
    int walls, i = 0, j, prev_boardsize;
    
    //default values
    int black_walls = 10, white_walls = 10, boardsize = 9;
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
    there. If wall_matrix[3][2] is 'b', the wall starting below C4 keeps going below D4, or if wall_matrix[4][3] is 'r', the walll starting on the
    right of D5 keeps going on the right of D4. The same applies for cells with 'r'/'b'. Even though a wall does not start
    beneath them / on their right, it is possiblr that a wall starts below their left cell / on the right of the cell above and keeps going adjacently
    to the specific cell.*/

    char **wall_matrix = allocate_memory(9);

    while (1)
    {
        fgets(buff, 80, stdin);

        // command
        char *p = strtok(buff, " ,.");
        char temp[80];
        i = 0;
        while (p[i] != '\n' && p[i] != '\0')
        {
            temp[i] = p[i];
            i++;
        }
        temp[i] = '\0';

        char m = command_num(temp);
        
        if (m == 1)  // name
        {
            printf("IP Quoridor\n");
            fflush(stdout);
        }
        else if (m == 2)  // known_command
        {
            p = strtok(NULL, " ");
            known_command(p);
        }
        else if (m == 3)  // list_commands
        {
            list_commands();
        }
        else if (m == 4)  // quit
        {
            break;
        }
        else if (m == 5)  // boardsize
        {
            p = strtok(NULL, " ");
            if (isnumber(p))
            {
                prev_boardsize = boardsize;
                boardsize = atoi(p);
                if (boardsize <= 0)
                {
                    boardsize = -1;
                    unsuccessful_response("unacceptable size");
                }
                else
                {
                    // free previous matrix
                    free_array(wall_matrix, prev_boardsize);

                    // allocate memory for the new matrix
                    wall_matrix = allocate_memory(boardsize);
                    reset(boardsize, &white, &black);
                }
            }
            else
            {
                unsuccessful_response("invalid syntax");
            }
        }
        else if (m == 6)  // clear_board
        {
            for (i = 0; i < boardsize; i++) for (j = 0; j < boardsize; j++) wall_matrix[i][j] = 0;
            reset(boardsize, &white, &black);
            successful_command("");
        }
        else if (m == 7)  // walls
        {
            p = strtok(NULL, " ");
            if (isnumber(p))
            {
                walls = atoi(p);
                black_walls = walls;
                white_walls = walls;
            }
            else
            {
                unsuccessful_response("invalid syntax");
            }
        }
        else if (m == 8) // playmove
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
            showboard(wall_matrix, boardsize, black_walls, white_walls, &black, &white);
            fflush(stdout);      
        }
        else  // command not recognized
        {
            unsuccessful_response("unknown command");
        }
    }
    free_array(wall_matrix, boardsize);
    return 0;
}

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
    char temp[80];
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

void showboard(char **w_mtx, int boardsize, int black_walls, int white_walls, struct position *black, struct position *white)
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
    //letters above
    for (i = 1; i <= mfw+1; i++) putchar(' ');
    for (i = 1; i <= boardsize; i++) printf("  %c ", 'A'+i-1);
    putchar('\n');
    
    //top edge
    for (i=1; i<= mfw+1; i++) putchar(' ');
    for (i=1; i<=boardsize; i++) printf("+---");
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
            
            if (j==boardsize-1) break;
            
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
    for (i=1; i<=boardsize; i++) printf("+---");
    printf("+\n");
    
    //letters below
    for (i=1; i<= mfw+1; i++) putchar(' ');
    for (i=1; i<=boardsize; i++) printf("  %c ", 'A'+i-1);
    printf("\n\n");
}

void reset(int boardsize, struct position *white, struct position *black)
{
    white->i = 0;
    white->j = boardsize / 2;
    black->j = boardsize / 2;
    black->i = boardsize - 1;
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
