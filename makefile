SRC_DIR = ./src

OBJ = $(SRC_DIR)/ipquoridor.o \
	  $(SRC_DIR)/commands.o \
	  $(SRC_DIR)/utilities.o \
	  $(SRC_DIR)/bfs.o \
	  $(SRC_DIR)/ai.o

EXEC = ipquoridor
CC = gcc

# make the executable file
ipquoridor: $(OBJ)
	$(CC) -o $(EXEC) $(OBJ)

# make the object files needed
ipquoridor.o: $(SRC_DIR)/ipquoridor.c
	$(CC) -c $(SRC_DIR)/ipquoridor.c

commands.o: $(SRC_DIR)/commands.c 
	$(CC) -c $(SRC_DIR)/commands.c 

utilities.o: $(SRC_DIR)/utilities.c
	$(CC) -c $(SRC_DIR)/utilities.c

bfs.o: $(SRC_DIR)/bfs.c
	$(CC) -c $(SRC_DIR)/bfs.c

ai.o: $(SRC_DIR)/ai.c
	$(CC) -c $(SRC_DIR)/ai.c

# delete excess object files
clear:
	rm -f $(OBJ)

# play the game
play:
	./ipquoridor
