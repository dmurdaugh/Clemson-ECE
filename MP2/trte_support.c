/* trte_support.c         <<-- A template 
 * Denzel Murdaugh          <<-- many updates required
 * Denzelm
 * ECE 223 Spring 2016 
 * MP2
 *
 * Propose: A template for MP2 
 *
 * Assumptions: 
 *
 * Bugs:
 *
 * You can modify the interface for any of the trte_ functions if you like
 * But you must clearly document your changes.
 *
 * (You CANNOT modify any of the details of the list.h interface, or use any 
 *  of the private variables outside of list.c.)
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "datatypes.h"
#include "list.h"
#include "trte_support.h"

#define FALSE 0
#define TRUE  1

// private functions for use in trte_support.c only

void fill_route_record(route_t *new);          // collect input
void print_route_rec_short(route_t *rec);      // print one record
void print_route_rec_long(route_t *rec) ;      // print one record

/* trte_compare is required by the list ADT for sorted lists. 
 *
 * This function returns 
 *     1 if rec_a should be closer to the head than rec_b,
 *    -1 if rec_b is to be considered closer to the head, and 
 *     0 if the records are equal.
 *
 * For the packet data we want to sort from lowest IP address up, so
 * closer to the front means a smaller IP address.
 *
 * The function expects pointers to two record structures, and it is an error
 * if either is NULL
 *
 * THIS FUNCTION SHOULD NOT BE CHANGED
 */
int trte_compare(const route_t *record_a, const route_t *record_b)
{
    assert(record_a != NULL && record_b !=NULL);
    if (record_a->src_ip_addr < record_b->src_ip_addr)
        return 1;
    else if (record_a->src_ip_addr > record_b->src_ip_addr)
        return -1;
    else {
        // source matches so check destination
        if (record_a->dest_ip_addr < record_b->dest_ip_addr) {
            return 1;
        } else if (record_a->dest_ip_addr > record_b->dest_ip_addr) {
            return -1;
        } else {
            return 0;
        }
    }
}

/* Function to pass into List ADT during construction to free the memory 
 * associate with a route record.  This hides the details of how the record
 * is defined from the List ADT.
 */
void trte_route_rec_cleanup(route_t *rec)
{
    if (rec != NULL) {
        free(rec->routelist);
        rec->routelist = NULL;
        free(rec);
    }
}

/* print one of the packet record lists
 *
 * inputs: 
 *    list_ptr: a list_t * pointer to either sorted or unsorted list
 *
 *    type_of_list: a charter string that must be either "List" or "Queue"
 *
 * output: prints the list in the format required for grading.
 *         Do not change any of these lines 
 */
void trte_print(list_t * list_ptr, char *list_type)
{
    assert(strcmp(list_type, "List")==0 || strcmp(list_type, "Queue")==0);

    int i, num_in_list = list_size(list_ptr);   // fix! list_size
    route_t *rec_ptr = list_access(list_ptr,LISTPOS_HEAD);// fix
    if (num_in_list == 0) {
        printf("%s empty\n", list_type);
    } else {
        printf("%s has %d record%s\n", list_type, num_in_list,
                num_in_list==1 ? "." : "s");
        for (i = 0; i < num_in_list; i++) {
            printf("%4d: ", i);
            rec_ptr = list_access(list_ptr,i);    // fix to use list_access(ptr, i)!
            print_route_rec_short(rec_ptr);
        }
    }
    rec_ptr = NULL;
}

/* This functions adds a packet record to the tail of a list.  The list is
 * unsorted, and it does not have any limits on the maximum size of the list.
 */
void trte_add_tail(list_t * list_ptr)
{
    route_t *rec_ptr = (route_t *) malloc(sizeof(route_t));  // fix
    fill_route_record(rec_ptr);
    list_insert(list_ptr,rec_ptr,LISTPOS_TAIL);
    printf("Appended %d %d onto queue\n", rec_ptr->src_ip_addr, rec_ptr->dest_ip_addr);
}

/* This function removes the packet record at the head of the list.  The
 * list is unsorted, and there is no limit on the maximum size of the list.
 *
 * The packet should be recycled so there are no memory leaks.
 */
void trte_remove_head(list_t * list_ptr)
{
    route_t *pkt_ptr = list_remove(list_ptr,0);
    if (pkt_ptr != NULL)
        printf("Deleted head with IP addrs: %d %d\n", 
                pkt_ptr->src_ip_addr, pkt_ptr->dest_ip_addr);
    else
        printf("Queue empty, did not remove\n");
}

