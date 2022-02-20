int bfs(int boardsize, char**m, int startx, int starty, int goalx);
char wallBelow(int i, int j, char **w_mtx, int boardsize);
char wallAbove(int i, int j, char **w_mtx, int boardsize);
char wallOnTheRight(int i, int j, char **w_mtx, int boardsize);
char wallOnTheLeft(int i, int j, char **w_mtx, int boardsize);
char there_is_a_wall(int i, int j, char **wall_matrix, int boardsize);