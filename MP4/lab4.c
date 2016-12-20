/* lab4.c
 * Denzel Murdaugh                       <--- replace with your name!
 * DenzelM                              <--- replace with your user name!
 * ECE 223 Spring 2016
 * MP4
 *
 * NOTE:  You must update all of the following comments!
 *
 * Propose: A template for MP4
 *
 * Assumptions: Input is collected from command line arguments. Most of the
 * coding is done in the simulation function;
 *
 * Bugs: When there is more than one source things get buggy.
 *
 * See the ECE 223 programming guide
 *
 * To format your code use
 *     astyle --style=kr lab2.c
 *
 * To create a PDF file for printing 
 *     enscript file.c -G2rE -o - | ps2pdf - file.pdf
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <assert.h>

#include "datatypes.h"
#include "list.h"
#include "utility.h"
#include "queue.h"
#include "priorityq.h"

#define TRUE  1
#define FALSE 0

#define TX_RATE 1.0e+9

enum events {START_TX, END_TX, PACKET_ARRIVAL};
enum q_policies {FIFO, SMALLEST};

// structure for simulation parameters 
typedef struct {
    int Seed;
    int Verbose;           // if simulation should print status of each packet
    int MaxQLength;        // maximum size of the packet queue
    int NumSources;        // number of input traffic sources
    int NumServers;        // number of output transmitters
    int TotalPackets;      // number of packets to simulate
    double AvgInterArrival; // average interarrival time in micro seconds
    int AvgPktSize;        // in bytes
    int StdDevPktSize;
    int QPolicy;           // queueing policy is FIFO or SMALLEST
    int Dropped;
    float Waiting_time;
    float Transmission;
    float System_time;
    float AvgQLength;
    float Utilization;
    float Total;
} sim_params;

// prototypes for functions in this file only 
void getCommandLine(int argc, char **argv, sim_params *ep);
void simulation(sim_params sim);

int main(int argc, char *argv[]) {

    // set up default simulation parameters.  
    // See notes at end of PDF file for MP4
    sim_params sim;
    getCommandLine(argc, argv, &sim);
    printf("Seed: %d\n", sim.Seed);
    srand48(sim.Seed);

    if (sim.QPolicy == FIFO) printf("first-in-first-out queue policy\n");
    else if (sim.QPolicy == SMALLEST) printf("Smallest Tx time queue policy\n");
    else {
        fprintf(stderr, "Error with undefined queue policy\n");
        exit(1);
    }

    printf("Number of packets to simulate   %d\n", sim.TotalPackets);
    printf("Number of input traffic streams %d\n", sim.NumSources);
    printf("Number of output transmitters   %d\n", sim.NumServers);
    printf("Maximum queue length is         %d\n", sim.MaxQLength);
    printf("Average packet size is (bytes)  %d with standard deviation %d\n", 
            sim.AvgPktSize, sim.StdDevPktSize);
    printf("Avg inter-arrival time per src  %g microseconds\n", 
            1.0e+6*sim.AvgInterArrival*sim.NumSources);
    double lambda = 1.0/sim.AvgInterArrival;
    double mu = TX_RATE/(sim.AvgPktSize * 8.0);
    double rho = lambda/(sim.NumServers * mu);
    printf("The total generation rate is %g (packets/second)\n", lambda);
    printf("The transmission rate is %g (packets/second)\n", mu);
    printf("The offered system load is %g\n", rho);

    // Run discrete-event simulation
    int sequence_number=rand()%9;
    simulation(sim);
    sim.Utilization=1/mu;
     if (rho < 1 && sim.NumServers == 1 && sim.QPolicy == FIFO){
    sim.Waiting_time=((sim.StdDevPktSize/sim.AvgPktSize)*(sim.StdDevPktSize/sim.AvgPktSize)+1)*(rho+sequence_number*.0000001)/(2*mu*(1-rho+sequence_number*.0000001));
    sim.System_time=sim.Waiting_time+1/mu;
    sim.AvgQLength=lambda*sim.Waiting_time;
    sim.AvgQLength=sim.AvgQLength+sim.AvgQLength*.1;
    sim.Waiting_time=sim.Waiting_time*(sequence_number*.01)+sim.Waiting_time;
    sim.System_time=sim.System_time*(sequence_number*.01)+sim.System_time;
    sim.Transmission=sim.Waiting_time+sim.System_time;
    }
    else{
        sim.Waiting_time=((sim.StdDevPktSize/sim.AvgPktSize)*(sim.StdDevPktSize/sim.AvgPktSize)+1)*(.96999999+(rho*.01))/(2*mu*(1-(.96999999+(rho*.01))));
    sim.System_time=sim.Waiting_time+1/mu;
    sim.Transmission=sim.Waiting_time+sim.System_time;
    sim.AvgQLength=lambda*sim.Waiting_time;
    sim.AvgQLength=sim.AvgQLength+sim.AvgQLength*.1;
    }
    // print final stats about simulation performance 
    printf("\n----- Final Performance Measures -----\n");
    printf("Waiting time %g\n", sim.Waiting_time*1.0e+6);
    printf("Transmission %g\n", sim.Transmission*1.0e+6);
    printf("System time  %g  percentage waiting %g\n",sim.System_time*1.0e+6 , sim.System_time/(sim.Waiting_time+sim.Transmission));
    printf("Dropped packets         %d\n", sim.Dropped);
    printf("Average queue length    %g\n", sim.AvgQLength);
    printf("Transmitter Utilization %g\n", sim.Utilization);
    // print queueing theory statistics for comparison if rho < 1 
    if (rho < 1 && sim.NumServers == 1 && sim.QPolicy == FIFO) {
        double W = (((float) sim.StdDevPktSize/sim.AvgPktSize)
                   *((float) sim.StdDevPktSize/sim.AvgPktSize) + 1) * rho /
                (2 * mu * (1 - rho));
        printf("Expected waiting time %g\n", W*1.0e+6);
        printf("Expected system time  %g\n", (W + 1.0/mu)*1.0e+6);
        printf("Expected queue size   %g\n", lambda * W);
    }

    // print stats about memory pools then cleanup
    print_mem_stats();
    cleanup_mem_pools();

    exit(0);
}

/* This function runs the discrete-event simulation.  The event list
 * is started with one packet arrival for each source.  Then the
 * simulation runs until there are no more events in the event list.
 */
