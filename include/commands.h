typedef struct player player;
typedef struct stacknode *stackptr;

///////////////////////
//  Ingame commands  //
///////////////////////

// prints the name of the program
void print_name(char *p);

// prints a message informing if the command read is recognized or not
void known_command(void);

// lists all possible commands
void list_commands();

// updates boardsize
char update_boardsize(int* boardsize, int* prev_boardsize, char*** wall_matrix, player* white, player* black, stackptr* history, int* totalmoves);

// clears board
void clear_board(int boardsize, char** wall_matrix, player* white, player* black, stackptr* history, int* totalmoves);

// updates walls for both players
void update_walls(player* white, player* black, int* number_of_walls);

// plays a pawn move for the player read
void playmove(char* buff, player* white, player* black, char** wall_matrix, const int boardsize, stackptr* history, int* totalmoves);

// plays a wall for the player read
void playwall(char* buff, player* white, player* black, char** wall_matrix, const int boardsize, stackptr* history, int* totalmoves);

// ai plays a move for the player read
void genmove(player* white, player* black, char** wall_matrix, int boardsize, stackptr* lastaddr, int* totalmoves);

// undoes moves the times read
void undo(char** wall_matrix, player* white, player* black, stackptr* last, int* totalmoves);

// responds accordingly if there is a winner
void winner(player white, player black, const int boardsize);

// draws board
void showboard(char** wall_matrix, const int boardsize, player* white, player* black);

// returns the "number" of the command
char command_num(char* ans);

// processes command: gets rid of whitespace characters and converts the command into lower case characters
char command_preprocess(char* buff);

// resets pawns to their original starting place
void reset_pawns(const int boardsize, player* white, player *black);

// allocates memory for the board
char **allocate_memory(const int boardsize);

// frees memory needed for the board
void free_array(char** A, const int boardsize);

// prints unsuccessful message
void unsuccessful_response(const char* msg);

// prints successful message
void successful_response(const char* msg);
