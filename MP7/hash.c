/* hash.c 
 * Denzel Murdaugh 
 */
 // Jenkins One At A Time Hash is used as the hash function.
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include "hash.h"

table_t *rehashTable(table_t *T);
int findfirst_prime(int number);
void insertIntoReHashTable(table_t *T,int key, char *word, int doc[]);
void insertInReHash(table_t *T,int key, char *word, int doc[]); 

table_t *tableConstruct(int size,int probe_type,int docs){
	int i,j;
	if(size<17){
		printf("Error: Table size too small");
		exit(1);
	}
	table_t *T;
	T=(table_t*)malloc(sizeof(table_t));
	T->size=size;
	T->probe_type=probe_type;
	T->num_keys=0;
	T->subHash=0;
	T->num_docs=docs;
	T->entry= (struct node_t *)malloc(sizeof(struct node_t)*size);
	for(i=0;i<size;i++){
		T->entry[i].marker=0;
		T->entry[i].word=malloc(30);
		for(j=0;j<100;j++){
			T->entry[i].doc_info[j]=0;
		}
	}
	return T;
}


void insertInHash(table_t *T,int key, char *word, int doc) {
        int hashIndex = key % T->size;
        if(hashIndex<0)
        	hashIndex=hashIndex*-1;
        if (T->size == T->num_keys) {
        		//T=rehashTable(T);
                printf("Can't perform Insertion..Hash Table is full!! need to rehash\n");
                return;
        }
        while (T->entry[hashIndex].marker == 1) {
        		if(T->entry[hashIndex].word!=NULL){
		    		if(strcmp(T->entry[hashIndex].word,word)==0){
		    			T->entry[hashIndex].doc_info[doc] += 1;
		    			return;
		    		}
        		}
                hashIndex = (hashIndex + 1)%T->size;
        }
        T->entry[hashIndex].key = key;
        T->entry[hashIndex].doc_info[doc] += 1;
        //if(T->entry[hashIndex].word!=NULL)
        strcpy(T->entry[hashIndex].word,word);
        T->entry[hashIndex].marker = 1;
        T->num_keys++;
        return;
  }

void insertInReHash(table_t *T,int key, char *word, int doc[]) {
        int hashIndex = key % T->size;
        int i;
        if(hashIndex<0)
        	hashIndex=hashIndex*-1;
        if (T->size == T->num_keys) {
        		//T=rehashTable(T);
                printf("Can't perform Insertion..Hash Table is full!! need to rehash\n");
                return;
        }
        while (T->entry[hashIndex].marker == 1) {
        	if(T->entry[hashIndex].word!=NULL){
        		if(strcmp(T->entry[hashIndex].word,word)==0){
        			for(i=0;i<T->num_docs;i++)
        			T->entry[hashIndex].doc_info[i] = doc[i];
        			return;
        		}
        	}
                hashIndex = (hashIndex + 1)%T->size;
        }
        T->entry[hashIndex].key = key;
        for(i=0;i<T->num_docs;i++)
        T->entry[hashIndex].doc_info[i] = doc[i];
        //if(T->entry[hashIndex].word!=NULL)
        strcpy(T->entry[hashIndex].word,word);
        T->entry[hashIndex].marker = 1;
        T->num_keys++;
        return;
  }
  
void insertIntoHashTable(table_t *T,int key, char *word, int doc) {
        /* 1st hashing - finding hash index */
        int hashInd = key % T->size, hashVal;
        if(hashInd<0)
        	hashInd=hashInd*-1;
        /* 2nd hashing - no of buckets to skip while probing */
        hashVal = T->subHash - (key % T->subHash);
        if (T->size == T->num_keys) {
        		T=rehashTable(T);
                printf("Can't perform Insertion..Hash Table is full!!");
                return;
        }
        /* double hashing */
        while (T->entry[hashInd].marker == 1) {
        	if(T->entry[hashInd].word!=NULL){
        		if(strcmp(T->entry[hashInd].word,word)==0){
        			T->entry[hashInd].doc_info[doc] += 1;
        			return;
        		}
        	}
                hashInd = (hashInd + hashVal)%T->size;
        }
        T->entry[hashInd].key = key;
        T->entry[hashInd].doc_info[doc]+=1;
        //if(T->entry[hashInd].word!=NULL)
       	strcpy(T->entry[hashInd].word,word);
        T->entry[hashInd].marker = 1;
        T->num_keys++;
        return;
 }
 