/* This creates a list and it can be either a sorted or unsorted list
 *
 * This function is provided to you as an example of how to use 
 * function pointers.  
 */
list_t * trte_create(void)
{
    return list_construct(trte_compare, trte_route_rec_cleanup);
}

/* This function adds a packet to the sorted list.  
 * If the new packet matches a packet in the list, the matching packet
 * is removed and the new one inserted in its place.
 *
 * Otherwise, if the list is full the packet is rejected.
 */
void trte_add(list_t * list_ptr, int max_list_size)
{
    route_t *rec_ptr = (route_t *)malloc (sizeof(route_t));  // fix
    fill_route_record(rec_ptr);

    // determine the correct return value 
    int added_return = 1;
    int index;
    route_t match;
    route_t *match_2=NULL;
    match.src_ip_addr = rec_ptr->src_ip_addr;
    match.dest_ip_addr= rec_ptr->dest_ip_addr;
    int size=list_size(list_ptr);
    if(size!=0){
        if(size==max_list_size){
            match_2=list_elem_find(list_ptr,&match,&index);
            if(match_2==NULL){
                added_return = -1;
            }
            else{
                match_2=list_access(list_ptr,index);
                if(match_2->dest_ip_addr==rec_ptr->dest_ip_addr){
                    //list_remove(list_ptr,index);
                    list_insert_sorted(list_ptr,rec_ptr);
                    added_return=0;
                 }
                 else{
                    added_return=-1;
                 }    
             }
        }else{
             list_elem_find(list_ptr,&match,&index);
             if(match_2==NULL){
                list_insert_sorted(list_ptr,rec_ptr);
                added_return=1;
            }
            else{
                match_2=list_access(list_ptr,index);
                if(match_2->dest_ip_addr==rec_ptr->dest_ip_addr){
                    list_insert_sorted(list_ptr,rec_ptr);
                    added_return=0;
                 }
                 else{
                    list_insert_sorted(list_ptr,rec_ptr);
                    added_return=1;
                 }    
             }
        }
    }
    else{
        list_insert_sorted(list_ptr,rec_ptr);
        added_return=1;
    }

    if (added_return == 1) {
        printf("Inserted: %d\n", rec_ptr->src_ip_addr);
    } else if (added_return == 0) {
        printf("Updated: %d\n", rec_ptr->src_ip_addr);
    } else if (added_return == -1) {
        printf("Rejected: %d\n", rec_ptr->src_ip_addr);
        free(rec_ptr->routelist);
        free(rec_ptr);
        
    } else {
        printf("Error with return value! Fix your code.\n");
    }
    rec_ptr = NULL;
}

/* This function prints all packets with the matching source IP address in the
 * sorted list.  Print all fields of each matching packet record. 
 */
void trte_find_first(list_t * list_ptr, int id)
{
    int index = 0;
    int found = FALSE;
    int size=list_size(list_ptr);
    route_t *rec_ptr = list_access(list_ptr,index);   // fix to use ?
    if(rec_ptr != NULL && rec_ptr->src_ip_addr==id){
                found=TRUE;
    }
    if(found!=TRUE){
        while(index < size){
            index++;
            rec_ptr = list_access(list_ptr,index);
            if(rec_ptr != NULL && rec_ptr->src_ip_addr==id){
                    found=TRUE;
                    break;
            }
        }
    }
    if (rec_ptr == NULL) {
        printf("Did not find: %d\n", id);
    } else {
        /* print items in structure */
        do {
            printf("Found: %d at index %d\n", id, index);
            print_route_rec_long(rec_ptr);

            // check next index position to see if same source IP.
            // if the next index position does not match change
            // found is false.
            index++;
            rec_ptr=list_access(list_ptr,index);
            if(rec_ptr != NULL && rec_ptr->src_ip_addr==id ){
                found=TRUE;
            }
            else{
                found=FALSE;
            }
        } while (found);
    }
    rec_ptr = NULL;
}

/* This function prints the packet with matching source and destination IP 
 * address in the sorted list.  Print all fields of the packet record. 
 */
