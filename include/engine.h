typedef struct player player;
typedef struct returningMove returningMove;

// returns what is believed to be the best move, by the engine, for player pl at specified depth
returningMove bestMove(char** wall_matrix, const int boardsize, const char pl, player* black, player* white, const unsigned char depth, 
    char pseudo, const float max_time, const float scope);
