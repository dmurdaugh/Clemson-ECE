// See ECE 223 programming guide for required comments!
//
// You must have comments at the top of the file.
//
// EVERY function must have comments

// The ONLY members you can access in a route_info_t block are 
// src_ip_addr and dest_ip_addr.  Also, routelist but in the destruct 
// function only.  All other members in this structure cannot be used 
// in this file. 

#include <stdlib.h>
#include <stdio.h>
#include "tracert.h"

//FUNCTION PROTOTYPES
int private_compare(struct route_info_t *list_rec,struct route_info_t *rec_ptr);


/*should return a pointer to the header block for the data structure. The 
data structure includes an array of pointers where the size of the array is 
equal to the value passed in to the
function
*/
struct tracert_t *tracert_construct(int size)
{
	struct tracert_t *trace_ptr;
	int i;
	trace_ptr= (struct tracert_t *) malloc(sizeof(struct tracert_t));
	trace_ptr-> trte_order=1;
	trace_ptr->trte_count=0;
	trace_ptr->trte_size=size;
	trace_ptr->trte_ptr=(struct route_info_t **) malloc(size * sizeof(struct route_info_t *));
	for(i=0;i<size;i++){
		trace_ptr->trte_ptr[i]=NULL;
	}
    return trace_ptr;
}
/*
free all the route_info_t memory blocks in the list, free the array of type
route_info_t, and free the memory block type tracert_t
*/
void tracert_destruct(struct tracert_t *ptr)
{
	int i;
	//int j;
	for (i=0;i<ptr->trte_size;i++){
		if(i<ptr->trte_count){
		free(ptr->trte_ptr[i]->routelist);
		ptr->trte_ptr[i]->routelist=NULL;
		}
		free(ptr->trte_ptr[i]);
		ptr->trte_ptr[i]=NULL;
	}
		free(ptr->trte_ptr);
		ptr->trte_ptr=NULL;
		free(ptr);
		ptr=NULL;
}



/*
should take a route_info_t memory block (that is already populated with 
input information) and insert it into the list such that the list is 
ordered and the list is sequential with no empty gaps between entries in 
the list.
return 0: replaced record in list
return 1: inserted new record into list
return -1: List is full
*/
int tracert_add(struct tracert_t *list_ptr, struct route_info_t *rec_ptr)
{
	int comp,i,j;
	i=0;
	comp=3;
	if(list_ptr->trte_count==0){//list is empty just insert block
		list_ptr->trte_ptr[i]=rec_ptr;
		list_ptr->trte_count++;
		return 1;
	}
	
	
	i=0;
	if(list_ptr->trte_size==list_ptr->trte_count){
		while(comp !=0 && i<list_ptr->trte_size){
			comp=private_compare(list_ptr->trte_ptr[i],rec_ptr);
			i++;
		}
		if(comp == 0){
		free(list_ptr->trte_ptr[i-1]->routelist);
		free(list_ptr->trte_ptr[i-1]);
		list_ptr->trte_ptr[i-1]=rec_ptr;
		return 0;
		}
		return -1; //list is full;
	}

	

	i=0;
	comp=private_compare(list_ptr->trte_ptr[i],rec_ptr);
	if(list_ptr->trte_order<0){
		comp=comp*(-1);
	}
	while(comp == 1 && i<list_ptr->trte_size){
		if(list_ptr->trte_ptr[i+1]==NULL){
			i++;
			break;
		}
		i++;
		comp=private_compare(list_ptr->trte_ptr[i],rec_ptr);
		if(list_ptr->trte_order<0){
			comp=comp*(-1);
		}
	}
	
	if(comp == 0){
		free(list_ptr->trte_ptr[i]->routelist);
		free(list_ptr->trte_ptr[i]);
		list_ptr->trte_ptr[i]=rec_ptr;
		return 0;
	}

	if(i<=list_ptr->trte_count-1){
		j=list_ptr->trte_count;
		while(j != i){
			list_ptr->trte_ptr[j]=list_ptr->trte_ptr[j-1];
			list_ptr->trte_ptr[j-1]=NULL;
			j--;
		}
	}
	list_ptr->trte_ptr[i] =rec_ptr;
	list_ptr->trte_count++;
		
    return 1;
}
//Private function used to see if the input 1 is greater than less than 
//or equal to input 2
int private_compare(struct route_info_t *list_rec,struct route_info_t *rec_ptr){
	if(rec_ptr->src_ip_addr > list_rec->src_ip_addr){
		return 1;
	}
	else if(rec_ptr->src_ip_addr == list_rec->src_ip_addr){
		if(rec_ptr->dest_ip_addr == list_rec->dest_ip_addr){
			return 0;
		}
		if(rec_ptr->dest_ip_addr > list_rec->dest_ip_addr){
			return 1;
		}
	}
	else{
		return -1;
	}
	return -3;
}
	
