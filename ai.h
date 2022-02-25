typedef struct player player;
typedef struct returningMove returningMove;
returningMove *bestMove(char** w_mtx, int boardsize, char pl, player* black, player* white, char depth);
float minimax(char** wall_matrix, int boardsize, char depth, float alpha, float beta, player* maximizing, player* minimizing, char pl, char maximizingPlays, player *black, player *white);