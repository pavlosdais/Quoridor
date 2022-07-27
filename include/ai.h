typedef struct player player;
typedef struct returningMove returningMove;

// returns the best move for player pl at specified depth
returningMove bestMove(char** wall_matrix, const int boardsize, const char pl, player* black, player* white, unsigned char depth, char pseudo, const float max_time);