void trte_find(list_t * list_ptr, int src_ip, int dest_ip)
{
    int index;
    route_t *rec_ptr = NULL;   // fix to use list_elem_find!
    route_t match;
    match.src_ip_addr = src_ip;
    match.dest_ip_addr = dest_ip;
    rec_ptr = list_elem_find(list_ptr, &match, &index);
    if (index == -1) {
        printf("Did not find: %d %d\n", src_ip, dest_ip);
    } else {
        /* print items in structure */
        printf("Found: %d %d at index %d\n", src_ip, dest_ip,index);
        print_route_rec_long(rec_ptr);
    }
}

/* This function prints the packet at the head of the queue.
 * Print all fields of the packet record. 
 */
void trte_find_head(list_t * list_ptr)
{
    route_t *rec_ptr = list_access(list_ptr,LISTPOS_HEAD);   // fix 

    if (rec_ptr == NULL) {
        printf("Queue is empty\n");
    } else {
        /* print items in structure */
        printf("Found at head of queue:\n");
        print_route_rec_long(rec_ptr);
    }
}
/* This function removes all packets from the sorted list with the matching
 * source ip address
 */
void trte_remove_first(list_t * list_ptr, int input_src_ip)
{
    int index = 0;
    int found = FALSE;
    int size = list_size(list_ptr);
    route_t *rec_ptr = list_access(list_ptr,index);   // fix to use ?
    if(rec_ptr != NULL && rec_ptr->src_ip_addr==input_src_ip){
                found=TRUE;
    }
    if(found!=TRUE){
        while(index < size){
            index++;
            rec_ptr = list_access(list_ptr,index);
            if(rec_ptr != NULL && rec_ptr->src_ip_addr==input_src_ip){
                    found=TRUE;
                    break;
            }
        }
    }
    if (rec_ptr == NULL) {
        printf("Did not remove: %d\n", input_src_ip);
    } else {
        do {
           
            printf("Removed: %d\n", input_src_ip); 
            print_route_rec_long(rec_ptr);
            rec_ptr=list_remove(list_ptr,index);
            // if there is another matching record found should be 
            // true
            rec_ptr=list_access(list_ptr,index);
            if(rec_ptr!=NULL){
                if(rec_ptr->src_ip_addr!=input_src_ip){
                    found=FALSE;
                }
            }
            else
            {
                found=FALSE;
            }
        } while (found);
    }
}

/* This function removes the packet from the sorted list with the matching
 * source and destination ip address
 */
void trte_remove(list_t * list_ptr, int input_src_ip, int input_dest_ip)
{
    int index = -1;
    route_t *rec_ptr = NULL;   // fix to use list_elem_find!
    route_t match;
    match.src_ip_addr = input_src_ip;
    match.dest_ip_addr = input_dest_ip;
    rec_ptr = list_elem_find(list_ptr, &match, &index);
    if (index == -1) {
        printf("Did not remove: %d %d\n", input_src_ip, input_dest_ip);
    } else {
        printf("Removed: %d %d\n", input_src_ip, input_dest_ip);
        print_route_rec_long(rec_ptr);
        rec_ptr=list_remove(list_ptr,index);
    }
}
/* Reverse the list using list_reverse and list_order
 */
void trte_reverse(list_t * list_ptr)
{
    
    list_reverse(list_ptr);
    
    int order_of_list = list_order(list_ptr);
    
    printf("List reversed, new order: %s\n",
            order_of_list == 1 ? "Ascending" : "Descending");
}

/* the function takes a pointer to each list and prints the
 * number of items in each list
 */
void trte_stats(list_t * sorted, list_t * unsorted)
{
    // get the number in list and size of the list
    int num_in_sorted_list = list_size(sorted);
    int num_in_unsorted_list = list_size(unsorted);
    int order_of_list = list_order(sorted);
    printf("Number records: %d, Order: %s\n", num_in_sorted_list, 
            order_of_list == 1 ? "Ascending" : "Descending");
    printf("Number records in queue: %d\n", num_in_unsorted_list);
}

/* this function frees the memory for either a sorted or unsorted list.
 */
void trte_cleanup(list_t * list_ptr)
{
    list_destruct(list_ptr);
}

/****************************************************************************/
//       Functions from here to end of file should not be changed

/* If a string ends with an end-of-line \n, remove it. */
void chomp(char *str) 
{
    int lastchar = strlen(str) - 1;
    if (lastchar >= 0 && str[lastchar] == '\n') {
        str[lastchar] = '\0';
    }
}

