# Compiler settings
CC = gcc
CFLAGS = -g
LDFLAGS =

# Directories
SRC_DIR = src
MODULES_DIR = modules
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
MODULES_FILES = $(wildcard $(MODULES_DIR)/*.c)

# Object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES)) \
            $(patsubst $(MODULES_DIR)/%.c, $(OBJ_DIR)/%.o, $(MODULES_FILES))

# Executable program
EXEC_NAME = ipquoridor
EXEC = $(BIN_DIR)/$(EXEC_NAME)

# Program command line arguments
CLA = 

# OS
OS := LIN
ifeq ($(OS), WIN)
	CC = x86_64-w64-mingw32-gcc
	EXEC = $(BIN_DIR)/$(EXEC_NAME).exe
endif

# Targets
.PHONY:
	all clear help play final exe ref

all: $(EXEC)

# Create the executable program
$(EXEC): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ -o $@

# Compile
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(MODULES_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Play the game
play:
	./$(EXEC) $(CLA)

# Clear files used by the program
clear:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Use valgrind - Output is saved at bin/valgrind.log
help: $(EXEC)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=./bin/valgrind.log ./$(EXEC) $(CLA)

# Final build // use optimization flags
final: CFLAGS = -Ofast
final: LDFLAGS = 
final: all

### REFEREE SETTINGS ##

# other programs directory (competition & more)
PROGS_DIR = otherProgs

# give some files executable permission
exe:
	chmod 744 quoridor_referee.py
	chmod 744 $(PROGS_DIR)/1.1
	chmod 744 $(PROGS_DIR)/1.2
	chmod 744 $(PROGS_DIR)/2
	chmod 744 $(PROGS_DIR)/other1

################################
### REFEREE SETTINGS SECTION ###
################################
WHITE = $(PROGS_DIR)/other1  # player with white
BLACK = $(EXEC)  # player with black
SIZE = 9  # board size
GAMES = 1  # number of games to play
VERBOSE = 2  # 2 to draw board, 1 to hide
MEMORY_LIMIT = 950  # maximum megabytes of memory that can be used for players at each point of the game

# run referee
ref: $(EXEC)
	./quoridor_referee.py --black ./$(BLACK) --white ./$(WHITE) --memory_limit $(MEMORY_LIMIT) --size $(SIZE) --verbose $(VERBOSE) --games $(GAMES)
