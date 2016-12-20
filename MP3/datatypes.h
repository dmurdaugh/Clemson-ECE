/* datatypes.h 
 * Denzel Murdaugh 
 * Denzelm
 * ECE 223 Spring 2016 
 * MP2
 *
 * Propose: The data type that is stored in the list ADT is defined here.  We
 *          define a single mapping that allows the list ADT to be defined in
 *          terms of a generic data_t.
 *
 * Assumptions: 
 *
 *     data_t: The type of data that we want to store in the list
 *
 * Bugs:
 * 
 */

#define MAXNAME 32

typedef struct route_info_tag {
    unsigned int src_ip_addr;  // IP address at source of route
    unsigned int dest_ip_addr; // IP address of destination
    char source[MAXNAME];      // name of node at source of route
    char dest[MAXNAME];        // destination's name 
    int hop_count;             // number relays in route
    unsigned int *routelist;   // pointer to list of IP addresses in route
    float round_trip_delay;    // round trip delay of route
    int time_received;         // time that information last updated
} route_t;

/* the list ADT works on packet data of this type */
typedef route_t data_t;

/* commands for vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