/* Prompts user for route record input starting with the source IP address.
 * The input is not checked for errors but will default to an acceptable value
 * if the input is incorrect or missing.
 *
 * The input to the function assumes that the structure has already been
 * created.  The contents of the structure are filled in.
 *
 * There is no output.
 *
 * DO NOT CHANGE THIS FUNCTION!
 */
void fill_route_record(route_t *new)
{
    int i, num_matches;
    char line[MAXLINE];
    assert(new != NULL);

    printf("Source IP address:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%u", &new->src_ip_addr);

    printf("Source name:");
    fgets(line, MAXLINE, stdin);
    chomp(line);
    if (strlen(line) >= MAXNAME || !isalpha(line[0])) {
        printf("Invalid source name in input %s\n", line);
        printf("Name must start with a letter.\n");
        printf("No way to recover so program terminates.  Fix input\n");
        exit(1);
    }
    strcpy(new->source, line);

    printf("Destination IP address:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%u", &new->dest_ip_addr);

    printf("Destination name:");
    fgets(line, MAXLINE, stdin);
    chomp(line);
    if (strlen(line) >= MAXNAME || !isalpha(line[0])) {
        printf("Invalid destination name in input %s\n", line);
        printf("Name must start with a letter.\n");
        printf("No way to recover so program terminates.  Fix input\n");
        exit(1);
    }
    strcpy(new->dest, line);

    printf("Number hops in route:");
    fgets(line, MAXLINE, stdin);
    new->hop_count = -1;
    sscanf(line, "%d", &new->hop_count);
    if (new->hop_count < 1 || new->hop_count > 255) {
        printf("Invalid hop count in input %d\n", new->hop_count);
        printf("No way to recover so program terminates.  Fix input\n");
        exit(1);
    }

    new->routelist = (unsigned int *) malloc(new->hop_count * sizeof(unsigned int));

    printf("\nInput list of %d IP addresses:\n", new->hop_count);
    for (i = 0; i < new->hop_count; i++) {
        printf("IP %d:", i);
        fgets(line, MAXLINE, stdin);
        num_matches = sscanf(line, "%u", &new->routelist[i]);
        if (num_matches != 1) {
            printf("Invalid IP address in list of hops: %s\n", line);
            printf("No way to recover so program terminates.  Fix input\n");
            exit(1);
        }
    }
    // add test to see if last address is destination?

    printf("Round trip delay:");
    fgets(line, MAXLINE, stdin);
    num_matches = sscanf(line, "%f", &new->round_trip_delay);
    if (num_matches != 1 || new->round_trip_delay < 0.0) {
        printf("Invalid round trip delay in input: %s\n", line);
        printf("No way to recover so program terminates.  Fix input\n");
        exit(1);
    }

    printf("Time received:");
    fgets(line, MAXLINE, stdin);
    new->time_received = -1;
    num_matches = sscanf(line, "%d", &new->time_received);
    if (num_matches != 1 || new->time_received < 0) {
        printf("Invalid time received in input: %s\n", line);
        printf("No way to recover so program terminates.  Fix input\n");
        exit(1);
    }
    printf("\n");
}

/* print the information for a particular route record in short form
 *
 * Input is a pointer to a record, and no entries are changed.
 *
 * DO NOT CHANGE THIS FUNCTION!
 */
void print_route_rec_short(route_t *rec)
{
    assert(rec != NULL);
    printf("src IP: %d, dest: %d\n", rec->src_ip_addr, rec->dest_ip_addr);
}

/* Long form to print a particular route record 
 *
 * Input is a pointer to a record, and no entries are changed.
 *
 * DO NOT CHANGE THIS FUNCTION!
 */
void print_route_rec_long(route_t *rec)
{
    int i;
    assert(rec != NULL);
    printf("    src: %d (%s), ", rec->src_ip_addr, rec->source);
    printf("dest: %d (%s), ", rec->dest_ip_addr, rec->dest);
    printf("delay: %g, time: %d\n", rec->round_trip_delay, rec->time_received);
    assert(rec->hop_count > 0 && rec->hop_count < 256);
    printf("    %d hops: (", rec->hop_count);
    for (i = 0; i < rec->hop_count; i++) {
        printf("%d%s", rec->routelist[i], i==rec->hop_count-1?")":",");
    }
    printf("\n");
}

/* commands specified to vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
