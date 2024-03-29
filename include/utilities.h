typedef struct player player;
typedef struct stacknode *stackptr;
typedef struct _gameState *gameState;

///////////////////////
// Utility functions //
///////////////////////

// returns 1 if n is a number, 0 in any other case
char isNumber(char* n);

// returns 1 if there are arguments left, 0 if not
char enough_arguments(char* argument);

// returns the player with the corresponding color
player *check_color(char* p, player* black, player* white);

// returns 'w' for white, 'b' for black or -1 in any other case
char colorValue(char* p);

// returns 'b' for horizontal, 'r' for vertical or -1 in any other case
char check_orientation(char* orientation);

// returns 1 if the vertex is valid, 0 if not
char is_vertex_valid(char hor, const int boardsize);

// returns 1 if the wall is valid, 0 if not
char isValidWall(const int vertex_x, const int vertex_y, gameState gs, char orientation);

// returns 1 if there is a path for white and black to reach their respective goal rows, 0 if not
char there_is_a_path(gameState gs);

// returns true if there is a wall horizontally of (vertex_x, vertex_y)
char thereIsAWallHorizontally(gameState gs, const int vertex_x, const int vertex_y);

// returns true if there is a wall vertically of (vertex_x, vertex_y)
char thereIsAWallVertically(gameState gs, const int vertex_x, const int vertex_y);

// returns 1 if there's a wall below (i, j), 0 if not
char wallBelow(const int i, const int j, char** wall_matrix, const int boardsize);

// returns 1 if there's a wall above (i, j), 0 if not
char wallAbove(const int i, const int j, char** wall_matrix, const int boardsize);

// returns 1 if there's a wall on the right of (i, j), 0 if not
char wallOnTheRight(const int i, const int j, char** wall_matrix, const int boardsize);

// returns 1 if there's a wall on the left of (i, j), 0 if not
char wallOnTheLeft(const int i, const int j, char** wall_matrix, const int boardsize);

// adds move to the stack (history)
void addMove(stackptr* last, const int i, const int j, char* type);

// plays and prints evaluated move
void playGenMove(gameState gs, char pl, returningMove evalMove, stackptr* lastaddr);
