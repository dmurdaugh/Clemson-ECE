//queue.c

#include <stdio.h>
#include <stdlib.h>

#include "datatypes.h"
#include "queue.h"
struct Queue *InitializeQueue(int size)
{
	struct Queue *Q=NULL;
	int i;
	Q= (struct Queue *) malloc(sizeof(struct Queue));
	Q->Size=size;	
	Q->Count=0;
	Q->Front=0;
	Q->Rear=0;
	Q->ItemArray=(packet_t **)malloc(size*sizeof(packet_t *));
	for(i=0;i<size;i++){
		Q->ItemArray[i]=NULL;
	}
	return Q;
}
	

int QueueEmpty(Queue *Q)
{ 
	return ( Q->Count == 0 ); 
}

int QueueFull(Queue *Q)
{ 
	return ( Q->Count == Q->Size);
}

void QueueInsert(packet_t *R, Queue *Q)
{
	if (Q->Count == Q->Size) {
		//printf("Queue is full\n");
		return;
	} else {
		Q->ItemArray[Q->Rear] = R;
		Q->Rear = (Q->Rear + 1) % Q->Size;
		++(Q->Count);
	}
}

void QueueRemove(Queue *Q, packet_t **F)
{
	if (Q->Count == 0) {
		//printf("Queue is empty\n");
		return;
	} else {
		*F = Q->ItemArray[Q->Front];
		Q->Front = (Q->Front + 1) % Q->Size;
		--(Q->Count);
	}
}
