# make the executable file
ipquoridor: ipquoridor.o commands.o
	gcc -o ipquoridor ipquoridor.o commands.o

# make the object files needed
ipquoridor.o: ipquoridor.c
	gcc -c ipquoridor.c

commands.o: commands.c
	gcc -c commands.c
	
# delete junk object  files
clear:
	rm -f *.o

# run the program
run:
	./ipquoridor