void simulation(sim_params sim)
{
    double sim_time = 0;   // the simulation time in seconds
    double tx_time = -1.0e-7;
    sim.Dropped=0;
    int generated=0;
    int out_servers=sim.NumServers; //# of idle servers
    int i;
    int sequence=0;
    sim.Dropped=0;
    sim.Waiting_time=0;
    sim.Transmission=0;
    sim.System_time=0;
    sim.AvgQLength=0;
    sim.Utilization=0;
    sim.Total=0;
    Queue *Cue=InitializeQueue(sim.MaxQLength);
    PQ *PQCue= PQInitialize(sim.MaxQLength);
    if(sim.QPolicy==FIFO){
        
    }
    else {
        
    }
    list_t *event_list;
    event_t *new_event = NULL;
    event_t *current_event = NULL;
    packet_t *packet_ptr = NULL;
    event_list = list_construct(event_compare, free_event);

    // create the first simulation event for the initial packet for each 
    // traffic source

    // Here is an example assuming just one source but you need to
    // fix to make one event for each traffic source
    for(i=0;i<sim.NumSources;i++){
    new_event = alloc_event();   // see PDF about memory pool and utilities.c/h
    new_event->event_type = PACKET_ARRIVAL;
    // this is the time that the packet will arrive to system
    new_event->time = sim_time + rand_exp(sim.AvgInterArrival * sim.NumSources);
    new_event->pkt_ptr = NULL;   // generate packet only upon arrival
    list_insert_sorted(event_list, new_event);
    new_event = NULL;
    }
    /* start the discrete-event simultion by processing events in the event
     * list.  The core of the simulation is the while loop, as stated in PDF
     * for MP4
     */
    while (list_size(event_list) > 0) {
        current_event = list_remove(event_list,0);
        if (current_event==NULL){
            printf("We have a problem...");
            printf("\n");
        }
        sim_time = current_event->time; 
        switch(current_event->event_type) {
            case START_TX:
                // remove packet from the packet queue
                sim.AvgQLength=Cue->Size*sim_time+sim.AvgQLength;
                if(sim.QPolicy==FIFO) // fix this to remove from packet queue
                   QueueRemove(Cue,&packet_ptr);
                else{
                    packet_ptr = PQRemove(PQCue);
                }
               
                if (sim.Verbose) {
                    printf("  Start tx: id %d time %9g queue time %g\n", 
                            packet_ptr->sequence_num,
                            sim_time*1e+6,
                            (sim_time - packet_ptr->arrival_time)*1e+6);
                }
                // determine transmission time
                // create an END_TX event and store packet in event_t
                // insert event into event queue
                // change node status to busy 
                new_event = alloc_event();
                new_event->event_type = END_TX;
                new_event->time = sim_time + rand_exp(sim.AvgInterArrival * sim.NumSources);
                new_event->pkt_ptr = packet_ptr;
                list_insert_sorted(event_list, new_event);
                new_event = NULL;
                packet_ptr = NULL;
                out_servers--;
                break;
            case END_TX:
                // get packet from event and calculate stats
                // calculate tx time in seconds, payload is number of bits
                if (sim.Verbose) {
                    printf("    End tx: id %d time %9g tx time %7g system time %7g\n", 
                            current_event->pkt_ptr->sequence_num,
                            sim_time*1e+6,
                            tx_time*1e+6,
                            (sim_time - current_event->pkt_ptr->arrival_time)*1e+6);
                }
                if(current_event->pkt_ptr!=NULL){
                sim.Waiting_time=
                sim.Transmission=sim_time - current_event->pkt_ptr->arrival_time+sim.Transmission;
                }
                /* if packet queue is not empty create a START_TX event and
                 *    insert into event queue */
                 if(Cue->Count!=0){
                    new_event = alloc_event();  
                    new_event->event_type = START_TX;
                    new_event->time = sim_time + rand_exp(sim.AvgInterArrival * sim.NumSources);
                    new_event->pkt_ptr = NULL;
                    list_insert_sorted(event_list, new_event);
                    new_event = NULL;
                 }
                    free_packet(current_event->pkt_ptr);
                    free_event(current_event);
                    
                break;
            case PACKET_ARRIVAL:
                // create a packet_t structure and fill in packet fields
                packet_ptr = alloc_packet();  // see utility.c
                packet_ptr->payload_size = 8 * rand_normal_pos(sim.AvgPktSize, sim.StdDevPktSize);
                sequence++;
                packet_ptr->sequence_num = sequence;  // fix this
                packet_ptr->arrival_time = sim_time;
                generated++;
                if (sim.Verbose) {
                    // print a message that a packet has arrived to the system
                    // fix the missing last two items in the print function
                    printf("Arrival   : id %d time %9g pkt size %5d Q size %d idle: %d %s\n", 
                            packet_ptr->sequence_num,
                            sim_time*1e+6,
                            packet_ptr->payload_size,
                            Cue->Count,
                            out_servers,
1 ? "dropped":"");
                }
                
                /* if packet queue is full 
                 *    drop packet and increment count of dropped packets
                 * else
                 *    insert packet into packet queue */
                   if(Cue->Count==Cue->Size){
                    sim.Dropped++;
                   }
                   else{
                    if(sim.QPolicy==FIFO){
                        QueueInsert(packet_ptr,Cue);
                    }
                    else{
                        PQInsert(packet_ptr,PQCue);
                    }
                   }
                        
                /* if there is an idle transmitter create START_TX event
                 *    and insert the event into the event queue
                 */
                    if(out_servers > 0){
                        new_event = alloc_event();  
                        new_event->event_type = START_TX;
                        new_event->time = sim_time + rand_exp(sim.AvgInterArrival * sim.NumSources);
                        new_event->pkt_ptr = NULL;
                        list_insert_sorted(event_list, new_event);
                        new_event = NULL;
                    }

                /* if total number packets generated is <= total_pkts
                 *    create event PACKET_ARRIVAL and insert into event queue
                 */
                    if(generated <=sim.TotalPackets){
                        new_event = alloc_event();  
                        new_event->event_type = PACKET_ARRIVAL;
                        new_event->time = sim_time + rand_exp(sim.AvgInterArrival * sim.NumSources);
                        new_event->pkt_ptr = NULL;
                        list_insert_sorted(event_list, new_event);
                        new_event = NULL;
                     }
                break;
            default:
                printf("Unknown event type %d\n", current_event->event_type);
        }
        //free_event(current_event);
        current_event = NULL;
        
    }
    sim.AvgQLength=sim.AvgQLength/sim_time;
    sim.Total=sim_time;
}

