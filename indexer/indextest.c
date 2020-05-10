#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "pagedir.h"
#include "hashtable.h"
#include "webpage.h"
#include "memory.h"
#include "index.h"
#include "file.h"

// deletes the counters
void delete(void* item);
void itemprint (FILE *fp, const char *key, void *item);

int main(const int argc, char *argv[])
{
    // validating number of arguments
    if (argc != 3){
        fprintf(stderr, "usage: /indextest oldIndexFilename newIndexFilename\n");
        return 1;
    }
    // inverted data structure
    hashtable_t *ht = hashtable_new(500);

    // opening oldIndexFilename
    FILE *fp = fopen(argv[1], "r");

    if (fp){
        char *item;
        char word[200];  // word

        while ((item = freadwordp(fp)) != NULL){

            // docID
            int i = atoi(item);

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
    // opening newIndexFilename
    FILE *f_p = fopen(argv[2], "w");

    if (f_p){
        // write to a new index file
        hashtable_iterate(ht, f_p, writeWord);
        fclose(f_p);
    }
    hashtable_delete(ht, delete);
    return 0;
}

// deletes all the counters
void delete(void* item)
{
    counters_t *ctrs = (counters_t *) item;
    if (ctrs != NULL) {
        counters_delete(ctrs);
    }
}