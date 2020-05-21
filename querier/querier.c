#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "ctype.h"
#include "pagedir.h"
#include "hashtable.h"
#include "counters.h"
#include "webpage.h"
#include "memory.h"
#include "word.h"
#include "index.h"
#include "dirent.h"
#include "file.h"
#include "bag.h"

int fileno(FILE *stream);
static void prompt(void);  
static hashtable_t *loadStruct(FILE * fp);                  // loads inverted-index data structure
void itemdelete(void* item);                                // deletes bag and hashtable
void intersect(void *arg, const int key, const int count);  // performs intersection of 2 counters
void unite(void *arg, const int key, const int count);      // performs union of 2 counters
void copy(void *arg, const int key, const int count);       // makes a copy of a counters-t struct
int min(int a, int b);                                      // returns minimum of 2 integers
int sum(int a, int b);                                      // returns sum of 2 integers
void count(void *arg, const int key, const int count);      // counts items in a counters_t struct
void sort(void *arg, const int key, const int count);       // inserts page_t structs into an array and sorts them

typedef struct page {   // stores docID, score pairs
    int docID;
    int score;
} page_t;

// stores an array of page_t structs and the current index
typedef struct array {
    int i;
    page_t ** pages;
} array_t;

int main(const int argc, char *argv[])
{
    // Validating number of command line arguments
    if (argc != 3){
        fprintf(stderr, "usage: ./querier pageDirectory indexFilename\n");
        return 1;
    }

    // Validating PageDirectory
    DIR* dir = opendir(argv[1]);
    char directory[5 + strlen(argv[1])]; 
    if (dir) {
        //Directory exists
        char pageDirectory[10 + strlen(argv[1])];
        strcpy(pageDirectory, argv[1]);
        strcpy(directory, argv[1]);

        // checking if .crawler file exists in the directory
        if (pageDirectory[strlen(pageDirectory) - 1] != '/'){
            strcat(pageDirectory, "/.crawler");
            strcat(directory, "/");
        }
        else { strcat(pageDirectory, ".crawler"); }

        FILE *fp = fopen(pageDirectory, "r");

        if (fp){ fclose(fp); }  //  .crawler exists

        else {
            fprintf(stderr, "Invalid pageDirectory.\n");
            return 1;
        }
        closedir(dir);
    }
    else {
        fprintf(stderr, "Invalid pageDirectory.\n");
        return 1;
    }

    // Validating indexFilename
    FILE *fp = fopen(argv[2], "r");
    if (! fp){
        fprintf(stderr, "Invalid indexFilename.\n");
        return 1;
    }
    // creating the inverted index data structure
    hashtable_t *ht = loadStruct(fp);
    if (ht == NULL){
        fprintf(stderr, "Error loading indexFile\n");
        return 2;
    }
    prompt();
    char *query;
    bool success = true;

    // keep reading queries until user quits
    while ((query = freadlinep(stdin)) != NULL) {
    
        int len = strlen(query) / 2;   // maximum possible number of words
        char *words[len];  // array of words
        int i = 0; 

        // separating words by space and normalizing them
        char* word = strtok(query, " ");
        normalizeWord(word);

        while(word != NULL){

            // check for non-alphabetic characters in each word
            for(int j = 0; j < strlen(word); j++){
                if (isalpha(word[j]) == 0 && isspace(word[j]) == 0) {
                    
                    fprintf(stderr, "Error: bad character '%c' in query\n", word[j]);
                    success = false;
                    break;  // break out of the for loop if a non-alphabetic character is found
                }
            }
            if (! success) { break; }  // break out of the while loop if a non-alphabetic character is found

            words[i] = word;  // store the word in the array of words
            word = strtok(NULL, " ");
            normalizeWord(word);
            i++;  
        }

        // start the loop again if problem encountered
        if (! success){
            count_free(query);
            success = true; 
            prompt(); 
            continue;
        }  

        int numWords = i;  // number of words in the query

        // check if the query starts with 'and' or 'or'
        if (strcmp(words[0], "and") == 0 || strcmp(words[0], "or") == 0){
            fprintf(stderr, "Error: '%s' cannot be first.\n", words[0]);
            count_free(query);
            prompt(); 
            continue;  // continue if error
        }

        // validate the rest of the query
        i = 0;
        while (i < (numWords - 1)){

            // check if 'and' and 'or' follow each other
            if ((strcmp(words[i], "and") == 0 && strcmp(words[i + 1], "or") == 0) ||
             (strcmp(words[i], "or") == 0 && strcmp(words[i + 1], "and") == 0)) {

                fprintf(stderr, "Error: '%s' and '%s' cannot follow each other\n",
                 words[i], words[i + 1]);

                success = false;
                break;  // break if error
            }

            // check if either 'and' or 'or' appear consecutively
            if ((strcmp(words[i], "and") == 0 && strcmp(words[i + 1], "and") == 0) ||
             (strcmp(words[i], "or") == 0 && strcmp(words[i + 1], "or") == 0)) {

                fprintf(stderr, "Error: consecutive '%s' not allowed\n", words[i]);
                success = false;
                break;  // break if error
            }

            // check if query ends in 'and' or 'or'
            if (i == (numWords - 2) && ((strcmp(words[i + 1], "and") == 0) || 
            (strcmp(words[i + 1], "or") == 0))){

                fprintf(stderr, "Error: query cannot end in '%s'\n", words[i + 1]);
                success = false;
                break;  // break if error
            }
            i++;  
        }

        // if any error was encountered during validation, move to next query
        if (! success) { count_free(query); prompt(); continue; }  

        // print query if it is a valid query
        else {
            fprintf(stdout, "Query: ");
            for(i = 0; i < numWords; i++){
                fprintf(stdout, "%s ", words[i]);
            }
            fprintf(stdout, "\n");
        }

        // bag of counters representing andsequences
        bag_t *andResults = bag_new();

        // running product - intersection of everything seen so far
        counters_t * resAnd = counters_new();

        // initialize the running product
        counters_iterate(hashtable_find(ht, words[0]), resAnd, copy);
        
        // process all the andsequences
        for(i = 1; i < numWords; i++){
            
            // if we encounter 'or' insert the running product into the bag of andsequences
            // re-initialize the running product to the next andsequence
            if (strcmp(words[i], "or") == 0 ){
                bag_insert(andResults, resAnd);

                resAnd = counters_new();
                counters_iterate(hashtable_find(ht, words[i + 1]), resAnd, copy);
            }

            // word is not 'or'
            else if (strcmp(words[i], "and") != 0){ 
                // make an array of pointers to counters with the running sum and the current word
                counters_t *curr = hashtable_find(ht, words[i]);
                counters_t* ptrs[2] = {resAnd, curr};

                // perform the intersection of the two
                counters_iterate(resAnd, ptrs, intersect);
            }
        }
        // insert the last andsequence into the bag
        bag_insert(andResults, resAnd);

        // process all the andsequences
        counters_t * resOr = bag_extract(andResults);  // running sum
        counters_t *curr;

        // keep extracting andsequences and performing the union of it and the running sum
        while ((curr =  bag_extract(andResults)) != NULL){
           counters_iterate(curr, resOr, unite);
           counters_delete(curr);
        }

        // count the number of documents in the running sum
        i = 0;
        counters_iterate(resOr, &i, count);

        // initialize the array of docID, score structs
        page_t *pageArray[i];

        // initialize struct containing the array initialized above and the current index (0)
        array_t * array = count_malloc(sizeof(array_t));
        array->i = 0;
        array->pages = pageArray;

        // insert structs into the array while sorting them
        counters_iterate(resOr, array, sort);
        
        // print out the results of the query
        fprintf(stdout, "Matches %d documents (ranked)\n", i);
        for(int k = 0; k < i; k++){

            char path[strlen(directory) + 5];  // directory of indexfile
            strcpy(path, directory);

            char doc[5];  // docID 
            sprintf(doc, "%d", pageArray[k]->docID);

            strcat(path, doc);  // absolute path of indexfile

            FILE *fp = fopen(path, "r");
            if (fp){
                char *url = freadlinep(fp);  // read URL from file
                fclose(fp);
                fprintf(stdout, "score%3d doc%3d: %s\n", pageArray[k]->score, pageArray[k]->docID, url);
                count_free(url);
            }
        }
        for(int k = 0; k < i; k++){ count_free(pageArray[k]); }  // free the structs

        fprintf(stdout, "-----------------------------------------------------------------------------------------------------------------------\n");
        count_free(array);
        bag_delete(andResults, itemdelete);
        counters_delete(resOr);
        count_free(query);
        prompt();
    }
    hashtable_delete(ht, itemdelete);
    return 0;
}

