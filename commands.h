typedef struct position position;

#define BUFFER_SIZE 50

// Function Prototypes
char command_num(char *ans);
void unsuccessful_response(char *msg);
void list_commands();
void successful_command(char *msg);
char isnumber(char *n);
void known_command(char *command);
void showboard(char **walls_matrix, int boardsize, int black_walls, int white_walls, position *black, position *white);
void reset_pawns(int boardsize, position *white, position *black);
char **allocate_memory(int boardsize);
void free_array(char **A, int boardsize);
void clear_board(int boardsize, char **wall_matrix, position *white, position *black);
void update_boardsize(char* p, int *boardsize, int *prev_boardsize);
void update_walls(char *p, int *black_walls, int *white_walls, int* number_of_walls);

struct position
{
    /*
    i and j will follow the matrix numbering, from 0 to n-1, and will refer to the cell (i+1,j+1)
    eg if black.i is 3 and black.j is 6, it means that the black pawn is on (4,7) or else H4
    */
    int i;
    int j;
};