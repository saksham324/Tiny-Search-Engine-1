#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "pagedir.h"
#include "hashtable.h"
#include "counters.h"
#include "webpage.h"
#include "memory.h"
#include "word.h"

// builds the inverted-index data structure and writes it into a file
void writeIndex(FILE *indexFile, char *pageDirectory);

// writes a line to the index file, in the specified format
void writeWord(void *arg, const char *key, void *item);

// writes the docID count pairs
void writeCount(void *arg, const int key, const int count);
