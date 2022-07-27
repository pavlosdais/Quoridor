#include <stdbool.h>

// returns true if there is a path from cell (startx, starty) to the row goalx
bool dfs(const int boardsize, char** m, const int startx, const int starty, const int goalx, const char player);
