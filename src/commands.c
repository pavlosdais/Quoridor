#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include "../include/structs.h"
#include "../include/utilities.h"
#include "../include/typedefs.h"
#include "../include/engine.h"
#include "../include/zobrist_hashing.h"
#include "../include/transposition_table.h"

// function prototypes
void unsuccessful_response(char *msg);
void successful_response(char *msg);
char **allocate_memory(cint boardsize);
void free_array(char** A, const int boardsize);
char command_num(char *ans);
void reset_pawns(cint boardsize, player *white, player *black);


void print_name(char *p) { successful_response(p); }

gameState game_state_init(uint number_of_walls, uint boardsize)
{
    gameState new_game_state = malloc(sizeof(*new_game_state));
    assert(new_game_state != NULL);

    new_game_state->wall_matrix = allocate_memory(boardsize);
    assert(new_game_state != NULL);

    reset_pawns(boardsize, &(new_game_state->white), &(new_game_state->black));  // default walues

    new_game_state->totalmoves = 0;
    new_game_state->white.walls = number_of_walls; new_game_state->black.walls = number_of_walls;
    new_game_state->boardsize = boardsize;

    // create zobrist keys & get the starting position id
    createZobristKeys(new_game_state);
    generateStartingPositionID(new_game_state);

    // initialize the transposition table
    tt_init(&(new_game_state->game_tt));

    return new_game_state;
}

