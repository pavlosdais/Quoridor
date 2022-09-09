// Helper function prototypes
int minimax(char** wall_matrix, cint boardsize, csint depth, int alpha, int beta, player* white, player* black, char Maximizing, cchar pseudo, cint start_range, cint end_range);

static char drwhite[] = {0, 0, 1, 1, 1, -1, -1, -1}, dcwhite[] = {-1, 1, -1, 0, 1, -1, 0, 1};
static char drblack[] = {-1, -1, -1, 0, 0, 1, 1, 1}, dcblack[] = {-1, 0, 1, -1, 1, -1, 0, 1};

typedef char (*check_path)(char** wall_matrix, cint boardsize, player* white, player* black);

#define MAX(a, b) (a > b ? a:b)
#define MIN(a, b) (a < b ? a:b)

#define SEARCH_START_RANGE(range) (range * 1.5)
#define SEARCH_END_RANGE(range) (range / 1.2)
#define MINIMAX_START_RANGE(range) (range * 1.5)
#define MINIMAX_END_RANGE(range) (range / 1.2)

void placeWall(player* pl, char** wall_matrix, char orientation, cint i, cint j);
void resetWallPlacement(player* pl, char** wall_matrix, cint i, cint j);
void uwp_white(returningMove* evalMove, cchar orientation, cint i, cint j, int* eval, int* max_eval);
void uwp_black(returningMove* evalMove, cchar orientation, cint i, cint j, int* eval, int* min_eval);
void updatePawnMovementWhite(player* white, returningMove* evalMove, int* eval, int* max_eval);
void updatePawnMovementBlack(player* black, returningMove* evalMove, int* eval, int* min_eval);

int positionEvaluation(player black, player white, cint boardsize, char** wall_matrix);

char there_is_a_path_black(char** wall_matrix, cint boardsize, player* white, player* black);


void check_walls(char** wall_matrix, player* white, player* black, player* curr_player, cint boardsize, csint depth, cchar pseudo, int start, cint end, cfloat max_time,
int* eval, int* best_eval, const clock_t t, returningMove* evalMove, QuitFunc quit, ChangeFunc change, cchar cond, updateWallWallPlacement uwp, char col, cfloat scope);

void check_walls_white(char** wall_matrix, player* white, player* black, cint boardsize, csint depth, cchar pseudo, int start_range, int end_range, cfloat max_time,
int* eval, int* best_eval, const clock_t t, returningMove* evalMove, cfloat scope);

void check_walls_black(char** wall_matrix, player* white, player* black, cint boardsize, csint depth, cchar pseudo, int start_range, int end_range, cfloat max_time,
int* eval, int* best_eval, const clock_t t, returningMove* evalMove, cfloat scope);

#define MOVE_UP(a) (++a->i)
#define MOVE_DOWN(a) (--a->i)
#define MOVE_RIGHT(a) (++a->j)
#define MOVE_LEFT(a) (--a->j)

#define MOVE_UP2(a) ((a->i)+=2)
#define MOVE_DOWN2(a) ((a->i)-=2)
#define MOVE_RIGHT2(a) ((a->j)+=2)
#define MOVE_LEFT2(a) ((a->j)-=2)

#define PRIMARY 5
