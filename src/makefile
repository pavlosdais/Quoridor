# make the executable file
ipquoridor: ipquoridor.o commands.o utilities.o bfs.o ai.o
	gcc -o ipquoridor ipquoridor.o commands.o utilities.o bfs.o ai.o

# make the object files needed
ipquoridor.o: ipquoridor.c
	gcc -c ipquoridor.c

commands.o: commands.c 
	gcc -c commands.c 

utilities.o: utilities.c
	gcc -c utilities.c

bfs.o: bfs.c
	gcc -c bfs.c

ai.o: ai.c
	gcc -c ai.c

# delete junk object  files
clear:
	rm -f *.o

# run the program
play:
	./ipquoridor
