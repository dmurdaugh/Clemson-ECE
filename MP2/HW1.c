#include <stdio.h>
#include <stdlib.h>

# define ASIZE 10

void print_stats(int array[]);
int find_max(int array[],int size);
int find_count(int array[],int max);
int SumSquares(NodeType* list)

struct list_t{
	struct list_node_tag *next;
	int info;
}Nodetype;

int main(){
	int A[ASIZE]={1,2,3,4,5,6,6,4,3,2};
	listptr=(list_t)malloc(	
	print_stats(A);
	return 0;
}


void print_stats(int array[]){
	int size=ASIZE;
	int max=find_max(array,size);
	int count=find_count(array,max);
	printf("max= %d count=%d\n",max,count);
}

int find_max(int array[],int size){
	int max;
	if(size==1){
		return array[0];
	}else{
		max=find_max(array,size-1);
		if(max > array[size-1])
			return max;
		else
			return array[size-1];
	}
}

int find_count(int array[],int max){
	int counter=0;
	int i;
	for( i=0; i<ASIZE;i++)
		if(array[i]==max)
			counter++;
	return counter;
}

int SumSquares(NodeType* list)
{
 int sum = 0;
 while (list != NULL)
 {
 sum += list->info*list->info;
 list = list->next;
 }
 return sum;
}

