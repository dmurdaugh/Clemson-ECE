# makefile for MP4
# Version: 1
#
# requires updates.  For example, you must add queue.c
#
comp_flags = -g -Wall
link_flags = -lm

lab4 : list.o utility.o lab4.o queue.o priorityq.o
	gcc $(comp_flags) list.o utility.o lab4.o -o lab4 $(link_flags)

list.o : list.c datatypes.h list.h
	gcc $(comp_flags) -c list.c

utility.o : utility.c datatypes.h utility.h
	gcc $(comp_flags) -c utility.c

lab4.o : lab4.c datatypes.h list.h utility.h
	gcc $(comp_flags) -c lab4.c

queue.o : queue.c queue.h datatypes.h
	gcc $(comp_flags) -c queue.c

priorityq.o : priorityq.c priorityq.h datatypes.h
	gcc $(comp_flags) -c priorityq.c

clean :
	rm -f *.o lab4 core