void insertIntoReHashTable(table_t *T,int key, char *word, int doc[]) {
        /* 1st hashing - finding hash index */
        int i;
        int hashInd = key % T->size, hashVal;
        if(hashInd<0)
        	hashInd=hashInd*-1;
        /* 2nd hashing - no of buckets to skip while probing */
        hashVal = T->subHash - (key % T->subHash);
        if (T->size == T->num_keys) {
        		T=rehashTable(T);
                printf("Can't perform Insertion..Hash Table is full!!");
                return;
        }
        /* double hashing */
        while (T->entry[hashInd].marker == 1) {
        	if(T->entry[hashInd].word!=NULL){
        		if(strcmp(T->entry[hashInd].word,word)==0){
        			for(i=0;i<T->num_docs;i++)
        			T->entry[hashInd].doc_info[i] = doc[i];
        			return;
        		}
        	}
                hashInd = (hashInd + hashVal)%T->size;
        }
        T->entry[hashInd].key = key;
        for(i=0;i<T->num_docs;i++)
        T->entry[hashInd].doc_info[i] = doc[i];
        //if(T->entry[hashInd].word!=NULL)
       	strcpy(T->entry[hashInd].word,word);
        T->entry[hashInd].marker = 1;
        T->num_keys++;
        return;
 }
int searchElement(table_t *T,int key) {
    int hashIndex = key % T->size, count = 0;
    if (T->num_keys == 0) {
            printf("Hash Table is Empty!!");
            return -1;
    }
    while (T->entry[hashIndex].marker != 0 && count <= T->size) {
            if (T->entry[hashIndex].key == key) {
            	return hashIndex;
            }
            count++;
            hashIndex = (hashIndex + 1)%T->size;
    }

    //if (!flag)
            //printf("Given data is not present in hash table\n");
    return -1;
}

int searchData(table_t *T,int key) {
    int hashInd = key % T->size, count = 0, hashVal = 0;
    hashVal = T->subHash - (key % T->subHash);
    if (T->num_keys == 0) {
            printf("Hash Table is Empty!!");
            return -1;
    }
    while (T->entry[hashInd].marker != 0 && count <= T->size) {
            if (T->entry[hashInd].key == key) {
            	return hashInd;
            }
            count++;
            hashInd = (hashInd + hashVal) % T->size;
    }

    //if (!flag)
            //printf("Given data is not present in hash table\n");
    return -1;
}


void display(table_t *T) {
    int i;
    int j;
    int modulo;
    if (T->num_keys == 0) {
            printf("Hash Table is Empty!!\n");
            return;
    }
    printf("Key      modulo     Word        index    document \n");
    printf("-----------------------------------------\n");
    for (i = 0; i < T->size; i++) {
            if (T->entry[i].marker == 1) {
            		modulo=T->entry[i].key%T->size;
            		if(modulo<0)
            			modulo=modulo*-1;
                    printf("%-13d", T->entry[i].key);
                    printf("%-7d",	modulo);
                    printf("%-15s", T->entry[i].word);
                    printf("%d\t", i);
                    for(j=0;j<T->num_docs;j++)
                    	printf("doc%d: %d ",j,T->entry[i].doc_info[j]);
                   	printf("\n");
            }
    }
    printf("\n");
    return;
}


unsigned int Hash(const char *key, int len, int size) {
    unsigned int hash, i;
    for(hash = i = 0; i < len; ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash % 100000000;
}

table_t *rehashTable(table_t *T){
	int i;
	table_t *new_table=tableConstruct(findfirst_prime(T->size*2),T->probe_type,T->num_docs);
	new_table->subHash = new_table->size / 2;
	for(i=0;i<T->size;i++){
		if(T->entry[i].marker==1){
			if(T->entry[i].word!=NULL){
			if(new_table->probe_type==LINEAR){
				insertInReHash(new_table,Hash(T->entry[i].word,strlen(T->entry[i].word),new_table->size),T->entry[i].word,T->entry[i].doc_info);
			}
			else{
				insertIntoReHashTable(new_table,Hash(T->entry[i].word,strlen(T->entry[i].word),new_table->size),T->entry[i].word,T->entry[i].doc_info);
			}
		}
		}
	}
freeHash(T);
return new_table;
}
	


int findfirst_prime(int number)
{
    int i, foundfactor;
    double upper;
    assert(number > 1);
    // if even move to next odd
    if (number % 2 == 0)
        number++;
    do {
        foundfactor = 0;      // assume number is prime
        upper = sqrt(number);
        for (i = 3; i < upper + 1; i += 2)
            if (number % i == 0) {
                foundfactor = 1;
                number += 2;  // only test odds
                break;
            }
    } while (foundfactor);
    return number;
}

void freeHash(table_t *T){
	int i;
	for(i=0;i<T->size;i++){
		free(T->entry[i].word);
	}
	free(T->entry);
	free(T);
}
