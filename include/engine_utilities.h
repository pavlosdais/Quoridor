typedef char (*check_path)(char** wall_matrix, cint boardsize, player* white, player* black);

#define MOVE_UP(a) (++a->i)
#define MOVE_DOWN(a) (--a->i)
#define MOVE_RIGHT(a) (++a->j)
#define MOVE_LEFT(a) (--a->j)

#define MOVE_UP2(a) ((a->i)+=2)
#define MOVE_DOWN2(a) ((a->i)-=2)
#define MOVE_RIGHT2(a) ((a->j)+=2)
#define MOVE_LEFT2(a) ((a->j)-=2)

#define MAX(a, b) (a > b ? a:b)
#define MIN(a, b) (a < b ? a:b)

int minimax(char** wall_matrix, cint boardsize, const small_int depth, int alpha, int beta, player* white, player* black, char Maximizing, cchar pseudo, cint range);

// places wall at (i,j)
void placeWall(player* pl, char** wall_matrix, char orientation, cint i, cint j);

// resets wall placement at (i,j)
void resetWallPlacement(player* pl, char** wall_matrix, cint i, cint j);

// returns 1 if there is a path for white and black to reach their respective goal rows, 0 if not
// checks first if there is a path for white, then for black
char there_is_a_path_black(char** wall_matrix, cint boardsize, player* white, player* black);

// check wall placement
void check_walls(char** wall_matrix, player* white, player* black, player* curr_player, cint boardsize, csint depth, cchar pseudo, int start, cint end, cfloat max_time,
int* eval, int* best_eval, const clock_t t, returningMove* evalMove, QuitFunc quit, ChangeFunc change, cchar cond, updateWallWallPlacement uwp, char col);

// returns the evaluation of the current position (negative if black has an advantage, positive if white has an advantage)
int positionEvaluation(player black, player white, const int boardsize, char** wall_matrix);

// functions needed for the engine
void uwp_white(returningMove* evalMove, cchar orientation, cint i, cint j, int* eval, int* max_eval);
void uwp_black(returningMove* evalMove, cchar orientation, cint i, cint j, int* eval, int* min_eval);
void updatePawnMovementWhite(player* white, returningMove* evalMove, int* eval, int* max_eval);
void updatePawnMovementBlack(player* black, returningMove* evalMove, int* eval, int* min_eval);
int compare_white(int a, int b);
int change_white(int* a);
int compare_black(int a, int b);
int change_black(int* a);
