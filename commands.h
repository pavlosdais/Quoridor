typedef struct player player;

char isnumber(char *n);
void reset_pawns(int boardsize, player *white, player *black);
void swap_boardsize(char* p, int *boardsize, int *prev_boardsize);
char is_vertex_available(char hor, int boardsize);
player *check_color(char *p, player *black, player *white);
char check_orientation(char *orientation);
char there_is_a_wall(int i, int j, char **wall_matrix, int boardsize);
char enough_arguments(char *argument);
char there_is_a_path(char **wall_matrix, int boardsize, player *white, player *black);
