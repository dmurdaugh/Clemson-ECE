# makefile
# Denzel Murdaugh
# MP7
# ECE 2230
 
comp = gcc
comp_flags = -g -Wall 
comp_libs = -lm  
#comp = cc

lab7 : hash.o lab7.o 
	$(comp) $(comp_flags) hash.o lab7.o -o lab7 $(comp_libs)

hash.o : hash.c hash.h
	$(comp) $(comp_flags) -c hash.c hash.h

lab7.o : lab7.c hash.h
	$(comp) $(comp_flags) -c lab7.c hash.h

clean :
	rm -f *.o lab7 core
