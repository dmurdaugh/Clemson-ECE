/* lab6.c 
 * Denzel Murdaugh 
 * DenzelM
 *
 * Lab6: Word Ladder and shortest path
 * ECE 223, Spring 2016
 *
 * This file contains a template.  Many details are missing.
 *
 * The code correctly opens and reads a dictionary file and stores the file
 * into an array called wordlist.  
 *
 * You must add the details to build a graph and run the shortest path
 * algorithm.
 *
 * You MUST use a module for your graph algorithms.
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "graph.h"

enum {FALSE, TRUE};
#define MAXLINE 128

// Global variables for command line options

char *FileName = "/usr/share/dict/words";
int WeightFun = 1;
int GraphOp = 1;
int WordLength = 6;
int MaxWords = INT_MAX;
char *Source = "";
char *Dest = "";
int Verbose = FALSE;

void getCommandLine(int argc, char **argv);

/* Returns a count of the number of characters that are different
 * between the two words.  It assumes the two words have the same
 * length and it is given by len.
 */
int char_diff(char *a, char *b, int len) 
{
    int i, diffs = 0;
    for (i = 0; i < len; i++) {
        if (a[i] != b[i])
            ++diffs;
    }
    return diffs;
}

/* Returns one if the two words are different in exactly one location.
 * Otherwise, zero is returned.  
 * It assumes the two words have the same length given by len.
 */
int one_char_diff(char *a, char *b, int len) 
{
    int i, diffs = 0;
    for (i = 0; i < len; i++) {
        if (a[i] != b[i])
            if (++diffs > 1)
                return 0;
    }
    return diffs == 1;
}

/* Calculates a hash value for an arbitrary block of memory.  key
 * is a pointer to the starting address, and len is the number of
 * bytes in the memory block.
 */
unsigned hash(void *key, int len)
{
    unsigned char *p = key;
    unsigned h = 0;
    int i;
    for (i = 0; i < len; i++) {
        h = 33 * h + p[i];
    }
    return h;
}

/* A non-linear and asymmetric function that returns a positive
 * integer.
 */
int mystery(int a, int b)
{
    return abs(a-b) + (a-b+3)*(a-b+3) + 3*b;
}


