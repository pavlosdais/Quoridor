# source directory
SRC_DIR = ./src

# engine directory
ENG_DIR = ./src/engine

# modules directory
MOD_DIR = ./modules

# object files needed
OBJ = $(SRC_DIR)/ipquoridor.o \
	  $(SRC_DIR)/commands.o \
	  $(SRC_DIR)/utilities.o \
	  $(SRC_DIR)/bfs.o \
	  $(SRC_DIR)/dfs.o \
	  $(SRC_DIR)/zobrist_hashing.o \
	  $(MOD_DIR)/transposition_table.o \
	  $(MOD_DIR)/pq.o \
	  $(ENG_DIR)/generate_moves.o \
	  $(ENG_DIR)/engine.o \
	  $(ENG_DIR)/engine_utilities.o 

EXEC = ipquoridor
CC = gcc
flags = -ggdb3

# make the executable file
$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(flags)


# make the object files needed

# GAME
$(EXEC).o: $(SRC_DIR)/$(EXEC).c
	$(CC) -c $(SRC_DIR)/$(EXEC).c $(flags)

commands.o: $(SRC_DIR)/commands.c 
	$(CC) -c $(SRC_DIR)/commands.c $(flags)

utilities.o: $(SRC_DIR)/utilities.c
	$(CC) -c $(SRC_DIR)/utilities.c $(flags)

# PATHFINDING
bfs.o: $(SRC_DIR)/bfs.c
	$(CC) -c $(SRC_DIR)/bfs.c $(flags)

dfs.o: $(SRC_DIR)/dfs.c
	$(CC) -c $(SRC_DIR)/dfs.c $(flags)

# MODULES
transposition_table.o: $(MOD_DIR)/transposition_table.c
	$(CC) -c $(MOD_DIR)/transposition_table.c $(flags)

pq.o: $(MOD_DIR)/pq.c
	$(CC) -c $(MOD_DIR)/pq.c $(flags)

# ENGINE 
engine.o: $(ENG_DIR)/engine.c
	$(CC) -c $(ENG_DIR)/engine.c $(flags)

generate_moves.o: $(ENG_DIR)/generate_moves.c
	$(CC) -c $(ENG_DIR)/generate_moves.c $(flags)

engine_utilities.o: $(ENG_DIR)/engine_utilities.c
	$(CC) -c $(ENG_DIR)/engine_utilities.c $(flags)

zobrist_hashing.o: $(SRC_DIR)/zobrist_hashing.c
	$(CC) -c $(SRC_DIR)/zobrist_hashing.c $(flags)

# delete excess object files
clear:
	rm -f $(OBJ)

# play the game
play: $(EXEC)
	./$(EXEC)

# run valgrind
help: $(EXEC)
	valgrind --leak-check=full -v --show-leak-kinds=all --track-origins=yes ./$(EXEC)


### USE REFEREE ##

# other programs directory (competition & more)
PROGS_DIR = otherProgs

# give some files executable permission
file = $(PROGS_DIR)/1.2
exe:
	chmod 744 quoridor_referee.py
	chmod 744 $(PROGS_DIR)/1.1
	chmod 744 $(PROGS_DIR)/1.2
	chmod 744 $(PROGS_DIR)/2
	chmod 744 $(PROGS_DIR)/other1

################################
### REFEREE SETTINGS SECTION ###
################################
WHITE = $(EXEC)  # player with white
BLACK = $(PROGS_DIR)/other1  # player with black
SIZE = 7  # board size
GAMES = 1  # number of games to play
VERBOSE = 2  # 2 to draw board, 1 to hide
MEMORY_LIMIT = 950  # maximum megabytes of memory that can be used for players at each point of the game

# run referee
ref: $(EXEC)
	./quoridor_referee.py --black ./$(BLACK) --white ./$(WHITE) --memory_limit $(MEMORY_LIMIT) --size $(SIZE) --verbose $(VERBOSE) --games $(GAMES)
