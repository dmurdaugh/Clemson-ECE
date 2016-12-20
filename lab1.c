/* lab1.c
 * Denzel Murdaugh                       <--- replace with your name!
 * DenzelM                             <--- replace with your user name!
 * ECE 223 Spring 2016
 * MP1
 *
 * To submit send email to ece_assign@clemson.edu
 * subject header: ECE223-1,#1
 * Attached all files to the email
 *
 * NOTE:  You must update all of the following comments!
 *
 * Purpose: A template for MP1 
 *
 * Assumptions: Many details are incomplete.  The functions to collect input
 * for a record and to print a record specify the format that is required for
 * grading.
 *
 * The program accepts one command line arguement that is the size of the list.
 *
 * An outline for the interactive menu input is provided.  Details need to be
 * completed but format of the commands and the output generated by the
 * print statements must not be changed. You have to call the corrent 
 * print statement in response to the input
 *
 * Bugs: Many detail have not been implemented.
 *
 * See the ECE 223 programming guide
 *
 * NOTE: if it forbidden to access any of the members in the tracert_t
 * structure.   The members that start with trte followed by an underscore
 * MUST NOT be found in this file or it is a design violation.  Instead you 
 * must utilize the tracert_* fuctions found in the tracert.h header file to
 * access any details of the list.
 *
 * One of the requirements is to verify you program does not have any 
 * memory leaks or other errors that can be detected by valgrind.  Run with
 * your test script(s):
 *      valgrind --leak-check=full ./lab1 5 < your_test_script
 *  and replace "5" with the value that makes sense for your test script
 * 
 * Are you unhappy with the way this code is formatted?  You can easily
 * reformat (and automatically indent) your code using the astyle 
 * command.  If it is not installed use the Ubuntu Software Center to 
 * install astyle.  Then in a terminal on the command line do
 *     astyle --style=kr lab1.c
 *
 * See "man astyle" for different styles.  Replace "kr" with one of
 * ansi, java, gnu, linux, or google to see different options.  Or, set up 
 * your own style.
 *
 * To create a nicely formated PDF file for printing install the enscript 
 * command.  To create a PDF for "file.c" in landscape with 2 columns do:
 *     enscript file.c -G2rE -o - | ps2pdf - file.pdf
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "tracert.h"

#define MAXLINE 256
#define FALSE 0
#define TRUE 1

// function prototypes for lab1.c
void fill_route_record(struct route_info_t *new);
void print_route_rec_short(struct route_info_t *rec);
void print_route_rec_long(struct route_info_t *rec);

int main(int argc, char *argv[])
{
    char line[MAXLINE];
    char command[MAXLINE];
    char junk[MAXLINE];
    int list_size = -1;
    int num_items;
    int input_src_ip, input_dest_ip;
    int i;
    int found = FALSE;
    struct route_info_t *rec_ptr = NULL;
    struct tracert_t *new_trace;

    if (argc != 2) {
        printf("Usage: ./lab1 list_size\n");
        exit(1);
    }
    list_size = atoi(argv[1]);
    if (list_size < 1) {
        printf("Invalid list size %d\n", list_size);
        exit(2);
    } 
    printf("The list size is %d. Possible commands:\n", list_size);
    printf("INSERT\nFIND s or FIND s d\nREMOVE s or REMOVE s d\nPRINT\nSTATS\nREVERSE\nQUIT\n");

    new_trace=tracert_construct(list_size);
    // remember fgets includes newline \n unless line too long 
    while (fgets(line, MAXLINE, stdin) != NULL) {
        num_items = sscanf(line, "%s %d %d %s", command, &input_src_ip, &input_dest_ip, junk);
        if (num_items == 1 && strcmp(command, "QUIT") == 0) {
            tracert_destruct(new_trace);
            printf("Goodbye\n");
            break;
        } else if (num_items == 1 && strcmp(command, "INSERT") == 0) {
            rec_ptr = (struct route_info_t *) malloc(sizeof(struct route_info_t));   // fix! (done)
            fill_route_record(rec_ptr);

            // use tracert_add to get return value (done)
            int added_return = tracert_add(new_trace,rec_ptr);

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
                exit(1);
            }
            rec_ptr = NULL;
        } else if (num_items == 2 && strcmp(command, "FIND") == 0) {
            int index = tracert_lookup_first_src(new_trace,input_src_ip);
            rec_ptr = tracert_access(new_trace,index);   // fix to use lookup_first_src and access! (done)

            if (rec_ptr == NULL) {
                printf("Did not find: %d\n", input_src_ip);
            } else {
                /* print items in structure */
                do {
                    printf("Found: %d at index %d\n", input_src_ip, index);
                    print_route_rec_long(rec_ptr);
                    // check next index position to see if same source IP.
                    // if the next index position does not match change
                    // found is false. (done)
                    
                    if(index+1<list_size){
                        index++;
                        rec_ptr = tracert_access(new_trace,index);
                        if(rec_ptr == NULL){
                            found=FALSE;
                        }
                        else if(rec_ptr->src_ip_addr!=input_src_ip){
                            found=FALSE;
                            
                        }
                        else if(rec_ptr->src_ip_addr==input_src_ip){
                            found=TRUE;
                        }
                     else{
                        found = FALSE;
                        }
                    }
                    else{
                        found=FALSE;
                    }
                } while (found);
            }
            rec_ptr = NULL;
        } else if (num_items == 3 && strcmp(command, "FIND") == 0) {
            int index = tracert_lookup_src_dest(new_trace,input_src_ip,input_dest_ip);
            rec_ptr = tracert_access(new_trace,index);   // fix to use lookup_src_dest and access! (done)

            if (rec_ptr == NULL) {
                printf("Did not find: %d %d\n", input_src_ip, input_dest_ip);
            } else {
                /* print items in structure */
                printf("Found: %d %d at index %d\n", 
                        input_src_ip, input_dest_ip,index);
                print_route_rec_long(rec_ptr);
            }
            rec_ptr = NULL;
        } else if (num_items == 2 && strcmp(command, "REMOVE") == 0) {
            int index = tracert_lookup_first_src(new_trace,input_src_ip);
            rec_ptr = tracert_remove(new_trace,index);   // fix to use lookup_first_src and remove! (done)

            if (index == -1) {
                printf("Did not remove: %d\n", input_src_ip);
            } else {
                do {
                    printf("Removed: %d\n", input_src_ip);
                    print_route_rec_long(rec_ptr);
                    free(rec_ptr->routelist);
                    free(rec_ptr);
                    // if there is another matching record found should be 
                    // true  
                        rec_ptr = tracert_access(new_trace,index);
                    if(rec_ptr == NULL){
                            found= FALSE;
                        }
                    else if(rec_ptr != NULL){
                            if(rec_ptr->src_ip_addr!=input_src_ip){
                                found=FALSE;
                            }
                            else if(rec_ptr != NULL){
                                if(rec_ptr->src_ip_addr==input_src_ip){
                                rec_ptr = tracert_remove(new_trace,index);
                                found=TRUE;
                                }
                            }
                        }
                    else{
                        found=FALSE;
                    }
                } while (found);
            }
            rec_ptr = NULL;
        } else if (num_items == 3 && strcmp(command, "REMOVE") == 0) {
            int index = tracert_lookup_src_dest(new_trace,input_src_ip,input_dest_ip);
            rec_ptr = tracert_remove(new_trace,index);   // fix to use lookup_src_dest and remove!

            if (index == -1) {
                printf("Did not remove: %d %d\n", input_src_ip, input_dest_ip);
            } else {
                printf("Removed: %d %d\n", input_src_ip, input_dest_ip);
                print_route_rec_long(rec_ptr);
            }
            rec_ptr = NULL;
        } else if (num_items == 1 && strcmp(command, "STATS") == 0) {
            // get the number in list and order of the list
            int num_in_list = tracert_number_entries(new_trace);   // fix!
            int order_of_list = tracert_order(new_trace);
            printf("Number records: %d, Order: %s\n", num_in_list, 
                    order_of_list == 1 ? "Ascending" : "Descending");
        } else if (num_items == 1 && strcmp(command, "REVERSE") == 0) {
            tracert_reverse(new_trace);
            int order_of_list = tracert_order(new_trace);
            printf("List reversed, new order: %s\n",
                    order_of_list == 1 ? "Ascending" : "Descending");
        } else if (num_items == 1 && strcmp(command, "PRINT") == 0) {
            int num_in_list = tracert_number_entries(new_trace);   // fix!
            if (num_in_list == 0) {
                printf("List empty\n");
            } else {
                printf("List has %d records\n", num_in_list);
                for (i = 0; i < num_in_list; i++) {
                    printf("%4d: ", i);
                    rec_ptr = tracert_access(new_trace,i);    // fix to use tracert_access(ptr, i)!
                    print_route_rec_short(rec_ptr);
                }
            }
            rec_ptr = NULL;
        } else {
            printf("# %s", line);
        }
    }
    exit(0);
}

/* If a string ends with an end-of-line \n, remove it.
 */
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
void fill_route_record(struct route_info_t *new)
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
void print_route_rec_short(struct route_info_t *rec)
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
void print_route_rec_long(struct route_info_t *rec)
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