//Function to lookup the first node with a matching ip address to the given ip_addr
int tracert_lookup_first_src(struct tracert_t *list_ptr, int ip_addr)
{
	int i;
	for(i=0;i<list_ptr->trte_size;i++){
		if(list_ptr->trte_ptr[i] != NULL){
			if(list_ptr->trte_ptr[i]->src_ip_addr == ip_addr){
				return i;
			}
		}
	}
    return -1;
}
//Function to lookup the first node with a matching ip address and 
// destination Ip to the given src_ip and dest_ip
int tracert_lookup_src_dest(struct tracert_t *list_ptr, int src_ip, int dest_ip)
{
	int i;
	for(i=0;i<list_ptr->trte_size;i++){
		if(list_ptr->trte_ptr[i]!=NULL){
			if(list_ptr->trte_ptr[i]->src_ip_addr == src_ip){
				if(list_ptr->trte_ptr[i]->dest_ip_addr == dest_ip){
				return i;
				}
			}
		}
	}
    return -1;
}

//Given an index access the the route_info_t node so lab1.c can use it safely
struct route_info_t *tracert_access(struct tracert_t *list_ptr, int index)
{
	//printf("attempting to access index %d...\n", index);
	if(index >= 0 && index<list_ptr->trte_size){
		return list_ptr->trte_ptr[index];
	}
    return NULL;
}

//Remove the node from the list and correct the list so that there are no gaps
struct route_info_t *tracert_remove(struct tracert_t *list_ptr, int index)
{
	if(index >= list_ptr->trte_size || index < 0 || list_ptr->trte_ptr[index]==NULL){
    	return NULL;
    }

    struct route_info_t *temp=list_ptr->trte_ptr[index];
    if(index < list_ptr->trte_count-1){
    	while(index != list_ptr->trte_count-1){
    		list_ptr->trte_ptr[index]=list_ptr->trte_ptr[index+1];
    		list_ptr->trte_ptr[index+1]=NULL;
    		index++;
    		}
    		list_ptr->trte_count--;
    }
    else{
    	list_ptr->trte_ptr[index]=NULL;
    	list_ptr->trte_count--;
    }
    return temp;
}

//Give the number of entries of the list
int tracert_number_entries(struct tracert_t *list_ptr)
{
    return list_ptr->trte_count;
}

//Give the order of the list(ascending or descending)
int tracert_order(struct tracert_t *list_ptr)
{
    return list_ptr->trte_order;
}

//Reverse the order of the list
void tracert_reverse(struct tracert_t *ptr)
{
	int i=0;
	int size=ptr->trte_count-1;
	struct route_info_t *temp=NULL;
	while(i<ptr->trte_count/2){
		temp=ptr->trte_ptr[i];
		ptr->trte_ptr[i]=ptr->trte_ptr[size-i];
		ptr->trte_ptr[size-i]=temp;
		i++;
	}
	ptr->trte_order=ptr->trte_order*(-1);
}

