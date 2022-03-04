#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "structs.h"
#include "utilities.h"
#include "ai.h"

// Function Prototypes
void unsuccessful_response(char *msg);
void successful_response(char *msg);
char **allocate_memory(int boardsize);
void free_array(char **A, int boardsize);
char command_num(char *ans);
void reset_pawns(int boardsize, player *white, player *black);

void print_name(char *p)
{
    successful_response(p);
}

void known_command()
{
    char *p = strtok(NULL, " ");
    char m = command_num(p);

    if (m >= 1 && m <= 13)
        successful_response("true");
    else
        unsuccessful_response("false");
}

void list_commands()
{
    printf("=\nname\nknown_command\nlist_commands\nquit\nboardsize\n");
    fflush(stdout);
    printf("clear_board\nwalls\nplaymove\nplaywall\ngenmove\nundo\nwinner\nshowboard\n\n");
    fflush(stdout);
}

char update_boardsize(int* boardsize, int* prev_boardsize, char*** wall_matrix, player* white, player* black, stackptr* history, int* totalmoves)
{
    char *p = strtok(NULL, " ");
    if (!enough_arguments(p)) return 2;
    if (isNumber(p) && p[0] != '-')
    {
        *boardsize = atoi(p);

        // free previous grid
        free_array(*wall_matrix, *prev_boardsize);
        free(*wall_matrix);

        // allocate memory for the new grid
        *wall_matrix = allocate_memory(*boardsize);
        if (wall_matrix == NULL)
		{
            unsuccessful_response("allocation failure");
			return 0;
		}

        reset_pawns(*boardsize, white, black);

        *totalmoves = 0;
        stackptr temp = NULL;
        while (*history != NULL)
        {
            temp = *history;
            history = &((*history)->next);
            free(temp);
        }
        *prev_boardsize = *boardsize;
        successful_response("");
    }
    else
	{
        unsuccessful_response("invalid syntax");
		return 2;
	}
	return 1;
}

void clear_board(int boardsize, char** wall_matrix, player white, player black, stackptr* history, int *totalmoves)
{
    for (int i = 0; i < boardsize; i++) 
        for (int j = 0; j < boardsize; j++) 
            wall_matrix[i][j] = 0;
    
    // white's and black's pawns return to their starting position
    reset_pawns(boardsize, &white, &black);
    *totalmoves = 0;

    // clear history
    stackptr temp = NULL;
    while (*history != NULL)
    {
        temp = *history;
        history = &((*history)->next);
        free(temp);
    }
    successful_response("");
}

void update_walls(player* white, player* black, int* number_of_walls)
{
    char *p = strtok(NULL, " ");   // number of walls
    if (!enough_arguments(p)) return;

    if (isNumber(p) && p[0] != '-')
    {
        *number_of_walls = atoi(p);
        black->walls = *number_of_walls;
        white->walls = *number_of_walls;
        successful_response("");
    }
    else
        unsuccessful_response("invalid syntax");
}

