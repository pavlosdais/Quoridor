# source directory
SRC_DIR = ./src

# object files
OBJ = $(SRC_DIR)/ipquoridor.o \
	  $(SRC_DIR)/commands.o \
	  $(SRC_DIR)/utilities.o \
	  $(SRC_DIR)/bfs.o \
	  $(SRC_DIR)/engine.o \
	  $(SRC_DIR)/dfs.o

EXEC = ipquoridor
CC = gcc
flags = -ggdb3

# make the executable file
ipquoridor: $(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(flags)

# make the object files needed
ipquoridor.o: $(SRC_DIR)/ipquoridor.c
	$(CC) -c $(SRC_DIR)/ipquoridor.c $(flags)

commands.o: $(SRC_DIR)/commands.c 
	$(CC) -c $(SRC_DIR)/commands.c $(flags)

utilities.o: $(SRC_DIR)/utilities.c
	$(CC) -c $(SRC_DIR)/utilities.c $(flags)

bfs.o: $(SRC_DIR)/bfs.c
	$(CC) -c $(SRC_DIR)/bfs.c $(flags)

dfs.o: $(SRC_DIR)/dfs.c
	$(CC) -c $(SRC_DIR)/dfs.c $(flags)

engine.o: $(SRC_DIR)/engine.c
	$(CC) -c $(SRC_DIR)/engine.c $(flags)

# delete excess object files
clear:
	rm -f $(OBJ)

# play the game
play: $(EXEC)
	./$(EXEC)

# run valgrind
help: $(EXEC)
	valgrind --leak-check=full -v --show-leak-kinds=all --track-origins=yes ./ipquoridor


### USE REFEREE ##

# other programs directory (competition & more)
PROGS_DIR = otherProgs

# give a file executable permission
file = $(PROGS_DIR)/other1
exe:
	chmod 744 $(file)

################################
### REFEREE SETTINGS SECTION ###
################################
WHITE = $(PROGS_DIR)/other1  # player with white
BLACK = $(EXEC)  # player with black
SIZE = 5  # board size
GAMES = 1  # number of games
VERBOSE = 2  # 2 to draw board, 1 to hide

# run referee
ref: $(EXEC)
	./quoridor_referee.py --black ./$(BLACK) --white ./$(WHITE) --size $(SIZE) --verbose $(VERBOSE) --games $(GAMES)