void destroy_game(gameState game_state)
{
    tt_destroy(game_state->game_tt);
    destroyZobristKeys(game_state);
    free_array(game_state->wall_matrix, game_state->boardsize);
    free(game_state->zobrist_values);
    free(game_state->wall_matrix);
    free(game_state);
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

char update_boardsize(gameState game_state, int* prev_boardsize, stackptr* history)
{
    char *p = strtok(NULL, " ");
    if (!enough_arguments(p)) return 2;
    if (isNumber(p) && p[0] != '-')
    {
        // destroy previous board keys
        // - since the boardsize is changing we need to create new ones
        destroyBoardKeys(game_state);

        game_state->boardsize = atoi(p);  // new boardsize

        // free previous grid
        free_array(game_state->wall_matrix, *prev_boardsize);
        free(game_state->wall_matrix);

        // allocate memory for the new grid
        game_state->wall_matrix = allocate_memory(game_state->boardsize);
        assert(game_state->wall_matrix != NULL);

        reset_pawns(game_state->boardsize, &(game_state->white), &(game_state->black));

        createBoardKeys(game_state);
        generateStartingPositionID(game_state);

        *prev_boardsize = game_state->boardsize;
	    game_state->totalmoves = 0;
        successful_response("");
    }
    else
    {
        unsuccessful_response("invalid syntax");
        return 2;
    }
    return 1;
}

void clear_board(gameState game_state)
{
    // clear board of the walls
    for (int i = 0; i < game_state->boardsize; i++) 
        for (int j = 0; j < game_state->boardsize; j++) 
            game_state->wall_matrix[i][j] = false;
    
    // white's and black's pawns return to their starting position
    reset_pawns(game_state->boardsize, &(game_state->white), &(game_state->black));
    game_state->totalmoves = 0;
    
    successful_response("");
}

void update_walls(gameState game_state)
{
    char *p = strtok(NULL, " ");   // number of walls
    if (!enough_arguments(p)) return;

    if (isNumber(p) && p[0] != '-')
    {
        int number_of_walls = atoi(p);
        destroyWallKeys(game_state);

        game_state->black.walls = number_of_walls;
        game_state->white.walls = number_of_walls;

        createWallKeys(game_state);

        generatePositionID(game_state);

        if (tt_size(game_state->game_tt) != 0)
            tt_clear(game_state->game_tt);
        
        successful_response("");
    }
    else
        unsuccessful_response("invalid syntax");
}

void playmove(gameState game_state, stackptr* history)
{
    // get color
    char *p = strtok(NULL, " ");
    if (!enough_arguments(p)) return;  // not enough arguments

    player *pl = check_color(p, &(game_state->black), &(game_state->white));  // player
    if (pl == NULL)
    {
        unsuccessful_response("invalid syntax");
        return;
    }
    
    player *op = (pl == &(game_state->black)) ? &(game_state->white) : &(game_state->black);  // opponent
    
    // Get vertex
    if (!enough_arguments(p)) return;
    p = strtok(NULL, " ");

    char vertex_y = p[0] - 'a';
    char vertex_x = atoi(p+1) - 1;
    
    char** wall_mtx = game_state->wall_matrix;
    uint boardsize = game_state->boardsize;

    if (!is_vertex_valid(vertex_x, boardsize) || !is_vertex_valid(vertex_y, boardsize) || (vertex_x == op->i && vertex_y == op->j))
    {
        unsuccessful_response("illegal move");
        return;
    }
    
    char ok;
    uint dist = abs(pl->i - vertex_x) + abs(pl->j - vertex_y);

    if (dist > 2 || dist == 0)
    {
        unsuccessful_response("illegal move");
        return;
    }
    else if (dist == 1)
    {
        if (vertex_x == pl->i)
            ok = ((vertex_y > pl->j && !wallOnTheRight(pl->i, pl->j, wall_mtx, boardsize)) ||
            (vertex_y < pl->j && !wallOnTheLeft(pl->i, pl->j, wall_mtx, boardsize)));
        else  // vertex_y == pl->j
            ok = ((vertex_x > pl->i && !wallAbove(pl->i, pl->j, wall_mtx, boardsize)) ||
            (vertex_x < pl->i && !wallBelow(pl->i, pl->j, wall_mtx, boardsize)));
    }
    else  // dist == 2
    {
        if (vertex_x == pl->i && op->i == pl->i && pl->j+vertex_y == 2*op->j) // in the same row, with opponent in the middle
            ok = !wallOnTheRight(op->i, op->j, wall_mtx, boardsize) && !wallOnTheLeft(op->i, op->j, wall_mtx, boardsize);
        else if (vertex_y == pl->j && op->j == pl->j && pl->i+vertex_x == 2*op->i) // in the same column, with opponent in the middle
            ok = !wallAbove(op->i, op->j, wall_mtx, boardsize) && !wallBelow(op->i, op->j, wall_mtx, boardsize);
        else if (vertex_x != pl->i && vertex_y != pl->j)  // diagonally by 1 vertex
        {
            if (vertex_x == op->i && pl->j == op->j)
            {
                ok = (op->i > pl->i && (wallAbove(op->i, op->j, wall_mtx, boardsize) || op->i == boardsize-1)) 
                || (op->i < pl->i && (wallBelow(op->i, op->j, wall_mtx, boardsize) || op->i == 0));

                if (vertex_y < op->j && wallOnTheLeft(op->i, op->j, wall_mtx, boardsize) || vertex_y > op->j && wallOnTheRight(op->i, op->j, wall_mtx, boardsize)) ok = 0;
            }
            else if (vertex_y == op->j && pl->i == op->i)
            {
                ok = (op->j > pl->j && (wallOnTheRight(op->i, op->j, wall_mtx, boardsize) || op->j == boardsize-1)) 
                || (op->j < pl->j && (wallOnTheLeft(op->i, op->j, wall_mtx, boardsize) || op->j == 0));

                if (vertex_x < op->i && wallBelow(op->i, op->j, wall_mtx, boardsize) || vertex_x > op->i && wallAbove(op->i, op->j, wall_mtx, boardsize)) ok = 0;
            }
            else ok = false;
        }
        else ok = false;
    }

    if (!ok)
    {
        unsuccessful_response("illegal move");
        return;
    }
    
    // hash pawn movement
    if (pl == &(game_state->white))  // white
    {
        addMove(history, pl->i, pl->j, "wm");
        game_state->position_id ^= game_state->zobrist_values->white_pawn[pl->i][pl->j];
        game_state->position_id ^= game_state->zobrist_values->white_pawn[vertex_x][vertex_y];
    }
    else  // black
    {
        addMove(history, pl->i, pl->j, "bm");
        game_state->position_id ^= game_state->zobrist_values->black_pawn[pl->i][pl->j];
        game_state->position_id ^= game_state->zobrist_values->black_pawn[vertex_x][vertex_y];
    }

    // move pawn
    pl->i = vertex_x;
    pl->j = vertex_y;

    (game_state->totalmoves)++;
    successful_response("");
}

void playwall(gameState game_state, stackptr* history)
{
    // Get color
    char *p = strtok(NULL, " ");
    if (!enough_arguments(p)) return;  // not enough arguments
    
    player *pl = check_color(p, &(game_state->black), &(game_state->white));

    if (pl == NULL)
    {
        unsuccessful_response("invalid syntax");
        return;
    }
    
    // Get vertex
    p = strtok(NULL, " ");
    if (!enough_arguments(p)) return;  // not enough arguments

    char vertex_y = p[0] - 'a';
    char vertex_x = atoi(p+1) - 1;

    // Get orientation
    p = strtok(NULL, " ");
    if (!enough_arguments(p)) return;  // not enough arguments

    char orientation = check_orientation(p);
    if (orientation == -1)  // invalid orientation
    {
        unsuccessful_response("invalid syntax");
        return;
    }
    else if (!isValidWall(vertex_x, vertex_y, game_state, orientation))
    {
        unsuccessful_response("illegal move");
        return;
    }

    game_state->wall_matrix[vertex_x][vertex_y] = orientation;  // place wall
    
    // check to see if by placing the wall the path towards the end is blocked for either player
    if (!there_is_a_path(game_state))  // by placing the wall the path is blocked for at least a player
    {
        game_state->wall_matrix[vertex_x][vertex_y] = 0;    // reset placing the wall
        unsuccessful_response("illegal move");
        return;
    }

    // hash wall placement
    if (orientation == 'b')  // horizontal
        game_state->position_id ^= game_state->zobrist_values->horizontal_walls[vertex_x][vertex_y];
    else  // vertical
        game_state->position_id ^= game_state->zobrist_values->vertical_walls[vertex_x][vertex_y];
    
    // hash wall value
    if (pl == &(game_state->white))  // white
    {
        addMove(history, vertex_x, vertex_y, "ww");
        game_state->position_id ^= game_state->zobrist_values->white_walls[game_state->white.walls];
        game_state->position_id ^= game_state->zobrist_values->white_walls[--(game_state->white.walls)];
    }
    else  // black
    {
        addMove(history, vertex_x, vertex_y, "bw");
        game_state->position_id ^= game_state->zobrist_values->black_walls[game_state->black.walls];
        game_state->position_id ^= game_state->zobrist_values->black_walls[--(game_state->black.walls)];
    }
    
    (game_state->totalmoves)++;
   
    successful_response("");
}

void genmove(gameState game_state, stackptr* lastaddr)
{
    // printf("White walls = %d, black walls = %d\n", game_state->white.walls, game_state->black.walls);

    char *p = strtok(NULL, " ");
    if (!enough_arguments(p)) return;
    
    char pl;
    if ((pl=colorValue(p)) == -1)
    {
        unsuccessful_response("invalid syntax");
        return;
    }
    
    // return and play the engine move
    returningMove evalMove = iterativeDeepening(game_state, pl);
    playGenMove(game_state, pl, evalMove, lastaddr);  // play engine move
    
    (game_state->totalmoves)++;
}

void undo(gameState game_state, stackptr* last)
{
    int times;
    char *p = strtok(NULL, " ");
    if (p == NULL) times = 1;
    else times = atoi(p);
    
    if (game_state->totalmoves < times)
    {
        unsuccessful_response("cannot undo");
        return;
    }

    for (int i = 0; i < times; i++)
    {
        if (strcmp((*last)->type, "bm") == 0)  // black pawn movement
        {
            game_state->position_id ^= game_state->zobrist_values->black_pawn[game_state->black.i][game_state->black.j];
            game_state->black.i = (*last)->i;
            game_state->black.j = (*last)->j;
            game_state->position_id ^= game_state->zobrist_values->black_pawn[game_state->black.i][game_state->black.j];
        }
        else if (strcmp((*last)->type, "wm") == 0)  // white pawn movement
        {
            game_state->position_id ^= game_state->zobrist_values->white_pawn[game_state->white.i][game_state->white.j];
            game_state->white.i = (*last)->i;
            game_state->white.j = (*last)->j;
            game_state->position_id ^= game_state->zobrist_values->white_pawn[game_state->white.i][game_state->white.j];
        }
        else  // wall placement
        {
            if (game_state->wall_matrix[(*last)->i][(*last)->j]  == 'b')  // horizontal
                game_state->position_id ^= game_state->zobrist_values->horizontal_walls[(*last)->i][(*last)->j];
            else
                game_state->position_id ^= game_state->zobrist_values->vertical_walls[(*last)->i][(*last)->j];
            
            if (strcmp((*last)->type, "bw") == 0)  // black wall placement
            {
                game_state->position_id ^= game_state->zobrist_values->black_walls[game_state->black.walls];
                game_state->position_id ^= game_state->zobrist_values->black_walls[(++(game_state->black.walls))];
                game_state->wall_matrix[(*last)->i][(*last)->j] = 0;
            }
            else  // white wall placement
            {
                game_state->position_id ^= game_state->zobrist_values->white_walls[game_state->white.walls];
                game_state->position_id ^= game_state->zobrist_values->white_walls[(++(game_state->white.walls))];
                game_state->wall_matrix[(*last)->i][(*last)->j] = 0;
            }
        }

        stackptr temp = *last;
        *last = (*last)->next;
        free(temp);
    }
    game_state->totalmoves -= times;
    successful_response("");
}

void winner(gameState game_state)
{
    if (game_state->white.i == game_state->boardsize-1)
        successful_response("true white");
    else if (game_state->black.i==0)
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
void showboard(gameState game_state)
{
    printf("=\n");
    /*min field width is the greatest power of 10 in which when 10 is raised gives a result less than or equal to boardsize
    More simply, it is the number of digits of boardsize, eg. boardsize 9 -> mfw 1, boardsize 10 -> mfw 2*/
    int mfw = 0;
    int tempb = game_state->boardsize;
    while (tempb>0)
    {
        mfw++;
        tempb/=10;
    }
    
    int i, j, ch;
    // letters above
    for (i = 1; i <= mfw+1; i++) putchar(' ');
    for (i = 1; i <= game_state->boardsize; i++) printf("  %c ", 'A'+i-1);
    putchar('\n');
    
    // top edge
    for (i=1; i<= mfw+1; i++) putchar(' ');
    for (i=1; i<=game_state->boardsize; i++) printf("+---");
    printf("+\n");
    
    // main board
    for (i = game_state->boardsize-1; i >= 0; i--)
    {
        // printing the cell contents and the seperating lines/walls
        printf("%*d |", mfw, i+1);
        for (j = 0; j <= game_state->boardsize-1; j++)
        {
            // the cell content
            putchar(' ');
            if (game_state->black.i == i && game_state->black.j == j) putchar('B');
            else if (game_state->white.i == i && game_state->white.j == j) putchar('W');
            else putchar(' ');
            putchar(' ');
            
            if (j==game_state->boardsize-1) break;
            
            // the vertical seperating line/wall
            if (game_state->wall_matrix[i][j]=='r') putchar('H');
            else if (i<game_state->boardsize-1 && game_state->wall_matrix[i+1][j]=='r') putchar('H');
            else putchar('|');
        }
        printf("| %-*d  ", mfw, i+1);
        if (i==game_state->boardsize-1) printf("Black walls: %d", game_state->black.walls);
        else if (i==game_state->boardsize-2) printf("White walls: %d", game_state->white.walls);
        putchar('\n');
        
        if (i==0) break;  // so that the bottom edge is printed without checking for walls
        
        // printing grid lines 
        for (j = 1; j <= mfw+1; j++) putchar(' ');
        putchar('+');
        for (j = 0; j <= game_state->boardsize-1; j++)
        {
            // the horizontal seperating lines/walls
            if (game_state->wall_matrix[i][j]=='b') ch = '=';
            else if (j>0 && game_state->wall_matrix[i][j-1]=='b') ch = '=';
            else ch = '-';
            printf("%c%c%c", ch, ch, ch);
            
            if (j==game_state->boardsize-1) break;
            
            // the intersection of grid lines
            if(game_state->wall_matrix[i][j]=='b') putchar('=');
            else if (game_state->wall_matrix[i][j]=='r') putchar('H');
            else putchar('+');
        }
        printf("+\n");
    }
    
    // bottom edge
    for (i=1; i<= mfw+1; i++) putchar(' ');
    for (i=1; i<=game_state->boardsize; i++) printf("+---");
    printf("+\n");
    
    // letters below
    for (i=1; i<= mfw+1; i++) putchar(' ');
    for (i=1; i<=game_state->boardsize; i++) printf("  %c ", 'A'+i-1);
    printf("\n\n");
    fflush(stdout);
}

// Functions needed in main program

void reset_pawns(cint boardsize, player* white, player* black)
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

char **allocate_memory(cint boardsize)
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

void free_array(char** A, cint boardsize)
{
    for (int i = 0; i < boardsize; i++)
        free(A[i]);
}

// Get rid of whitespace characters and convert the command into lower case characters
char command_preprocess(char* buff)
{
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
