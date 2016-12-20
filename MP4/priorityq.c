//priorityq.c

#include <stdio.h>
#include <stdlib.h>
#include "datatypes.h"
#include "priorityq.h"

//function Prototypes
void PriorityQueueSort(int A[]);

struct PQ *PQInitialize(int size)
{
	struct PQ * PriQ=NULL;
	int i;
	PriQ= (struct PQ *) malloc(sizeof(struct PQ));
	PriQ->ItemArray=(packet_t **) malloc(size *sizeof(packet_t *));
	PriQ->Count=0;
	PriQ->Size=size;
	for(i=0;i<size;i++){
		PriQ->ItemArray[i]=NULL;
	}
	return PriQ;
}

int PQEmpty(PQ *PriQ)
{
	return(PriQ->Count==0);
}

int PQFull(PQ *PriQ)
{
	return(PriQ->Count==PriQ->Size);
}

/*void PriorityQueueSort(int A[])*/
/*{*/
/*	int i; PQ PriQ;*/
/*	Initialize(&PriQ);*/

/*	for( i=0;i < sizeof(A)/sizeof(A[0]); i++)*/
/*		Insert(A[i],&PriQ);*/

/*	for( i=sizeof(A)/sizeof(A[0])-1; i>=0; --i)*/
/*		A[i]=remove(&PriQ);*/
/*}*/

void PQInsert(packet_t *Item, PQ *PriQ)
{
	if ( !PQFull(PriQ)){
		PriQ->ItemArray[PriQ->Count]=Item;
		PriQ->Count++;
	}
}

packet_t *PQRemove(PQ *PriQ)
{
	int i;
	int MaxIndex;
	int MinItem;
	packet_t *copy;
	if( !PQEmpty(PriQ)){
		MinItem=PriQ->ItemArray[0]->payload_size;
		MaxIndex=0;
		for( i=1; i< PriQ->Count; i++){
			if (PriQ->ItemArray[i]->payload_size < MinItem){
				MinItem=PriQ->ItemArray[i]->payload_size;
				MaxIndex=i;
			}
		}
		PriQ->Count--;
		//free(PriQ->ItemArray[MaxIndex]);
		copy=PriQ->ItemArray[MaxIndex];
		PriQ->ItemArray[MaxIndex] = PriQ->ItemArray[PriQ->Count];
		return copy;
	}
	return NULL;
}


