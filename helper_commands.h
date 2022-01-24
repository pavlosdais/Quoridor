typedef struct player player;

char isnumber(char *n);
void reset_pawns(int boardsize, player *white, player *black);
void swap_boardsize(char* p, int *boardsize, int *prev_boardsize);
char is_x_available(char hor, player white);
char is_y_availabe(char hor, player black);
player *check_color(char *p, player *black, player *white);
char check_orientation(char *orientation);
char arguments(char *buff);
