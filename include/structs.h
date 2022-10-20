#define BUFFER_SIZE 81
#define INFINITY 50000
#define NEG_INFINITY -50000

typedef struct _tt* transposition_table;

typedef struct player {
    /* i and j will follow the matrix numbering, from 0 to n-1, and will refer to the cell (i+1,j+1)
    eg if black.i is 3 and black.j is 6, it means that the black pawn is on (4,7) or else H4 */
    unsigned char i, j;
    int walls;
} player;

typedef struct stacknode *stackptr;
struct stacknode {
    int i,j;
    // "bm": black player left (i,j), "wm": white player left (i,j), "bw": black player placed wall at (i,j),  "ww": white player placed wall at (i,j)
    char *type; 
    stackptr next;
};

typedef struct returningMove
{
    unsigned char x;
    unsigned char y;
    char move;  // w for wall at (x,y), m for move at (x,y) and -1 for error
    char or;    // orientation of the wall
}
returningMove;

typedef unsigned long long U64;
typedef struct _zobristValues
{
    // we hash based on 3 parameters
    U64** white_pawn, ** black_pawn;  // for pawns
    U64** horizontal_walls, ** vertical_walls;  // for wall placement

    U64* white_walls, *black_walls;  // for walls left
}
*zobristValues;

typedef struct _gameState
{
    player white, black;           // players
    char** wall_matrix;            // board
    uint boardsize;                // the size of the board
    uint totalmoves;               // total moves played
    U64 position_id;               // "almost" unique position identifier/ hash key/ position key
    zobristValues zobrist_values;  // zobrist values
    transposition_table game_tt;   // transposition table
}
*gameState;

typedef struct _move
{
    char col;                // player
    unsigned char i, j;      // move at (i, j)
    unsigned char previ;     // (prev, or_prevj) is the previous place of the pawn movement
    int move_eval;           // evaluation of the move
    char move;               // w for wall at (x,y) OR m for move at (x,y)
    unsigned char or_prevj;  // orientation of the wall, or old j in case of a pawn movement
}
_move;
typedef struct _move* move;

typedef enum
{
    OPENING, MIDDLEGAME, ENDGAME
}
game_phaze;
