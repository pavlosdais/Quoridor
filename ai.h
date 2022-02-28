typedef struct player player;
typedef struct returningMove returningMove;
returningMove bestMove(char** wall_matrix, int boardsize, char pl, player* black, player* white, unsigned char depth);
