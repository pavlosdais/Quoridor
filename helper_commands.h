typedef struct position position;

char isnumber(char *n);
void reset_pawns(int boardsize, position *white, position *black);
void swap_boardsize(char* p, int *boardsize, int *prev_boardsize);
char is_x_available(char hor, position white);
char is_y_availabe(char hor, position black);
position *check_color(char *p, position *black, position *white);
char check_orientation(char *orientation);
char arguments(char *buff);
