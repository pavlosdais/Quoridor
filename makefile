# make the executable file
ipquoridor: ipquoridor.o commands.o helper_commands.o
	gcc -o ipquoridor ipquoridor.o commands.o helper_commands.o

# make the object files needed
ipquoridor.o: ipquoridor.c
	gcc -c ipquoridor.c

commands.o: commands.c 
	gcc -c commands.c 

helper_commands.o: helper_commands.c
	gcc -c helper_commands.c
	
# delete junk object  files
clear:
	rm -f *.o

# run the program
run:
	./ipquoridor