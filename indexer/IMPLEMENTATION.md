# Lab 5

## Mark Gitau, CS50 Spring 2020

### TSE Indexer Implementation Spec

The TSE indexer reads the document files produced by the TSE crawler, builds an index, and writes that index to a file. Its companion, the index tester, loads an index file produced by the indexer and saves it to another file.

### Structs

* *hashtable* with words as keys and a *counters* module as item - *counters* keeps track of the number of times the word has apeared in a certain document

### Modules

The following modules constitute the *indexer*:

#### writeIndex

builds the inverted data structure and writes an index file

```c
void writeIndex(FILE *indexFile, char *pageDirectory);
```

#### loadPage

reads a webpage file created by crawler and loads in t=into a `struct webpage`

```c
webpage_t *loadPage(char *pathname);
```

#### indextest

reads an index file, rebuilds the inverted data structure, and writes the index file into another file

#### main

`main` leverages all the other modules to give the indexer its functionality.
The basic working of `main` is as follows:

1. Parse and validate command line arguments,  which involves opening files that need to be opened

2. Initialize the inverted-index data structure as a `struct hashtable`

3. while there are more output files to be read,

    1. read the file and load it as a `struct webpage`

    2. while there is still a word in the webpage to be read,
        1. read the word and normalize it (convert it to lowercase)

        2. insert the word into the `struct hashtable` as a key and a new empty `struct counters`

        3. add the document id in which the word was found to the `struct counter` associated with that word

4. Iterate through the inverted-index data structure and write its contents to `indexFilename` with the format specified in `REQUIREMENTS.md`