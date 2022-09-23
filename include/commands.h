typedef struct player player;
typedef struct stacknode *stackptr;
typedef struct _gameState *gameState;

///////////////////////
//  Ingame commands  //
///////////////////////

// initializes game state
gameState game_state_init(uint number_of_walls, uint boardsize);

// prints the name of the program
void print_name(char *p);

// prints a message informing if the command read is recognized or not
void known_command(void);

// lists all possible commands
void list_commands();

// updates boardsize
char update_boardsize(gameState game_state, int* prev_boardsize, stackptr* history);

// clears board
void clear_board(gameState game_state);

// updates walls for both players
void update_walls(gameState game_state);

// plays a pawn move for the player read
void playmove(gameState game_state, stackptr* history);

// plays a wall for the player read
void playwall(gameState game_state, stackptr* history);

// ai plays a move for the player read
void genmove(gameState game_state, stackptr* lastaddr);

// undoes moves the times read
void undo(gameState game_state, stackptr* last);

// responds accordingly if there is a winner
void winner(gameState game_state);

// draws board
void showboard(gameState game_state);

// returns the "number" of the command
char command_num(char* ans);

// processes command: gets rid of whitespace characters and converts the command into lower case characters
char command_preprocess(char* buff);

// destroys game state
void destroy_game(gameState game_state);

// allocates memory for the board
char **allocate_memory(const int boardsize);

// frees memory needed for the board
void free_array(char** A, const int boardsize);

// prints unsuccessful message
void unsuccessful_response(const char* msg);

// prints successful message
void successful_response(const char* msg);
