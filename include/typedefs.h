typedef struct player player;
typedef struct returningMove returningMove;
typedef int (*QuitFunc)(int a, int b);
typedef int (*ChangeFunc)(int* a);
typedef void (*updateWallWallPlacement)(returningMove* evalMove, char orientation, int i, int j, int* eval, int* max_eval);

#define VERTICAL_WALL 'r'
#define HORIZONTAL_WALL 'b'
#define ABS(a, b) (a > b? a-b : b-a)

typedef unsigned char small_int;
typedef unsigned long long U64;
typedef const int cint;
typedef const char cchar;
typedef const float cfloat;
typedef const unsigned char csint;
