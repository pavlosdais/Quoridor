typedef struct _gameState *gameState;
typedef struct _move* move;
typedef struct _pq* PQ;

// generates all legal moves for the player specified (1 for white, -1 for black)
PQ generateMoves(gameState gs, char player);

// helper functions
PQ generate_moves_black(gameState gs);
PQ generate_moves_white(gameState gs);

#define find_end_time(gs, timeout_time) if (gs->boardsize < 6)       \
                                            timeout_time =  14;      \
                                        else if (gs->boardsize < 8)  \
                                            timeout_time = 12;       \
                                        else if (gs->boardsize < 10) \
                                            timeout_time = 6;        \
                                        else if (gs->boardsize < 14) \
                                            timeout_time = 5;        \
                                        else                         \
                                            timeout_time = 4;

#define playMoveWhite(gs, m) if (m->move == 'm')                                                             \
                             {                                                                               \
                                gs->position_id ^= gs->zobrist_values->white_pawn[gs->white.i][gs->white.j]; \
                                gs->white.i = m->i; gs->white.j = m->j;                                      \
                                gs->position_id ^= gs->zobrist_values->white_pawn[gs->white.i][gs->white.j]; \
                             }                                                                               \
                             else                                                                            \
                             {                                                                               \
                                if (m->or_prevj == 'b')                                                      \
                                    gs->position_id ^= gs->zobrist_values->horizontal_walls[m->i][m->j];     \
                                else                                                                         \
                                    gs->position_id ^= gs->zobrist_values->vertical_walls[m->i][m->j];       \
                                gs->wall_matrix[m->i][m->j] = m->or_prevj;                                   \
                                gs->position_id ^= gs->zobrist_values->white_walls[gs->white.walls];         \
                                gs->position_id ^= gs->zobrist_values->white_walls[--(gs->white.walls)];     \
                             }

#define playMoveBlack(gs, m) if (m->move == 'm')                                                             \
                             {                                                                               \
                                gs->position_id ^= gs->zobrist_values->black_pawn[gs->black.i][gs->black.j]; \
                                gs->black.i = m->i; gs->black.j = m->j;                                      \
                                gs->position_id ^= gs->zobrist_values->black_pawn[gs->black.i][gs->black.j]; \
                             }                                                                               \
                             else                                                                            \
                             {                                                                               \
                                if (m->or_prevj == 'b')                                                      \
                                    gs->position_id ^= gs->zobrist_values->horizontal_walls[m->i][m->j];     \
                                else                                                                         \
                                    gs->position_id ^= gs->zobrist_values->vertical_walls[m->i][m->j];       \
                                gs->wall_matrix[m->i][m->j] = m->or_prevj;                                   \
                                gs->position_id ^= gs->zobrist_values->black_walls[gs->black.walls];         \
                                gs->position_id ^= gs->zobrist_values->black_walls[--(gs->black.walls)];     \
                             }

#define undoMoveBlack(gs, m)            if (m->move == 'm') \
                                        { \
                                            gs->position_id ^= gs->zobrist_values->black_pawn[gs->black.i][gs->black.j]; \
                                            gs->black.i = m->previ; gs->black.j = m->or_prevj; \
                                            gs->position_id ^= gs->zobrist_values->black_pawn[gs->black.i][gs->black.j]; \
                                        } \
                                        else \
                                        { \
                                            if (m->or_prevj == 'b') \
                                                gs->position_id ^= gs->zobrist_values->horizontal_walls[m->i][m->j]; \
                                            else \
                                                gs->position_id ^= gs->zobrist_values->vertical_walls[m->i][m->j]; \
                                            gs->wall_matrix[m->i][m->j] = 0; \
                                            gs->position_id ^= gs->zobrist_values->black_walls[gs->black.walls]; \
                                            gs->position_id ^= gs->zobrist_values->black_walls[++(gs->black.walls)]; \
                                        }

#define undoMoveWhite(gs, m)            if (m->move == 'm') \
                                        { \
                                            gs->position_id ^= gs->zobrist_values->white_pawn[gs->white.i][gs->white.j]; \
                                            gs->white.i = m->previ; gs->white.j = m->or_prevj; \
                                            gs->position_id ^= gs->zobrist_values->white_pawn[gs->white.i][gs->white.j]; \
                                        } \
                                        else \
                                        { \
                                            if (m->or_prevj == 'b') \
                                                gs->position_id ^= gs->zobrist_values->horizontal_walls[m->i][m->j]; \
                                            else \
                                                gs->position_id ^= gs->zobrist_values->vertical_walls[m->i][m->j]; \
                                            gs->wall_matrix[m->i][m->j] = 0; \
                                            gs->position_id ^= gs->zobrist_values->white_walls[gs->white.walls]; \
                                            gs->position_id ^= gs->zobrist_values->white_walls[++(gs->white.walls)]; \
                                        }

#define playMove(gs, m, player)         if (player == 1) \
                                        { \
                                            playMoveWhite(gs, m) \
                                        } \
                                        else \
                                        {
                                            playMoveBlack(gs, m) \
                                        }

#define undoMove(gs, m, player)         if (player == 1) \
                                        { \
                                            undoMoveWhite(gs, m) \
                                        } \
                                        else \
                                        { \
                                            undoMoveBlack(gs, m) \
                                        }
