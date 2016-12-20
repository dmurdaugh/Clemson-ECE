# The makefile for MP2.
# Type:
#   make         -- to build program lab2
#   make testlist -- to compile testing program
#   make clean   -- to delete object files, executable, and core
#   make design  -- check for simple design errors (incomplete)
#   make list.o  -- to compile only list.o (or: use lab2.o, trte_support.o)
#
# You should not need to change this file.  
#
# Format for each entry
#    target : dependency list of targets or files
#    <tab> command 1
#    <tab> command 2
#    ...
#    <tab> last command
#    <blank line>   -- the list of commands must end with a blank line

lab2 : list.o trte_support.o lab2.o
	gcc -Wall -g list.o trte_support.o lab2.o -o lab2

list.o : list.c datatypes.h list.h
	gcc -Wall -g -c list.c

trte_support.o : trte_support.c trte_support.h datatypes.h list.h
	gcc -Wall -g -c trte_support.c

lab2.o : lab2.c datatypes.h list.h trte_support.h
	gcc -Wall -g -c lab2.c

testlist : testlist.o list.o trte_support.o
	gcc -Wall -g list.o trte_support.o testlist.o -o testlist

testlist.o : testlist.c datatypes.h list.h trte_support.h
	gcc -Wall -g -c testlist.c

design :
	@grep -e "-> *head" lab2.c trte_support.c ||:
	@grep -e "-> *tail" lab2.c trte_support.c ||:
	@grep -e "-> *current_list_size" lab2.c trte_support.c ||:
	@grep -e "-> *list_sorted_state" lab2.c trte_support.c ||:
	@grep -e "-> *next" lab2.c trte_support.c ||:
	@grep -e "-> *prev" lab2.c trte_support.c ||:
	@grep -e "-> *data_ptr" lab2.c trte_support.c ||:
	@grep "comp_proc" lab2.c trte_support.c ||:
	@grep "data_clean" lab2.c trte_support.c ||:
	@grep "list_node_t" lab2.c trte_support.c ||:
	@grep "src_ip_addr" list.c ||:
	@grep "dest_ip_addr" list.c ||:
	@grep "routelist" list.c ||:
	@grep "hop_count" list.c ||:
	@grep "route_t" list.c ||:
	@grep "route_info_tag" list.c ||:
	@grep "trte_" list.c ||:

clean :
	rm -f *.o lab2 testlist core a.out 

