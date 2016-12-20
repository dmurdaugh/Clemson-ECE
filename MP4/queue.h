//Queue.h
#define MaxSize 100;

typedef struct Queue{
	int Count;
	int Front;
	int Rear;
	int Size;
	packet_t **ItemArray;
}Queue;

struct Queue *InitializeQueue(int size);
int QueueEmpty(Queue *Q);
int QueueFull(Queue *Q);
void QueueInsert(packet_t *R, Queue *Q);
void QueueRemove(Queue *Q,packet_t **F);