// print a prompt iff stdin is a tty (terminal)
static void prompt(void){
  if (isatty(fileno(stdin))) {
    printf("Query? ");
    }
}

// load the inverted index data structure 
static hashtable_t *loadStruct(FILE * fp){
    hashtable_t *ht = hashtable_new(500); 
    if (fp){
        char *item;
        char word[200];  // word

        while ((item = freadwordp(fp)) != NULL){

            int i = atoi(item); // docID

            // if atoi's return value is zero, it means we found a non-integer
            if (i == 0 && strlen(item) >= 3){

                // copy the word into word
                strcpy(word, item);

                // add a new entry to the hashtable with the word
                counters_t *new = counters_new();

                if (! hashtable_insert(ht, word, new)){
                    // free the new counter if it wasn't inserted
                    count_free(new);
                }
            }
            else if (i > 0){
                // read the next integer, which is the count
                char *count = freadwordp(fp);
                int j = atoi(count);
                count_free(count);
                // if we found an integer, add the corresponding value to the counter
                counters_set(hashtable_find(ht, word), i, j);
            }
            count_free(item);
        }
        fclose(fp);
    }
    return ht;
}

void itemdelete(void* item)
{
    counters_t * ite = (counters_t *) item;
  if (item != NULL) {
    counters_delete(ite);   
  }
}

