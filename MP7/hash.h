

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum ProbeDec_t {LINEAR, DOUBLE};

typedef struct node_t {
    	int key;
        char *word;
        int doc_info[100];
        int marker;
} node;

typedef struct table_tag{
	int size;
	int probe_type;
	int num_keys;
	node *entry;
	int subHash;
	int num_docs;
} table_t;

table_t *tableConstruct(int size,int probe_type,int docs);
void insertInHash(table_t *T,int key, char *word, int doc);
void insertIntoHashTable(table_t *T,int key, char *word, int doc);
void deleteFromHash(table_t *T,int key);
void deleteFromHashTable(table_t *T,int key);
int searchElement(table_t *T,int key);
int searchData(table_t *T,int key);
unsigned int Hash(const char *key, int len, int size);
void display(table_t *T);
table_t *rehashTable(table_t *T);
int findfirst_prime(int number);
void freeHash(table_t *T);