void playmove(char* buff, player* white, player* black, char** wall_mtx, int boardsize, stackptr* lastaddr, int* totalmoves)
{
    // Get color
    char *p = strtok(NULL, " ");
    if (!enough_arguments(p)) return;

    player *pl = check_color(p, black, white);  // player
    player *op = (pl == white) ? black : white;  // opponent
    if (pl == NULL)
    {
        unsuccessful_response("invalid syntax");
        return;
    }

    // Get vertex
    if (!enough_arguments(p)) return;
    p = strtok(NULL, " ");

    char vertex_y = p[0] - 'a';
    char vertex_x = atoi(p+1) - 1;

    if (!is_vertex_valid(vertex_x, boardsize) || !is_vertex_valid(vertex_y, boardsize))
    {
        unsuccessful_response("illegal move");
        return;
    }
    if (vertex_x == op->i && vertex_y == op->j)
    {
        unsuccessful_response("illegal move");
        return;
    }

    char ok;
    unsigned int dist = abs(pl->i - vertex_x) + abs(pl->j - vertex_y);

    if (dist > 2 || dist == 0)
    {
        unsuccessful_response("illegal move");
        return;
    }
    else if (dist == 1)
    {
        if (vertex_x == pl->i)
            ok = ((vertex_y > pl->j && !wallOnTheRight(pl->i, pl->j, wall_mtx, boardsize)) || (vertex_y < pl->j && !wallOnTheLeft(pl->i, pl->j, wall_mtx, boardsize)));
        else  // vertex_y == pl->j
            ok = ((vertex_x > pl->i && !wallAbove(pl->i, pl->j, wall_mtx, boardsize)) || (vertex_x < pl->i && !wallBelow(pl->i, pl->j, wall_mtx, boardsize)));
    }
    else  // dist == 2
    {
        if (vertex_x == pl->i && op->i == pl->i && pl->j+vertex_y == 2*op->j) // in the same row, with opponent in the middle
        {
            ok = !wallOnTheRight(op->i, op->j, wall_mtx, boardsize) && !wallOnTheLeft(op->i, op->j, wall_mtx, boardsize);
        }
        else if (vertex_y == pl->j && op->j == pl->j && pl->i+vertex_x == 2*op->i) // in the same column, with opponent in the middle
        {
            ok = !wallAbove(op->i, op->j, wall_mtx, boardsize) && !wallBelow(op->i, op->j, wall_mtx, boardsize);
        }
        else if (vertex_x != pl->i && vertex_y != pl->j)  // diagonally by 1 vertex
        {
            if (vertex_x == op->i && pl->j == op->j)
            {
                ok = (op->i > pl->i && (wallAbove(op->i, op->j, wall_mtx, boardsize) || op->i == boardsize-1)) || (op->i < pl->i && (wallBelow(op->i, op->j, wall_mtx, boardsize) || op->i == 0));
                if (vertex_y < op->j && wallOnTheLeft(op->i, op->j, wall_mtx, boardsize) || vertex_y > op->j && wallOnTheRight(op->i, op->j, wall_mtx, boardsize)) ok = 0;
            }
            else if (vertex_y == op->j && pl->i == op->i)
            {
                ok = (op->j > pl->j && (wallOnTheRight(op->i, op->j, wall_mtx, boardsize) || op->j == boardsize-1)) || (op->j < pl->j && (wallOnTheLeft(op->i, op->j, wall_mtx, boardsize) || op->j == 0));
                if (vertex_x < op->i && wallBelow(op->i, op->j, wall_mtx, boardsize) || vertex_x > op->i && wallAbove(op->i, op->j, wall_mtx, boardsize)) ok = 0;
            }
            else ok = 0;
        }
        else ok = 0;
    }

    if (!ok)
    {
        unsuccessful_response("illegal move");
        return;
    }
    
    // add pawn movement to history
    char *type = (pl == white) ? "wm" : "bm";
    ok = addMove(lastaddr, pl->i, pl->j, type);  // adding pawn's position to history before moving the pawn
    if (ok) 
        (*totalmoves)++;
    else
    {
        unsuccessful_response("allocation failure");
        return;
    }
    pl->i = vertex_x;
    pl->j = vertex_y;
    successful_response("");    
}

char playwall(char* buff, player* white, player* black, char** wall_matrix, int boardsize, stackptr* lastaddr, int* totalmoves)
{
    // return 1 if wall was placed, 2 if there was an error in trying to place the wall and 0 for allocation error
    // Get color
    char *p = strtok(NULL, " ");
    if (!enough_arguments(p)) return 2;
    
    player *pl = check_color(p, black, white);
    if (pl == NULL)
    {
        unsuccessful_response("invalid syntax");
        return 2;
    }
    
    // Get vertex
    p = strtok(NULL, " ");
    if (!enough_arguments(p)) return 2;
    char vertex_y = p[0] - 'a';
    char vertex_x = atoi(p+1) - 1;

    // Get orientation
    p = strtok(NULL, " ");
    if (!enough_arguments(p)) return 2;

    char orientation = check_orientation(p);
    if (orientation == -1)  // invalid orientation
    {
        unsuccessful_response("invalid syntax");
        return 2;
    }
    else if (!isValidWall(vertex_x, vertex_y, boardsize, wall_matrix, orientation))
    {
        unsuccessful_response("illegal move");
        return 2;
    }

    wall_matrix[vertex_x][vertex_y] = orientation;  // place wall
    
    // check to see if by placing the wall the path towards the end is blocked for either player
    char path = there_is_a_path(wall_matrix, boardsize, white, black);

    if (!path)  // by placing the wall the path is blocked for at least a player
    {
        wall_matrix[vertex_x][vertex_y] = 0;    // reset placing the wall
        unsuccessful_response("illegal move");
        return 2;
    }
    else if (path == -1)  // error in allocating memory to search if the path is being blocked
    {
        wall_matrix[vertex_x][vertex_y] = 0;    // reset placing the wall
        return 0;
    }
    (pl->walls)--;
    
    // add wall placement to history
    char *type = (pl == white) ? "ww" : "bw";
    char ok = addMove(lastaddr, vertex_x, vertex_y, type);
    if (ok)
        (*totalmoves)++;
    else
    {
        wall_matrix[vertex_x][vertex_y] = 0;    // reset placing the wall
        (pl->walls)++;
        return 0;
    }    
    successful_response("");
    return 1;
}

