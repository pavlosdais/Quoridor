typedef struct player player;
float minimax(char**wall_matrix, int boardsize, char depth, float alpha, float beta, char maximizingPlayer, player* black, player* white, char pl, char *move, int *x, int *y, char* or);