// perform intersection of two counters
void intersect(void *arg, const int key, const int count) {
    if (arg != NULL) {

        counters_t ** ctrs = (counters_t **)arg;

        // get count of the current word
        int x = counters_get(ctrs[1], key);

        // set the count of the running product to the minimum of the two counters
        counters_set(ctrs[0], key, min(count, x));
    }
}

// perform union of two counters
void unite(void *arg, const int key, const int count) {
    if (arg != NULL){

        counters_t *other = (counters_t *) arg;

        int x = counters_get(other, key);
        // set the count of the running sum to the sum of the two counters
        counters_set(other, key, sum(count, x));
    }
}

// make a copy of a counters struct
void copy(void *arg, const int key, const int count){
    if (arg != NULL){
        counters_t *other = (counters_t *) arg;
        counters_set(other, key, count);
    }
}

// find minimum of 2 integers
int min(int a, int b) {
    if (a >= b) { return b; }
    else { return a; }
}

// find sum of 2 integers
int sum(int a, int b){
    return a + b;
}

// counts non-zero items in a counters
void count(void *arg, const int key, const int count){
    int *i = arg;
    if (i != NULL && key >= 0 && count > 0){
        (*i)++;
    }
}

// insert structs in the array while sorting them
void sort(void *arg, const int key, const int count){

    if (count > 0){
        array_t *results = (array_t *)arg;  //array of structs holding docID, score

        int idx = results->i;  // current index in the array

        // make a new struct to be inserted into the array
        page_t *page = malloc(sizeof(page_t));
        page->score = count;
        page->docID = key;

        // if it is the first item, no need to sort
        if(idx == 0){
            results->pages[0] = page;
        }

        // while there is a document with a higher score before it, 
        // keep moving the current document left
        while (idx > 0 && results->pages[idx - 1]->score < page->score){
            results->pages[idx] = results->pages[idx - 1];
            idx = idx - 1;
        }
        // insert the struct
        results->pages[idx] = page;

        // increment index
        results->i++;
    }
}