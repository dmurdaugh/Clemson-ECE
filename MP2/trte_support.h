/* trte_support.h       <<-- A template
 * Denzel Murdaugh        <<-- some changes needed
 * Denzelm
 * ECE 223 Spring 2016 
 * MP2
 *
 * Propose: A template for MP2 
 *
 * Assumptions: suggested structure definitions and public functions 
 *              as defined for assignmnet.  
 *
 *              You may change this file if needed for your design.
 *
 * Bugs:
 *
 */

#define MAXLINE 120

/* prototype function definitions */

/* function to compare trace route records */
int trte_compare(const route_t *rec_a, const route_t *rec_b);

/* function to free memory in a trace route record */
void trte_route_rec_cleanup(route_t *rec);

/* functions to create and cleanup a route record list */
list_t * trte_create(void);
void trte_cleanup(list_t *);

/* Functions to get and print packet information */
void trte_print(list_t * list_ptr, char *); /* print list of records */
void trte_print_head(list_t *list_ptr);     /* print record at head */
void trte_stats(list_t *, list_t *);        /* prints size of each list */


/* functions for sorted list that replaces duplicate records, and has a maximum
 * size for the list */
void trte_add(list_t *, int max_list_size);
void trte_find_first(list_t * , int);
void trte_find_head(list_t *);
void trte_find(list_t * , int, int);
void trte_remove_first(list_t * , int);
void trte_remove(list_t * , int, int);
void trte_reverse(list_t *);

/* functions for unsorted FIFO list that has no limit of the size of the list,
 * inserts at the tail, removes at the head, and does not check for duplicate
 * elements in the list */
void trte_add_tail(list_t *);
void trte_remove_head(list_t * );

/* commands for vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