char genmove(player* white, player* black, char** wall_matrix, int boardsize, stackptr* lastaddr, int* totalmoves)
{
    // return 2 for random error, 0 for allocation error, 1 if all went good
    char *p = strtok(NULL, " ");
    if (!enough_arguments(p)) return 2;
    
    char pl;
    if ((pl=colorValue(p)) == -1)
    {
        unsuccessful_response("invalid syntax");
        return 2;
    }
	char pseudodepth;
	char depth = findDepth(boardsize, &pseudodepth);
    returningMove evalMove = bestMove(wall_matrix, boardsize, pl, black, white, depth, pseudodepth);
    if (evalMove.move == 'w')  // ai evaluated placing a wall
    {
        wall_matrix[evalMove.x][evalMove.y] = evalMove.or;
        if (pl == 'w') --white->walls;
        else --black->walls;

        if (evalMove.or == 'b')  // horizontal
            printf("= %c%d %c\n\n", 'A'+evalMove.y, evalMove.x+1, 'h');
            
        else  // vertical
            printf("= %c%d %c\n\n", 'A'+evalMove.y, evalMove.x+1, 'v');
        
        // add wall placement to history
        char *type = (pl == 'w') ? "ww" : "bw";
        char ok = addMove(lastaddr, evalMove.x, evalMove.y, type);
        if (ok) (*totalmoves)++;
    }
    else if (evalMove.move == 'm')  // ai evaluated a pawn movement
    {
        char *type = (pl == 'w') ? "wm" : "bm";
        char ok;
        if (pl == 'w')
        {
            ok = addMove(lastaddr, white->i, white->j, type);  // adding pawn's position to history before moving the pawn
            white->i = evalMove.x;
            white->j = evalMove.y;
        }
        else
        {
            ok = addMove(lastaddr, black->i, black->j, type);  // adding pawn's position to history before moving the pawn
            black->i = evalMove.x;
            black->j = evalMove.y;
        }
        if (ok) (*totalmoves)++;
        printf("= %c%d\n\n", 'A'+evalMove.y, evalMove.x+1);
    }
    else return false;  // memory allocation problem
    fflush(stdout);
    return 1;
}

void undo(char** wall_matrix, player* white, player* black, stackptr* last, int* totalmoves)
{    
    int times;
    char *p = strtok(NULL, " ");
    if (p == NULL) times = 1;
    else times = atoi(p);
    
    if (*totalmoves < times)
    {
        unsuccessful_response("cannot undo");
        return;
    }

    for (int i = 1; i <= times; i++)
    {
        if (strcmp((*last)->type,"bm") == 0)
        {
            black->i = (*last)->i;
            black->j = (*last)->j;
        }
        else if (strcmp((*last)->type,"wm") == 0)
        {
            white->i = (*last)->i;
            white->j = (*last)->j;
        }
        else if (strcmp((*last)->type,"bw") == 0)
        {
            wall_matrix[(*last)->i][(*last)->j] = 0;
            (black->walls)++;
        }
        else  // strcmp((*last)->type,"bw") == 0
        {
            wall_matrix[(*last)->i][(*last)->j] = 0;
            (white->walls)++;
        }

        stackptr temp = *last;
        *last = (*last)->next;
        free(temp);
    }
    *totalmoves -= times;
    successful_response("");
}

void winner(player white, player black, int boardsize)
{
    if (white.i==boardsize-1)
        successful_response("true white");
    else if (black.i==0)
        successful_response("true black");
    else
        successful_response("false");
}

