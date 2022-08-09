typedef unsigned char small_int;
typedef struct player player;
typedef struct returningMove returningMove;
typedef int (*QuitFunc)(int a, int b);
typedef int (*ChangeFunc)(int* a);
typedef void (*updateWallWallPlacement)(returningMove* evalMove, char orientation, int i, int j, int* eval, int* max_eval);
