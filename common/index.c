#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "pagedir.h"
#include "hashtable.h"
#include "counters.h"
#include "webpage.h"
#include "memory.h"
#include "word.h"

// function prototypes
void writeIndex(FILE *indexFile, char *pageDirectory);
void writeWord(void *arg, const char *key, void *item);
void writeCount(void *arg, const int key, const int count);
void ctrsdelete(void* item);

// builds the inverted-index data structure and writes it into a file
void writeIndex(FILE *indexFile, char *pageDirectory)
{

    // inverted-index data structure
    hashtable_t *index = hashtable_new(500);

    // making a copy of pageDirectory
    char filename[11 + strlen(pageDirectory)];
    strcpy(filename, pageDirectory);
    char *ext;

    // user can give directory as either dirname/ or dirname
    if (filename[strlen(filename) - 1] != '/'){
        ext = "/";
        strcat(filename, ext);
    }

    int i = 1;   // docID

    // path name of index file
    char path[strlen(filename) + 7];  
    strcpy(path, filename);
    char n[4];
    sprintf(n, "%d", i);
    strcat(path, n);

    webpage_t *page;

    // while there are still more files to be read
    while ((page = loadPage(path)) != NULL){
        int pos = 0;
        char *result;

        // read each word of the webpage
        while ((result = webpage_getNextWord(page, &pos)) != NULL) {

            // only consider words of length 3 and up
            if (strlen(result) >= 3) {

                normalizeWord(result);

                // try to insert the word into the hashtable with a new counter
                counters_t *new = counters_new();
                if (! hashtable_insert(index, result, new)){
                    counters_delete(new);
                }

                // add the docid to the counters
                counters_t *found = hashtable_find(index, result);
                counters_add(found, i);
            }
            // free the word read
            free(result);
        }
        // increment docID    
        i++;

        // update pathname
        strcpy(path, filename);
        sprintf(n, "%d", i);
        strcat(path, n);

        // delete the webpage after reading its words
        webpage_delete(page);
    }
    hashtable_iterate(index, indexFile, writeWord);
    hashtable_delete(index, ctrsdelete);
}


// writes a line to the index file, in the specified format
void writeWord(void *arg, const char *key, void *item){
    FILE *fp = (FILE *) arg;
    counters_t *ctrs = (counters_t *) item;
    fputs(key, fp);
    fputs(" ", fp);

    // writing the docid count pairs
    counters_iterate(ctrs, fp, writeCount);
    fputc('\n', fp);
}

// writes the docID count pairs
void writeCount(void *arg, const int key, const int count) {
    FILE *fp = (FILE *) arg;

    // writing key
    char n[4];
    sprintf(n, "%d", key);
    fputs(n, fp);
    fputs(" ", fp);
    
    // writing count
    char p[4];
    sprintf(p, "%d", count);
    fputs(p, fp);
    fputs(" ", fp);
}

// delete all counters
void ctrsdelete(void* item)
{
    counters_t *ctrs = (counters_t *) item;
    if (ctrs != NULL) {
        counters_delete(ctrs);
    }
}