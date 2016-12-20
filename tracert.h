/* tracert.h 
 * Prof. Russell 
 * ECE 223 Spring 2016 
 * MP1
 *
 * Purpose: The interface definition for MP1.  DO NOT CHANGE THIS FILE
 *
 * Assumptions: structure definitions and public functions as defined for
 * assignmnet.  There are no extensions.
 *
 * Bugs:
 */

#define MAXNAME 32

struct route_info_t {
    unsigned int src_ip_addr;  // IP address at source of route
    unsigned int dest_ip_addr; // IP address of destination
    char source[MAXNAME];      // name of node at source of route
    char dest[MAXNAME];        // destination's name 
    int hop_count;             // number relays in route
    unsigned int *routelist;   // pointer to list of IP addresses in route
    float round_trip_delay;    // round trip delay of route
    int time_received;         // time that information last updated
};

// Header block for the list
struct tracert_t {
    int trte_order;       // if list is ascending or descending 
    int trte_count;       // current number of records in list
    int trte_size;        // size of the list
    struct route_info_t **trte_ptr;
};

//  The sequential list ADT must have the following interface:

struct tracert_t *tracert_construct(int size);
void tracert_destruct(struct tracert_t *);
void tracert_reverse(struct tracert_t *);
int tracert_number_entries(struct tracert_t *);
int tracert_order(struct tracert_t *);
int tracert_add(struct tracert_t *, struct route_info_t *);
int tracert_lookup_first_src(struct tracert_t *, int ip_addr);
int tracert_lookup_src_dest(struct tracert_t *, int src_ip, int dest_ip);
struct route_info_t *tracert_access(struct tracert_t *, int index);
struct route_info_t *tracert_remove(struct tracert_t *, int index);


