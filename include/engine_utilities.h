typedef char (*check_path)(char** wall_matrix, cint boardsize, player* white, player* black);

#define PAWN_MOVEMENT_EVAL 10000

#define RAN_OUT_OF_TIME() ((((double)clock() - start_time)/CLOCKS_PER_SEC) > timeout_time)

#define MOVE_UP(a) (++(a->i))
#define MOVE_DOWN(a) (--(a->i))
#define MOVE_RIGHT(a) (++(a->j))
#define MOVE_LEFT(a) (--(a->j))
#define MOVE_UP2(a) ((a->i)+=2)
#define MOVE_DOWN2(a) ((a->i)-=2)
#define MOVE_RIGHT2(a) ((a->j)+=2)
#define MOVE_LEFT2(a) ((a->j)-=2)

#define MAX(a, b) (a > b ? a:b)
#define MIN(a, b) (a < b ? a:b)

// engine helper prototypes
returningMove bestMove(gameState gs, cchar pl, small_int depth);
int minimax(gameState gs, small_int depth, int alpha, int beta, char maximizing);

// returns 1 if there is a path for white and black to reach their respective goal rows, 0 if not
// checks first if there is a path for white, then for black
bool there_is_a_path_black(gameState gs);

// returns 1 if there is a path for white and black to reach their respective goal rows, 0 if not
// checks first if there is a path for black, then for white
bool there_is_a_path_white(gameState gs);

// returns the evaluation of the current position (negative if black has an advantage, positive if white has an advantage)
int positionEvaluation(gameState gs);

// functions needed for the engine
void updatePawnMovement(returningMove* evalMove, move a, int* eval, int* best_eval);
void updateWallPlacement(returningMove* evalMove, move a, int* eval, int* best_eval);