int main(int argc, char *argv[])
{
    FILE *fpt;
    char line[MAXLINE];
    char wordin[MAXLINE];
    char **wordlist;      // list of words to use for graph
    int num_words = 0;    // number of words in the wordlist
    int source = -1;      // position in the list of the source word
    int dest = -1;         //          and destination word
    clock_t start, end;
	int pock=0;
	int info[4];
    getCommandLine(argc, argv);
    
    // open file 
    fpt=fopen(FileName,"r");
    if (fpt == NULL) {
        printf("Unable to open %s for reading\n",FileName);
        exit(0);
    }
    printf("Dictionary file: %s\n", FileName);
    if (strcmp(Source, "") != 0) {
        WordLength = strlen(Source);
    }
    printf("Selecting words with length %d\n", WordLength);
    if (MaxWords < INT_MAX)
        printf("  Accepting at most %d words for graph\n", MaxWords);

    if (WeightFun == 1) {
        printf("Weight function 1: word ladder\n");
    } else if (WeightFun == 2) {
        printf("Weight function 2: up to half letters can be different\n");
    } else if (WeightFun == 3) {
        printf("Weight function 3: hash weights for sparse graph\n");
    } else if (WeightFun == 4) {
        printf("Weight function 4: hash weights for dense graph\n");
    } else {
        printf("invalid weight funtion%d\n", WeightFun);
        exit(1);
    }

    int wordlistsize = 16;
    wordlist = (char **) malloc(wordlistsize * sizeof(char *));
    // read in words one at a time, find word length, count it
    //
    // Warning: this assumes that the input file of words has one word 
    //          per line and that there are no duplicate words.  If this 
    //          is not true strange things might happen...
    // 
    // This does not require that the list of words be sorted.
    //
    start = clock();
    while (fgets(line, MAXLINE, fpt) != NULL) {
        int num_items = sscanf(line, "%s", wordin);
        if (num_items != 1)   // failed to read first word on line
            continue;
        int word_len = strlen(wordin);

        if (word_len != WordLength)
            continue;

        // optionally can discard proper and possesive nouns 
        if (isupper(wordin[0]) || wordin[word_len-2] == '\'')
            continue;

        // check if the list is full, and if so double the list
        if (num_words == wordlistsize) {
            wordlistsize *= 2;
            wordlist = realloc(wordlist, wordlistsize*sizeof(char *));
        }
        wordlist[num_words] = strdup(wordin);

        // save location of source and destination words, if given
        if (strcmp(wordin, Source) == 0)
            source = num_words;
        if (strcmp(wordin, Dest) == 0)
            dest = num_words;

        num_words++;
        if (num_words == MaxWords)
            break;
    }
    end = clock();
    // close word file
    fclose(fpt);
    if (num_words == 0) {
        printf("Failed to find any words!\n");
        exit(1);
    }
    printf("Stored %d words in %g msecs\n", num_words,
            1000.0*(end-start)/CLOCKS_PER_SEC);

    if (strcmp(Source, "") != 0) {
        if (source < 0 || source >= num_words) {
            printf("Failed to find source word \"%s\" in the list\n", Source);
            exit(1);
        }
        printf("Source: %s at %d\n", Source, source);
    }
    if (strcmp(Dest, "") != 0) {
        if (dest < 0 || dest >= num_words) {
            printf("Failed to find dest word \"%s\" in the list\n", Dest);
            exit(1);
        }
        printf("Dest: %s at %d\n", Dest, dest);
    }

	graph_t *G =graph_construct(num_words);
	//printf("creating graph\n");
    // find the edges and add to graph
    int i, j;
    int num_edges = 0;
    int weight_i_to_j = 0;
    int weight_j_to_i = 0;
    start = clock();
    for (i = 0; i < num_words; i++) {
        for (j = i + 1; j < num_words; j++) {

            // Determine if there is and edge and its weight
            if (WeightFun == 1) {
                weight_i_to_j = one_char_diff(wordlist[i], wordlist[j], WordLength);
                weight_j_to_i = weight_i_to_j;
            } else if (WeightFun == 2) {
                weight_i_to_j = char_diff(wordlist[i], wordlist[j], WordLength);
                if (weight_i_to_j > WordLength/2)
                    weight_i_to_j = 0;
                weight_j_to_i = weight_i_to_j;
            } else if (WeightFun == 3 || WeightFun == 4) {
                int worda_hash = hash(wordlist[i], WordLength) % num_words;
                int wordb_hash = hash(wordlist[j], WordLength) % num_words;
                weight_i_to_j = mystery(worda_hash, wordb_hash);
                weight_j_to_i = mystery(wordb_hash, worda_hash);
                if (WeightFun == 3 && weight_i_to_j > 5*num_words)
                    weight_i_to_j = 0;
                if (WeightFun == 3 && weight_j_to_i > 5*num_words)
                    weight_j_to_i = 0;
            } else {
                printf("invalid weight type %d\n", WeightFun);
                exit(1);
            }

            // Install the edges in the graph if the weight is positive
            if (weight_i_to_j != 0) {
                if (Verbose && weight_j_to_i == weight_i_to_j)
                    printf("edge %d-%s <---> %d-%s %d\n", i, wordlist[i], j, wordlist[j], weight_i_to_j);
                else if (Verbose) 
                    printf("edge %d-%s ---> %d-%s %d\n", i, wordlist[i], j , wordlist[j], weight_i_to_j);

                // You must add a function to add the edge to the graph
                graph_add_edge(G, i, weight_i_to_j, j);
                num_edges += 1;
            }
            if (weight_j_to_i != 0) {
                if (Verbose && weight_j_to_i != weight_i_to_j)
                    printf("edge %d-%s ---> %d-%s %d\n", j, wordlist[j], i , wordlist[i], weight_j_to_i);

                // You must add a function to add the edge to the graph
                graph_add_edge(G, i, weight_i_to_j, j);
                num_edges += 1;
            }
        }
    }
    end = clock();
    printf("Added %d edges in %g msecs\n", num_edges,
            1000.0*(end-start)/CLOCKS_PER_SEC);
    int max_edges = num_words * (num_words - 1);
    printf("    %g edge density percentage\n", 100.0 * num_edges/ max_edges);

    // average, maximum, and minmum number of neighbors
    int u, tot_nbrs = 0;
    int min_nbrs = num_words;
    int max_nbrs = -99, max_word = 0;
    int nonbrs = 0;
    for (u = 0; u < num_words; u++) {
        int nbrs = graph_num_nbrs(G,u); 
        // you must add a function to return the number of neighbors of vertex u
        tot_nbrs += nbrs;
        if (nbrs < min_nbrs)
            min_nbrs = nbrs;
        if (nbrs > max_nbrs) {
            max_nbrs = nbrs;
            max_word = u;     // location of word with most neighbors
        }
        if (nbrs == 0) {
            //printf("%d %s no nbrs\n", u, wordlist[u]);
            nonbrs++;
        }
    }
    printf("Avg num nbrs: %g", ((float) tot_nbrs)/num_words);
    printf("   min: %d, max: %d\n", min_nbrs, max_nbrs);
    if (nonbrs > 0) {
        printf("    %g percent of the nodes have no nbrs\n", 
                100.0*nonbrs/num_words);
    }

    // Neighbors of a word, or word with most neighbors
    if (GraphOp == 1) {

        // If source was given find its neighbors and print all of them
        if (source != -1) {
        	int neigh=graph_num_nbrs(G,source);
            printf("word %s has %d neighbors\n", wordlist[source], neigh);
            for(i=0; i< G->num; i++){
            		if(G->adj_list[source][i]!=INT_MAX && G->adj_list[source][i]!=0){
            			printf("%s\n",wordlist[i]);
            }
           }
        } else {
            // Found max_word above.  print its neighbors
            printf("word %s with max number neighbors = %d\n", 
                    wordlist[max_word], max_nbrs);
                for(i=0; i< G->num; i++){
            		if(G->adj_list[max_word][i]!=INT_MAX && G->adj_list[max_word][i]!=0){
            			printf("%s\n",wordlist[i]);
            	}
            }
        }
    // Shortest path from a given source
    } else if (GraphOp == 2) {
        if (source < 0 || source >= num_words) {
            printf("Required to give source word for -g 2\n");
            exit(1);
        }
        printf("Start shortest path algorithm\n");
        start = clock();

        // You must call the shortest path algorithm here
		dist_t *ShortestDist=graph_shortest_path(G,source,dest);
        end = clock();

        if(ShortestDist==NULL){
        	printf("ERROR \n ERROR\n ERROR\n");
        	exit(1);
        }
        printf("Ran shortest path algorithm in %g msecs\n", 
                1000.0*(end-start)/CLOCKS_PER_SEC);

        // If a destination word is given, use these prints
        if (dest != -1) {
            // if a path to the destination is found print 
            if(ShortestDist[dest].weight!=INT_MAX){
		        printf("  Cost is %d for path from %s to %s\n", 
		                ShortestDist[dest].weight, wordlist[source], wordlist[dest]);
		        // Next print the words in the path one per line
		       
				list_node_t *rover= ShortestDist[dest].path->head;
				printf("List: %s\n",wordlist[source]);
				while(rover != NULL){
					
					pock=*(rover->data_ptr);
					printf(" -> %s\n",wordlist[pock]);
					rover = rover->next;
				}
				printf("\n");
			}
			else{
            // However, if no path was found then print
            printf("  No path from %s to %s\n", wordlist[source], wordlist[dest]);
            }
        // No destination given so find longest shortest path from source
        } else {
            // If there is some path from the source print it
            if(ShortestDist[source].weight > 0){
		        dest=ShortestDist[source].final;
		        printf("  Longest shortest-path from %s to %s cost %d\n", 
		                wordlist[source], wordlist[dest], ShortestDist[source].weight);
		        // Next print the words in the path one per line
		        int mage=ShortestDist[source].final;
		        list_node_t *rover= ShortestDist[mage].path->head;
				printf("List: %s\n",wordlist[source]);
				rover=rover->next;
				while(rover != NULL){
					pock=*(rover->data_ptr);
					printf(" -> %s\n",wordlist[pock]);
					rover = rover->next;
				}
				printf("\n");
			}
			else{
            // However, if no path is found that means source has no nbrs
            printf("%s has no neighbors\n", wordlist[source]);
            }
        }
        graph_free_dij(ShortestDist,G->num);
    	free(ShortestDist);
    } else if (GraphOp == 3) {
        printf("Start diameter algorithm\n");
        start = clock();

        // Insert your diameter algorithm here
        	graph_diameter(G,info);

        end = clock();
        printf("Ran diameter algorithm in %g msecs\n", 
                1000.0*(end-start)/CLOCKS_PER_SEC);

        // In the very strange case there are no edges in the graph print
        if(info[0]==0){
        printf(" A graph with no edges!\n");
		}
		else{
        // Otherwise there is some longest path
        	printf("Longest path is %d from (%s) to (%s) \n", info[0], wordlist[info[1]], wordlist[info[2]]);
        
        // Next print the words in the path one per line
		    dist_t *ShortestDist=graph_shortest_path(G,info[1],info[2]);
		    list_node_t *rover= ShortestDist[info[2]].path->head;
			printf("List: %s\n",wordlist[info[1]]);
			while(rover != NULL){
				if(rover->data_ptr!=NULL){
				pock=*(rover->data_ptr);
				printf("%s\n",wordlist[pock]);
				rover = rover->next;
				}
			}
			printf("\n");
			graph_free_dij(ShortestDist,G->num);
			free(ShortestDist);
		}
		
    } else {
        printf("invalid operation %d\n", GraphOp);
        exit(1);
    }

    // End of code so clean up
    for (i = 0; i < num_words; i++)
        free(wordlist[i]);
    free(wordlist);
    graph_destruct(&G);

    exit(0);
}

