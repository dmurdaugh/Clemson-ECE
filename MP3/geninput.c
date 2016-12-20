/* geninput.c
 * MP3
 * Harlan Russell
 * ECE 223, Spring 2016
 *
 * Compile with 
 *     gcc -g -Wall geninput.c -o geninput 
 *
 * Purpose: Generate an input file for MP3 
 * Assumptions: Input file is for testing sort.  Creates
 *              an unsorted list, uses ADDTAIL to insert items, and
 *              calls SORT to sort the list
 *
 * Command line arguments:
 *    1st -- number of records to create
 *    2nd -- type of list to create
 *           1: random addresses
 *           2: assending and sequential
 *           3: descending and sequential
 *    3rd -- type of sort
 *           1: Insertion
 *           2: Recursive Selection
 *           3: Iterative Selection
 *           4: Merge
 *    4th -- If sort order should change (optional)
 *           1: keep default as ascending
 *           2: sort in descending order
 *
 *
 * Pipe the output of this program into lab3. For example
 *     ./geninput 10000 1 1 | ./lab3 4
 *
 * See also mp3test.sh 
 */

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int records = -1, list_type = -1, sort_type = -1;
    int src_addr_range, dest_addr_range;
    int src_ip_addr;
    int dest_ip_addr;
    int sort_order = 1;
    int seed = 116773734;

    if (argc !=4 && argc !=5) {
        printf("Usage: ./geninput listsize listype sorttype [sortorder]\n");
        printf("\tlistsize:  number of records to create\n");
        printf("\tlisttype:  {1, 2, 3} for random, ascending or descending\n");
        printf("\tsorttype:  1: Insertion\n");
        printf("\t           2: Recursive Selection\n");
        printf("\t           3: Iterative Selection\n");
        printf("\t           4: Merge\n");
        printf("\tsortorder: 1: Ascending (default)\n");
        printf("\t           2: Descending\n");
        exit(1);
    }
    records = atoi(argv[1]);
    if (records < 2) {
        printf("genniput has invalid number records: %d\n", records);
        exit(2);
    }
    list_type = atoi(argv[2]);
    sort_type = atoi(argv[3]);
    if (sort_type < 1 || sort_type > 4) {
        printf("genniput has invalid type of sort: %d\n", sort_type);
        exit(2);
    }
    if (argc == 5) {
        sort_order = atoi(argv[4]);
        if (sort_order < 1 || sort_order > 2) {
            printf("genniput has invalid sort order: %d\n", sort_order);
            exit(3);
        }
    }
    if (records > 500)
        src_addr_range = records/100;
    else if (records > 50)
        src_addr_range = records/10;
    else
        src_addr_range = records/4;
    dest_addr_range = records * 0.75;
    srand48(seed);

    int i;
    if (list_type == 1) {
        // random addresses, 
        for (i = 0; i < records; i++) {
            if (i == records/4 || i == records/2 || i == 3*records/4) {
                src_ip_addr = records;
                dest_ip_addr = records;
            } else {
                src_ip_addr = (int) (src_addr_range * drand48()) + 1;
                dest_ip_addr = (int) (dest_addr_range * drand48()) + 1;
            }
            printf("ADDTAIL %d %d\n", src_ip_addr, dest_ip_addr);
        }
    }
    else if (list_type == 2) {
        // ascending addresses
        for (i = 0; i < records; i++) {
            printf("ADDTAIL %d %d\n", i+1, 1);
        }
    }
    else if (list_type == 3) {
        // descending addresses
        for (i = 0; i < records; i++) {
            printf("ADDTAIL %d %d\n", 1, records - i);
        }
    }
    else {
        printf("geninput has invalid list type: %d\n", list_type);
        exit(4);
    }
    
    printf("SORT %d %d\n", sort_type, sort_order);
    printf("PRINTQ\n");
    printf("QUIT\n");
    exit(0);
}

