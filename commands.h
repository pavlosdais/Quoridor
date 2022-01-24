#define BUFFER_SIZE 81

// Function Prototypes
char command_num(char *ans);
void print_name(char *p);
void unsuccessful_response(char *msg);
void successful_response(char *msg);
void list_commands();
void known_command();
void showboard(char **walls_matrix, int boardsize, int black_walls, int white_walls, position *black, position *white);
char **allocate_memory(int boardsize);
void free_array(char **A, int boardsize);
void clear_board(int boardsize, char **wall_matrix, position *white, position *black);
void update_boardsize(int *boardsize, int *prev_boardsize, char ***wall_matrix, position *white, position *black);
void update_walls(int *black_walls, int *white_walls, int* number_of_walls);
void playwall(char *buff, position *white, position *black);
void command_preprocess(char *buff);
char arguments(char *buff);

struct player
{
    /*
    i and j will follow the matrix numbering, from 0 to n-1, and will refer to the cell (i+1,j+1)
    eg if black.i is 3 and black.j is 6, it means that the black pawn is on (4,7) or else H4
    */
    int i;
    int j;
    int walls;
};

typedef struct player player;
