# Lab 5

## Mark Gitau, CS50 Spring 2020

### TSE Indexer Design Spec

### User interface

The **indexer's** only interface with the user is on the command-line; it must always have two arguments.

```
./indexer pageDirectory indexFilename
```

For example:

```bash
./indexer ./indexer ../crawler/output/letters-depth-6 ./indexout
```

Note: We do not assume the pageDirectory is entered with slash at the end. It could be entered with or without it, and will be handled correctly.

The **index tester's** only interface with the user is on the command-line; it must always have two arguments.

```
./indextest oldIndexFilename newIndexFilename
```

For example:

```bash
./indextest ./indexout ./newindexout
```

### Inputs and outputs

Input: the only inputs are command-line parameters; see the User Interface above.

Output: The indexer saves an index file with a format specified in `REQUIREMENTS.md` and the index tester saves a file similar to the index file, though the order of its contents may vary.

### Functional decomposition into modules

1. *main* parses arguments and calls all the other modules
2. *writeIndex* contained in `common/index.c` builds an inverted data structure and uses it wo write the index file
3. *load page* contained in `common/pagedir.c` reads a weppage file and loads it into a `struct webpage`
4. *indextest* which reads an index file, rebuilds the inverted data structure, and writes the index file into another file

Helper modules (data structures):

* *hashtable* with words as keys and a *counters* module as item - *counters* keeps track of the number of times the word has apeared in a certain document

### Pseudo code for logic/algorithmic flow

The indexer runs as follows:

1. execute from a command line as shown in the User Interface
2. parse the command line, validate parameters
3. make a new *hashtable* to store the words and their counts in each webpage
4. read all webpage files and build the hashtable structure:
5. iterate through the structure and write the index file

### Dataflow through modules

1. *main* parses arguments and calls *writeIndex* 
2. *writeIndex* repeatedly calls *loadpage* which returns webpage structs one by one
3. *writeIndex* builds the inverted data structure using the returned webpages, and writes the index file.

### Major data structures

1. *hashtable* with words as keys and a *counters* module as item - *counters* keeps track of the number of times the word has apeared in a certain document

### Testing plan

* test the indexer with an incorrect number of command line arguments
* test the indexer with an invalid pageDirectory
* test the indexer with letters-depth-6 and wikipedia-depth-1,  directories made by the crawler.
* for each directory, run *indextest* , sort the outputs using `indexsort.awk` and compate the outputs.
