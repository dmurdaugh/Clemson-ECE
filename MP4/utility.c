/* utility functions for MP4 
 * ECE 223 Spring 2016
 * Harlan Russell
 * Version: 1
 *
 * This file has support functions for
 *
 *  -- two memory pools for event_t and packet_t structures
 *  -- functions to generate exponential, normal, and truncated normal random
 *     variables
 *  -- a comparison function for event_t structures that can be used by list.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "datatypes.h"
#include "utility.h"

/* structures for this file only */

/* this structure is used by the stacks to store unusued memory in the
 * memory pools.  Both the event and packet pools use the same structure
 * for building the memory stack
 */
typedef struct mem_node_tag {
    void *item;   // void allows any type of structure to be saved
    struct mem_node_tag *next;
} mem_node_t;

/* variables available in this file only */

/* the three memory pools for storing unused memory */
static mem_node_t *PacketMemList = NULL;
static mem_node_t *EventMemList = NULL;
static mem_node_t *MemNodeList = NULL;

/* counters that track statistics for the memory pools */
static int NumPackets = 0;
static int MaxAllocatedPackets = 0;
static int NumEvents = 0;
static int MaxAllocatedEvents = 0;
static int NumMemNodes = 0;
static int MaxAllocatedMemNodes = 0;

/* prototypes for this file only */

/* the functions to allocate and free the structure for storing unused event
 * and packet memory blocks
 */
mem_node_t *alloc_mem_node(void);
void free_mem_node(mem_node_t *M);
double rand_normal(void);

/* Uses pooled memory to store packets.  If a packet is in the pool it is
 * returned, otherwise a new packet is allocated.
 *
 * Must use call to free_packet to return packet to memory pool.
 */
packet_t *alloc_packet()
{
    packet_t *pkt_ptr;
    mem_node_t *mem_ptr;

    if (PacketMemList == NULL) {
        pkt_ptr = (packet_t *) malloc(sizeof(packet_t));
        MaxAllocatedPackets++;
    } else {
        /* pop packet off of unused memory stack */
        pkt_ptr = (packet_t *) PacketMemList->item;
        mem_ptr = PacketMemList;
        PacketMemList = PacketMemList->next;
        mem_ptr->item = NULL;
        free_mem_node(mem_ptr);
    }
    NumPackets++;
    return pkt_ptr;
}

/* Returns a packet to the memory pool for packets.  Packet must have been
 * generated with a call to alloc_packet.  A stack is used to store packets in
 * the memory pool.  We also use a memory pool for the linked list that
 * implements the stack.
 */
void free_packet(packet_t *P)
{
    mem_node_t *mem_ptr;
    assert(NumPackets > 0);
    /* push packet onto stack */
    mem_ptr = alloc_mem_node();
    mem_ptr->item = (void *) P;
    mem_ptr->next = PacketMemList;
    PacketMemList = mem_ptr;
    NumPackets--;
}

/* Uses pooled memory to store event structures.  If event structure is in the
 * pool it is returned, otherwise a new event structure is allocated.
 *
 * Must use call to free_event to return event structure to memory pool.
 */
event_t *alloc_event()
{
    event_t *event_ptr;
    mem_node_t *mem_ptr;

    if (EventMemList == NULL) {
        event_ptr = (event_t *) malloc(sizeof(event_t));
        MaxAllocatedEvents++;
    } else {
        /* pop event off of unused memory stack */
        event_ptr = (event_t *) EventMemList->item;
        mem_ptr = EventMemList;
        EventMemList = EventMemList->next;
        mem_ptr->item = NULL;
        free_mem_node(mem_ptr);
    }
    NumEvents++;
    return event_ptr;
}


/* Returns a event to the memory pool for events.  Event must have been
 * generated with a call to alloc_event.  A stack is used to store events in
 * the memory pool.  We also use a memory pool for the linked list that
 * implements the stack.
 */
void free_event(event_t *E)
{
    mem_node_t *mem_ptr;
    assert(NumEvents > 0);
    /* push event onto unused memory stack */
    mem_ptr = alloc_mem_node();
    mem_ptr->item = (void *) E;
    mem_ptr->next = EventMemList;
    EventMemList = mem_ptr;
    NumEvents--;
}

/* The memory pools are simple stacks implemented with a (one-way) linked list.
 * The elements in the linked list can store any type of memory block using the
 * item pointer.  The item pointer has type void, so cast pointers to void when
 * pushing on stack and cast pointer as appropriate type when popping from
 * stack.
 */
mem_node_t *alloc_mem_node()
{
    mem_node_t *mem_ptr;

    if (MemNodeList == NULL) {
        mem_ptr = (mem_node_t *) malloc(sizeof(mem_node_t));
        MaxAllocatedMemNodes++;
    } else {
        /* pop off of stack */
        mem_ptr = MemNodeList;
        MemNodeList = MemNodeList->next;
    }
    NumMemNodes++;
    return mem_ptr;
}

