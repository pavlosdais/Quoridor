typedef struct player player;
typedef struct returningMove returningMove;

// returns what is believed to be the best move, by the engine
returningMove iterativeDeepening(gameState gs, cchar pl);
