typedef struct _gameState *gameState;
typedef struct _move* move;
typedef struct _pq* PQ;

// generates all legal moves for the player specified (1 for white, -1 for black)
PQ generateMoves(gameState gs, char player);

// plays move for the player specified (1 for white, -1 for black)
void playMove(gameState gs, move m, char player);

// undoes move for the player specified (1 for white, -1 for black)
void undoMove(gameState gs, move m, char player);


// helper functions
PQ generate_moves_black(gameState gs);
PQ generate_moves_white(gameState gs);

void playMoveWhite(gameState gs, move m);
void undoMoveWhite(gameState gs, move m);

void playMoveBlack(gameState gs, move m);
void undoMoveBlack(gameState gs, move m);