/* read in command line arguments and store in global variables for easy
 * access by other functions.
 */
void getCommandLine(int argc, char **argv)
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

    while ((c = getopt(argc, argv, "f:w:m:l:g:s:d:v")) != -1)
        switch(c) {
            case 'f': FileName = optarg;           break;
            case 'w': WeightFun = atoi(optarg);    break;
            case 'm': MaxWords = atoi(optarg);     break;
            case 'l': WordLength = atof(optarg);   break;
            case 'g': GraphOp = atoi(optarg);      break;
            case 's': Source = optarg;             break;
            case 'd': Dest = optarg;               break;
            case 'v': Verbose = TRUE;              break;
            case '?':
                  if (isprint(optopt))
                      fprintf(stderr, "Unknown option %c.\n", optopt);
                  else
                      fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
            default:
                  printf("Lab6 command line options\n");
                  printf("General options ---------\n");
                  printf("  -v         turn on verbose prints (default off)\n");
                  printf("  -f name    filename of dictionary\n");
                  printf("  -w 1|2|3|4 weight function for edges\n");
                  printf("  -m 7       max number of words in graph\n");
                  printf("  -l 6       length of words in graph\n");
                  printf("  -g 1|2|3   graph operation\n");
                  printf("  -s fire    source word\n");
                  printf("  -d life    destination word\n");
                  exit(1);
        }
    for (index = optind; index < argc; index++)
        printf("Non-option argument %s\n", argv[index]);
}