/* Returns a item to the memory pool for storing structures that are currently
 * not in use.
 */
void free_mem_node(mem_node_t *M)
{
    assert(NumMemNodes > 0);
    /* push onto stack */
    M->next = MemNodeList;
    MemNodeList = M;
    NumMemNodes--;
}

/* At end of simulation return all memory to dynamic memory manager.
 *
 * Note this invalidates the counters used in print_mem_stats
 */
void cleanup_mem_pools()
{
    mem_node_t *mem_ptr;
    while (EventMemList != NULL) {
        mem_ptr = EventMemList;
        EventMemList = EventMemList->next;
        free(mem_ptr->item);
        free(mem_ptr);
    }
    while (PacketMemList != NULL) {
        mem_ptr = PacketMemList;
        PacketMemList = PacketMemList->next;
        free(mem_ptr->item);
        free(mem_ptr);
    }
    while (MemNodeList != NULL) {
        mem_ptr = MemNodeList;
        MemNodeList = MemNodeList->next;
        free(mem_ptr);
    }
}

/* Prints statistics associated with the three memory pools: Packet, Event, and
 * Memory node (for storing unused mem_node_t structures.
 *
 * Current allocated: number of items that have been taken from memory pool and
 * are stored within the discrete-event simulation.  This number should be zero
 * at the end of the simulation if all packets and events have been processed.
 * 
 * Avail on List: number of stuctures in memory pool and available for use by
 * the discrete-event simulation.  
 *
 * Max allocated: the largest size of the memory pool.
 */
void print_mem_stats()
{
    printf("\n");
    printf("Memory type   Current allocated  Avail on list  Max allocated\n");
    printf("-----------   -----------------  -------------  -------------\n");
    printf(" Packet       %15d    %11d    %10d\n", NumPackets, 
            MaxAllocatedPackets - NumPackets, 
            MaxAllocatedPackets);
    printf(" Event        %15d    %11d    %10d\n", NumEvents, 
            MaxAllocatedEvents - NumEvents, 
            MaxAllocatedEvents);
    printf(" Mem nodes    %15d    %11d    %10d\n", NumMemNodes, 
            MaxAllocatedMemNodes - NumMemNodes, 
            MaxAllocatedMemNodes);
}


/* creates an exponentially distributed random number.  The mean must be 
 * greater than zero
 */
double rand_exp(double mean)
{
    assert(mean > 0.0);
    return (- mean * log(drand48()));
}

/* creates a positive random number with a truncated normal distribution.  
 *
 * The truncation is an non-stardard way to use normal random variables and
 * should generally be avoided.  Note that with probability 0.9986 the value of
 * the random variable is greater than x, where x = mean - 3*std_dev.  For 2
 * standard deviations, the probability is 0.9772
 *
 * Thus, the truncation has little effect if std_dev < mean / 3.
 *
 * The Poisson distribution with parameter lambda is approximately normal
 * if lambda is large, for a normal distribution with mean = var = lambda.
 */
double rand_normal_pos(double mean, double std_dev)
{
    double z;
    assert(std_dev > 0.0);
    do {
        z = std_dev * rand_normal() + mean;
    } while (z <= 0.0);
    return z;
}

/* Creates a random variable with a standard normal distribution (i.e., zero
 * mean and unit variance).  Uses the Box-Muller transform to generate two
 * values and returns one of them.  On next call the other value is returned.
 */
double rand_normal()
{
    double x, y, base, s;
    double z_0;
    static double z_1;
    static int stored_value = 0;

    if (stored_value == 0) {
        do {
            x = 2.0 * drand48() - 1;
            y = 2.0 * drand48() - 1;
            s = x*x + y*y;
        } while(s > 1.0 || s <= 0.0);
        /* this will overflow if s is too small */
        base = sqrt(-2.0 * log(s)/s);
        z_0 = x * base;
        z_1 = y * base;
        stored_value = 1;
    } else {
        z_0 = z_1;
        stored_value = 0;
    }
    return z_0;
}

/* Events are stored in the priority queue according to time.  This function
 * returns 1 if event a occurs before event b, -1 if b occurs first, and 0 if
 * the events have the same time.
 *
 * Note: it is generally a poor practice to compare floating point variables
 * for equality.
 */
int event_compare(const event_t *a, const event_t *b)
{
    assert(a != NULL && b != NULL);

    if (a->time < b->time)
        return 1;
    else if (a->time > b->time)
        return -1;
    else
        return 0;
}

/* vi:set ts=8 sts=4 sw=4 et: */
