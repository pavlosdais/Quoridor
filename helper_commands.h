typedef struct player player;
typedef struct stacknode *stackptr;

char isnumber(char *n);
void swap_boardsize(char* p, int *boardsize, int *prev_boardsize);
char is_vertex_valid(char hor, int boardsize);
player *check_color(char *p, player *black, player *white);
char check_orientation(char *orientation);
char wallBelow(int i, int j, char **w_mtx, int boardsize);
char wallAbove(int i, int j, char **w_mtx, int boardsize);
char wallOnTheRight(int i, int j, char **w_mtx, int boardsize);
char wallOnTheLeft(int i, int j, char **w_mtx, int boardsize);
char enough_arguments(char *argument);
int path_steps(char **wall_matrix, int boardsize, player *pl, char color);
char there_is_a_wall(int i, int j, char **wall_matrix, int boardsize);
char there_is_a_path(char **wall_matrix, int boardsize, player *white, player *black);
void addMove(stackptr *last, int i, int j, char type);
