//priorityq.h
//#include "datatypes.h"

typedef struct PQ{
	int Count;
	int Size;
	packet_t **ItemArray;
} PQ;

struct PQ * PQInitialize(int size);
int PQEmpty(PQ *);
int PQFull(PQ *);
void PQInsert(packet_t *, PQ *);
packet_t *PQRemove(PQ *PriQ);
