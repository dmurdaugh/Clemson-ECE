/* MP7
 * ECE 223
 * Spring 2016
 *
 * Generate a set of documents to be processed by MP7.  This is the
 * type of testing we will do when grading this assignment.  
 *
 * This program is complete and should not need to be changed.
 * 
 * Input:
 *      Dictionary Size: max words to take from dictionary
 *      num docs       : number of test documents to create
 *      doc size       : number of words in each test document
 *      stdDev         : standard deviation for selecting words from dictionary
 *
 *      Recommendation: set stdDev in range 
 *          min: sqrt(Dictionary Size/2) for approx. Poisson r.v.
 *          max: Dictionary Size/6 for 0.9986 probability word in table
 *
 * Output:
 *    doc*:  documents with random sets of words
 *    mp7gradingscipt: test commands for MP7
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <assert.h>

const char *dictionary = "/usr/share/dict/words";
const char *testscript = "mp7gradescript";

// The word size includes the end-of-line character 
#define MIN_WORD_SIZE 5
#define MAX_WORD_SIZE 30
#define NAME_SIZE 16
#define LINE_SIZE 128
#define MAX_SECRET 11
const char *script_comm[] = {"MAX", "MIN", "DIST", "UNIQUE", "COUNT", 
                             "STATS", "QUIT"};
const char *secret_words[] = {"zero", "one", "two", "three", "four",
    "five", "six", "seven", "eight", "nine", "ten"};

// prototypes
int discrete_pseudo_gauss(int stdDev);
char *chomp(char *str);

int main(int argc, char *argv[])
{
    if(argc != 5) {
        printf("Usage: ./genDocs <Dictionary Size> <num docs> <doc size> <stdDev>\n");
        exit(1);
    }
    int max_dict_size = atoi(argv[1]);
    srand(time(NULL));

    int numDocs = atoi(argv[2]);
    int docSize = atoi(argv[3]);
    int stdDev = atoi(argv[4]);
    if (max_dict_size < 3 || max_dict_size > 10000000) {
        printf("Dictionary max_dict_size does not seem correct: %d\n", max_dict_size);
        exit(1);
    }
    if (numDocs < 1 || numDocs > 99) {
        printf("Number of documents to create out of range: %d\n", numDocs);
        exit(1);
    }
    if (docSize < 5 || docSize > max_dict_size) {
        printf("Size of a document is %d but dictionary max_dict_size is %d\n", 
                docSize, max_dict_size);
        exit(1);
    }
    if (stdDev < 5 || stdDev > max_dict_size) {
        printf("Standard deviation out of range %d\n", stdDev);
        printf("   Recommend in range %g to %d\n", 
                sqrt(max_dict_size/2), max_dict_size/6);
        exit(1);
    }

    char **dict_array = calloc(max_dict_size, sizeof(char *));
    int i, j;

    FILE *wordsfp = fopen(dictionary, "r");
    if (wordsfp == NULL) {
        printf("Failed to open dictionary at: %s\n", dictionary);
        exit(1);
    }
    FILE *scriptfp = fopen(testscript, "w");
    if (scriptfp == NULL) {
        printf("Failed to open grading script: %s\n", testscript);
        exit(1);
    }
    char word[LINE_SIZE+1];
    int word_count = 0;
    while(fgets(word, LINE_SIZE, wordsfp) != NULL && word_count < max_dict_size)
    {
        int word_len = strlen(word);

        if (word_len < MIN_WORD_SIZE || word_len > MAX_WORD_SIZE)
            continue;

        // optionally can discard proper and possesive nouns 
        if (isupper(word[0]) || word[word_len-3] == '\'')
            continue;

        dict_array[word_count] = strdup(word);
        int command = rand() % 3;
        fprintf(scriptfp, "%s %s", script_comm[command], word);

        word_count++;
    }
    for(i = 0; i < MAX_SECRET; i++) {
        fprintf(scriptfp, "%s %s\n", script_comm[2], secret_words[i]);
    }
    for(i = 0; i < numDocs; i++) {
        fprintf(scriptfp, "%s %i\n", script_comm[3], i);
        fprintf(scriptfp, "%s %i\n", script_comm[4], i);
    }
    fprintf(scriptfp, "%s\n", script_comm[5]);
    fprintf(scriptfp, "%s\n", script_comm[6]);
    fclose(wordsfp);
    fclose(scriptfp);

    printf("Read in %d words for generating documents\n", word_count);
    if (word_count < 1) {
        printf("Error, did not find any words in dictionary: %s\n", dictionary);
        exit(1);
    }
    int mean = word_count / 2;		//Center index of dict_array
    int expansion = 1;
    if (5 * stdDev < mean) {
        expansion = mean / (5 * stdDev);
        //printf("expanding word range by %d\n", expansion);
    }

    FILE *doc;
    int offset;
    char docName[NAME_SIZE+1];
    int min_pos = max_dict_size;
    int max_pos = -1;
    
    for(i = 0; i < numDocs; i++) {
        sprintf(docName, "doc%d", i);
        doc = fopen(docName, "w");
        for(j = 0; j < docSize; j++) {
            offset = expansion * discrete_pseudo_gauss(stdDev);
            if(offset != 0) {
                // if out of range then wrap
                offset = (abs(offset) % mean) * (offset/abs(offset));
            }
            int pos = mean+offset;
            assert(pos >=0 && pos < word_count);
            if (pos > max_pos) max_pos = pos;
            if (pos < min_pos) min_pos = pos;
            fprintf(doc, "%s", dict_array[pos]);
        }
        for (j = 0; j <= i && j < MAX_SECRET; j++) {
            fprintf(doc, "%s\n", secret_words[j]);
        }
        fclose(doc);
    }

    printf("Max position %d (%s), min position %d (%s)\n", max_pos, 
            chomp(dict_array[max_pos]), min_pos, chomp(dict_array[min_pos]));
    for (i = 0; i < word_count; i++)
        free(dict_array[i]);
    free(dict_array);
    return 0;
}

/* Creates a random variable with a standard normal distribution with zero 
 * mean.  The value is rounded to an integer.  Uses the Box-Muller transform 
 * to generate two values and returns one of them.  On next call the other 
 * value is returned.
 *
 * Note that with probability 0.9986 the value of the random variable 
 * has magnitute less than 3*stdDev.  The value is within 2 times the 
 * standard deviations with probability 0.9772
 *
 */
int discrete_pseudo_gauss(int stdDev)
{
    static double V1, V2, S;
    static int phase = 0;
    double X;

    if(phase == 0) {
        do {
            double U1 = (double)rand() / RAND_MAX;
            double U2 = (double)rand() / RAND_MAX;

            V1 = 2 * U1 - 1;
            V2 = 2 * U2 - 1;
            S = V1 * V1 + V2 * V2;
        } while(S >= 1 || S == 0);

        X = V1 * sqrt(-2 * log(S) / S);
    } else
        X = V2 * sqrt(-2 * log(S) / S);

    phase = 1 - phase;
    X = X*stdDev;
    //Round to nearest integer
    int A = (int)floor(X);
    double dec = X - A;
    if(dec >= .5)
    {
        A++;
    }

    return A;
}

/* If a string ends with an end-of-line \n, remove it.
 */
char *chomp(char *str) 
{
    int lastchar = strlen(str) - 1;
    if (lastchar >= 0 && str[lastchar] == '\n') {
        str[lastchar] = '\0';
    }
    return str;
}

