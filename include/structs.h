#include <limits.h>
#define BUFFER_SIZE 81
#define SCOPE 1.5
#define INFINITY INT_MAX
#define NEG_INFINITY INT_MIN

typedef struct player {
    /* i and j will follow the matrix numbering, from 0 to n-1, and will refer to the cell (i+1,j+1)
    eg if black.i is 3 and black.j is 6, it means that the black pawn is on (4,7) or else H4 */
    int i;
    int j;
    int walls;
} player;

typedef struct stacknode *stackptr;
struct stacknode {
    int i,j;
    char *type; // "bm": black player left (i,j), "wm": white player left (i,j), "bw": black player placed wall at (i,j),  "ww": white player placed wall at (i,j)
    stackptr next;
};

typedef struct returningMove
{
    int x;
    int y;
    char move;  // w for wall at (x,y), m for move at (x,y) and -1 for error
    char or;  // orientation of the wall
} returningMove;