/* Wall_matrix is used to represent if a wall begins next to a specific cell. for example wall_matrix[2][5] informs us about whether or not
   a wall starts next to the cell (3,6) or else F3. For that purpose we use a character.
   If the character is 'b' it means that a horizontal wall of length 2 has been placed below the specific cell and the cell on its right.
   If the character is 'r' it means that a vertical wall of length 2 has been placed on the right of the specific cell and the cell below.
   If not the character is the one with ascii code 0, as initialized by the following calloc. Since no walls cannot stack on top of another 
   or somehow cross, it is not possible that there are walls starting both horizontally below and vertically on the right of the specific cell
   Some examples:
    1.  wall_matrix[3][7] == 'r' means that no horizontal wall starts below H4, but a vertical one does on its right. So there is a vertical 
        wall between H4 and I4, which keeps going between H3 and I3.
    2.  wall_matrix[5][2] == 'b' means that no vertical wall starts on the right of C6, but a horizontal one does below it. So there is a
        horizontal wall between C6 and C5, which keeps going between D6 and D5.
   !It is important to notice that if a cell is 0 it does not mean that there might not be walls below or on its right. For example, even though
   wall_matrix[3][3] might be 0, it does NOT necessarily mean that no wall EXISTS below or on the right of D4, but simply a wall does not START
   there. If wall_matrix[3][2] is 'b', the wall starting below C4 keeps going below D4, or if wall_matrix[4][3] is 'r', the wall starting on the
   right of D5 keeps going on the right of D4. The same applies for cells with 'r'/'b'. Even though a wall does not start
   beneath them / on their right, it is possible that a wall starts below their left cell / on the right of the cell above and keeps going
   adjacently to the specific cell. */
void showboard(char** wall_matrix, int boardsize, player* white, player* black)
{
    printf("=\n");
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
            if (wall_matrix[i][j]=='r') putchar('H');
            else if (i<boardsize-1 && wall_matrix[i+1][j]=='r') putchar('H');
            else putchar('|');
        }
        printf("| %-*d  ", mfw, i+1);
        if (i==boardsize-1) printf("Black walls: %d", black->walls);
        else if (i==boardsize-2) printf("White walls: %d", white->walls);
        putchar('\n');
        
        if (i==0) break;  // so that the bottom edge is printed without checking for walls
        
        // printing grid lines 
        for (j = 1; j <= mfw+1; j++) putchar(' ');
        putchar('+');
        for (j = 0; j <= boardsize-1; j++)
        {
            // the horizontal seperating lines/walls
            if (wall_matrix[i][j]=='b') ch = '=';
            else if (j>0 && wall_matrix[i][j-1]=='b') ch = '=';
            else ch = '-';
            printf("%c%c%c", ch, ch, ch);
            
            if (j==boardsize-1) break;
            
            // the intersection of grid lines
            if(wall_matrix[i][j]=='b') putchar('=');
            else if (wall_matrix[i][j]=='r') putchar('H');
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
    fflush(stdout);
}

// Functions needed in main program

void reset_pawns(int boardsize, player* white, player* black)
{
    // White's position
    white->i = 0;
    white->j = boardsize / 2;

    // Black's position
    black->i = boardsize - 1;
    black->j = boardsize / 2;
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
    else return 14;  // command not recognized
}

void successful_response(char* msg)
{
    printf("= %s\n\n", msg);
    fflush(stdout);
}

void unsuccessful_response(char* msg)
{
    printf("? %s\n\n", msg);
    fflush(stdout);
}

char **allocate_memory(int boardsize)
{
    char **A = malloc(boardsize*sizeof(char *));
    if (A == NULL) return NULL;  // error allocating memory, exit
    for (int i = 0; i < boardsize; i++)
    {
        A[i] = calloc(boardsize, sizeof(char));
        if (A[i] == NULL) return NULL;  // error allocating memory, exit
    }

    // no error in allocationg, return array
    return A;
}

void free_array(char** A, int boardsize)
{
    for (int i = 0; i < boardsize; i++)
        free(A[i]);
}

char command_preprocess(char* buff)
{
    // Get rid of whitespace characters and convert the command into lower case characters
    if (buff[0] == '#')  // returns 1 if the first character is hash sign
    {
        buff[0] = '\0';
        return 1;
    }
    int i = 0, j = 0;
    char whsp = 0;
    while (buff[i] != '\n' && buff[i] != '\0')
    {
        if (buff[i] == ' ' || buff[i] == '\t')
        {
            whsp = 1;
            i++;
            continue;
        }
        if (whsp)
            buff[--i] = ' ';

        if (isupper(buff[i]))
            buff[j] = tolower(buff[i]);
        else
            buff[j] = buff[i];
        
        i++;
        j++;
        whsp = 0;
    }
    buff[j] = '\0';
    return 0;
}
