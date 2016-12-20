/* 

   Writing and debugging MP2 is challenging, and it is particularly hard if you
   try and write all the code before you do any testing. It is much easier to
   break the problem into small pieces and test each piece. 

For MP2, one approach is to write list_insert first, since it might be the
easiest algorithm and it depends only on list_construct.

   For testing, we use a simple main function in this file.  This file is for
   testing only, and you will NOT submit it as it is for your personal testing
   during development.

Below are a few simple examples that allocates a few structures and inserts
them into a list.

With this file containing a temporary main section, you can compile and run
list.c on its own using 
   gcc -Wall -g testlist.c list.c trte_support.c -o testlist

The compling command is also found in the provided makefile.  Do
   make testlist

Do this testing before you make ANY changes to trte_support.c.  The only 
reason we need to compile trte_support.c is for the trte_compare function.
The linker needs to find the comp_proc function and it is found in the template
   trte_support.c file (with no changes) 

*/

#include <stdlib.h>
#include <stdio.h>
#include "datatypes.h"
#include "list.h"
#include "trte_support.h"

/* prototype for list debug print */
void list_debug_print(list_t *list_ptr);

int main(void)
{
    data_t *datap = NULL;
    list_t *Lptr = NULL;

    Lptr = list_construct(trte_compare, trte_route_rec_cleanup);

    // create one item to test list_insert
    datap = (data_t *) calloc(1,sizeof(data_t));
    datap->src_ip_addr = 555;
    datap->dest_ip_addr = 555;
    list_insert(Lptr, datap, LISTPOS_HEAD);
    datap = NULL;

    // test list_access with one item in list
    datap = list_access(Lptr, LISTPOS_HEAD);
    printf("Should find 555 and found (%d)\n\n", datap->src_ip_addr);
    datap = NULL;

    // add a second item to tail of the list
    datap = (data_t *) calloc(1,sizeof(data_t));
    datap->src_ip_addr = 444;
    datap->dest_ip_addr = 444;
    list_insert(Lptr, datap, LISTPOS_TAIL);
    datap = NULL;

    // add a third item
    datap = (data_t *) calloc(1,sizeof(data_t));
    datap->src_ip_addr = 500;
    datap->dest_ip_addr = 500;
    list_insert(Lptr, datap, LISTPOS_TAIL);
    datap = NULL;


    // find all three and print 
    datap = list_access(Lptr, 0); 
    printf("Second test\nPosition 0 should find 555 and found (%d)\n", datap->src_ip_addr);
    datap = list_access(Lptr, 1);
    printf("Position 1 should find 444 and found (%d)\n", datap->src_ip_addr);
    datap = list_access(Lptr, 2);
    printf("Position 2 should find 500 and found (%d)\n", datap->src_ip_addr);

    //Next try to use list_debug_print 
    printf("\nTest of list print\n\n");
    list_debug_print(Lptr);


    // Uncomment this section to test list_elem_find 
   
    data_t template;
    int my_index = -999;
    template.src_ip_addr = 444;
    template.dest_ip_addr = 444;
    data_t *foundp = list_elem_find(Lptr, &template, &my_index);
    printf("\nTest of list elem find\n");
    if (foundp != NULL)
        printf("looked for %d and found %d at index %d\n",
            template.src_ip_addr, foundp->src_ip_addr, my_index);
    else
        printf("looked for %d and did not find \n", template.src_ip_addr);
    foundp = NULL;

    

    list_destruct(Lptr);
    // End of tests with unsorted list
 
    // Uncomment this section to try some tests on a sorted list 
    list_t *Lsortptr = list_construct(trte_compare, trte_route_rec_cleanup);

    datap = (data_t *) calloc(1,sizeof(data_t));
    datap->src_ip_addr = 23;
    datap->dest_ip_addr = 23;
    list_insert_sorted(Lsortptr, datap);
    datap = NULL;

    // add a second item
    datap = (data_t *) calloc(1,sizeof(data_t));
    datap->src_ip_addr = 7;
    datap->dest_ip_addr = 7;
    list_insert_sorted(Lsortptr, datap);
    datap = NULL;

    // add a third item
    datap = (data_t *) calloc(1,sizeof(data_t));
    datap->src_ip_addr = 15;
    datap->dest_ip_addr = 15;
    list_insert_sorted(Lsortptr, datap);
    datap = NULL;

    //Next try to use list_debug_print 
    printf("\nHere is the sorted list\n\n");
    list_debug_print(Lsortptr);
    template.src_ip_addr = 7;
    template.dest_ip_addr = 7;
	list_elem_find(Lsortptr,&template,&my_index);
	if (my_index != -1)
        printf("looked for %d and found %d at index %d\n",
            template.src_ip_addr, foundp->src_ip_addr, my_index);
    else
        printf("looked for %d and did not find \n", template.src_ip_addr);
    foundp = NULL;
    list_reverse(Lsortptr);
    list_debug_print(Lsortptr);
    list_destruct(Lsortptr);

    return 0;
}

/*

Next you will want to write your own list_debug_print function to print a
list. Then you can do "before and after" testing. That is, print the list
before a change, and print the list after the change to verify that the
change worked.

Something like

*/

void list_debug_print(list_t *list_ptr) {
    list_node_t *L;
    int count;

    printf("List size: %d\n", list_ptr->current_list_size);
    L = list_ptr->head;
    count = 0;
    while(L != NULL) {
        printf("Pos: %d contains IP: %d\n", count, L->data_ptr->src_ip_addr);
        L = L->next;
        count++;
    }
    if (count != list_ptr->current_list_size)
        printf("Hey!! the list size is wrong! count is %d\n", count);
}

