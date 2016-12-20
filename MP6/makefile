# makefile
# Richard Carter
# MP5 | 10.30.14
# ECE 2230 | Professor Russell
 
#comp = gcc
#comp_flags = -g -Wall 
#comp_libs = -lm  
#comp = cc

lab6 : graph.o list.o lab6.o
	gcc -Wall -g graph.c lab6.c list.c datatypes.h -o lab6 -lm

graph.o : graph.c graph.h
	gcc -Wall -g -c graph.c datatypes.h

lab6.o : lab6.c graph.h
	gcc -Wall -g -c lab6.c datatypes.h

list.o : list.c list.h datatypes.h
	gcc -Wall -g -c list.c datatypes.h

clean :
	rm -f *.o lab6 core
