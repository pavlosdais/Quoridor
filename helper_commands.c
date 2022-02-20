typedef struct player player;
typedef struct stacknode *stackptr;

char isNumber(char *n);
void swap_boardsize(char* p, int *boardsize, int *prev_boardsize);
char is_vertex_valid(char hor, int boardsize);
player *check_color(char *p, player *black, player *white);
char check_orientation(char *orientation);
char enough_arguments(char *argument);
int path_steps(char **wall_matrix, int boardsize, player *pl, char color);
char there_is_a_wall(int i, int j, char **wall_matrix, int boardsize);
char there_is_a_path(char **wall_matrix, int boardsize, player *white, player *black);
void addMove(stackptr *last, int i, int j, char *type);
float positionEvaluation(player* black, player* white, int boardsize, char** wall_matrix);
