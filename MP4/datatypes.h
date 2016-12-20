/* datatypes.h 
 * Version: 1
 *
 * Example for MP4.  You may need to modify this to suit your implementation.
 * 
 * data_t: The type of data that we want to store in the list
 */

/* include the definitions of the data we want to store in the list */

// Model for representing a packet in simulation
typedef struct packet_tag {
    // not used: int dest;         // destination IP address 
    // not used: int source;       // source IP address
    int payload_size;    // packet size in bits
    int sequence_num;
    double arrival_time; // time packet is accepted into system
}packet_t;

// simulation processes events.  when an event is added to the event
// queue it must have a time >= the current simulation time.
typedef struct event_tag {
    int event_type;      // flag to determine what action is needed
    double time;         // when event occurs, must be >= current time
    packet_t *pkt_ptr;   // only used while transmission is in progress
    int transmitter;
} event_t;





//typedef packet_t ItemType;
//typedef packet_t PQItem;
/* the list ADT works on data of this type */
typedef event_t data_t;

/* commands specified to vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
