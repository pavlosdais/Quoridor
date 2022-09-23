typedef struct _gameState *gameState;

// creates zobrist keys
void createZobristKeys(gameState gs);

// creates board keys
void createBoardKeys(gameState gs);

// creates wall keys
void createWallKeys(gameState gs);

// create starting position id
void generateStartingPositionID(gameState gs);

// generates the position id from scratch
void generatePositionID(gameState gs);

// destroys zobrist keys
void destroyZobristKeys(gameState gs);

// destroys board keys
void destroyBoardKeys(gameState gs);

// destroys wall keys
void destroyWallKeys(gameState gs);
