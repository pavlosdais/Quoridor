typedef struct player player;
typedef struct stacknode *stackptr;

void unsuccessful_response(char *msg);
void successful_response(char *msg);
char **allocate_memory(int boardsize);
void free_array(char **A, int boardsize);
char command_num(char *ans);
void print_name(char *p);
void known_command(void);
void list_commands();
void update_boardsize(int *boardsize, int *prev_boardsize, char ***wall_matrix, player *white, player *black);
void clear_board(int boardsize, char **wall_matrix, player *white, player *black);
void update_walls(player *black, player *white, int* number_of_walls);
void playmove(char *buff, player *white, player *black, char** wall_matrix, int boardsize);
void playwall(char *buff, player *white, player *black, char** wall_matrix, int boardsize);
void winner(player *white, player *black, int boardsize);
void showboard(char **w_mtx, int boardsize, player *black, player *white);
void command_preprocess(char *buff);
void reset_pawns(int boardsize, player *white, player *black);
void undo(char **wall_matrix, player *black, player *white, stackptr *last, int *totalmoves);