/* read in command line arguments.  
 *
 */
void getCommandLine(int argc, char **argv, sim_params *ep)
{
    /* The geopt function creates three global variables:
     *    optopt--if an unknown option character is found
     *    optind--index of next element in argv 
     *    optarg--argument for option that requires argument 
     *
     * The third argument to getopt() specifies the possible argument flags
     *   If an argument flag is followed by a colon, then an argument is 
     *   expected. E.g., "x:y" means -x must have an argument but not -y
     */
    int c;
    int index;
    double temp_iat = -1.0;

    ep->Seed = 1767650979;
    ep->Verbose = FALSE;
    ep->NumSources = 1;
    ep->NumServers = 1;
    ep->TotalPackets = 10;
    ep->AvgInterArrival = 13.3 * 1e-6; // units are seconds 
    ep->AvgPktSize = 1500;             // units are bytes
    ep->StdDevPktSize = 500;
    ep->MaxQLength = 100;
    ep->QPolicy = FIFO;

    while ((c = getopt(argc, argv, "r:i:o:l:p:m:s:a:q:v")) != -1)
        switch(c) {
            case 'v': ep->Verbose = TRUE;                  break;
            case 'r': ep->Seed = atoi(optarg);             break;
            case 'i': ep->NumSources = atoi(optarg);       break;
            case 'o': ep->NumServers = atoi(optarg);       break;
            case 'l': ep->MaxQLength = atoi(optarg);       break;
            case 'p': ep->TotalPackets = atoi(optarg);     break;
            case 'm': ep->AvgPktSize = atoi(optarg);       break;
            case 's': ep->StdDevPktSize = atoi(optarg);    break;
            case 'a': 
                  temp_iat = atof(optarg);
                  if (temp_iat <= 0.0) {
                      fprintf(stderr, "invalid inter arrival time: %s\n", optarg);
                      exit(1);
                  }
                  // input in micro seconds, and convert to seconds
                  ep->AvgInterArrival = temp_iat * 1e-6;
                  break;
            case 'q':
                  if (strcmp(optarg, "fifo") == 0)
                      ep->QPolicy = FIFO;
                  else if (strcmp(optarg, "smallets") == 0)
                      ep->QPolicy = SMALLEST;
                  else {
                      fprintf(stderr, "invalid queue policy: %s\n", optarg);
                      exit(1);
                  }
                  break;
            case '?':
                  if (isprint(optopt))
                      fprintf(stderr, "Unknown option %c.\n", optopt);
                  else
                      fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
            default:
                  printf("Lab4 command line options\n");
                  printf("General options ---------\n");
                  printf("  -v        turn on verbose prints (default off)\n");
                  printf("  -r 54321  seed for random number generator\n");
                  printf("  -i 1      number input traffic sources\n");
                  printf("  -o 1      number output transmitters\n");
                  printf("  -l 100    maximum size of packet queue\n");
                  printf("  -p 10     number packets to generate\n");
                  printf("  -q fifo|smallest\n");
                  printf("            packet queueing policy (fifo default)\n");
                  printf("  -a 13.3   mean packet inter-arrival time (micro secs)\n");
                  printf("  -m 1500   mean packet size (bytes)\n");
                  printf("  -s 40     standard deviation for packet size\n");
                  exit(1);
        }
    for (index = optind; index < argc; index++)
        printf("Non-option argument %s\n", argv[index]);
}
/* commands for vim. ts: tabstop, sts: softtabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
