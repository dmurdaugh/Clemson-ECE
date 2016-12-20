/* utility.h
 * utility functions for MP4 
 * ECE 223 Spring 2016
 * Harlan Russell
 * Version: 1
 *
 * See comments in utility.c
 */

// Two functions to create and recycle packet structures in a pool
packet_t *alloc_packet(void);
void free_packet(packet_t *P);

// Two functions to create and recycle event structures in a pool
event_t *alloc_event(void);
void free_event(event_t *E);

// Function to return all memory in pools back to dynamic memory manager
void cleanup_mem_pools(void);
// Before cleaning up pools, print how much memory is in each pool
void print_mem_stats(void);

// random number generators
//   exponential for inter-arrival time
double rand_exp(double mean);
//   Gaussian (truncated to be positive) for packet size
double rand_normal_pos(double mean, double std_dev);

// to compare events for insertion with list_insert_sorted into priority queue
int event_compare(const event_t *a, const event_t *b);

/* vi:set ts=8 sts=4 sw=4 et: */
