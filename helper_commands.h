typedef struct player player;

char isnumber(char *n);
void reset_pawns(int boardsize, player *white, player *black);
void swap_boardsize(char* p, int *boardsize, int *prev_boardsize);
char is_x_available(char hor, int boardsize);
char is_y_availabe(char hor, int boardsize);
player *check_color(char *p, player *black, player *white);
char check_orientation(char *orientation);
char wallBelow(int i, int j, char **w_mtx, int boardsize);
char wallAbove(int i, int j, char **w_mtx, int boardsize);
char wallOnTheRight(int i, int j, char **w_mtx, int boardsize);
char wallOnTheLeft(int i, int j, char **w_mtx, int boardsize);
char enough_arguments(char *argument);
int path_steps(char **wall_matrix, int boardsize, player *pl, char color);
