# Lab 6

## Mark Gitau, CS50 Spring 2020

### TSE Querier Design Spec

### User interface

The **querier's** only interface with the user is on the command-line; it must always have two arguments.

```
./querier pageDirectory indexFilename
```

For example:

```bash
./querier ../tse-output/toscrape-depth-1 ../indexer/toscrape-depth-1-index
```

Note: We do not assume the pageDirectory is entered with slash at the end. It could be entered with or without it, and will be handled correctly.

### Inputs and outputs

Input: the only inputs are command-line parameters; see the User Interface above.

Output: The querier prints out query results on stdout with each line representing one result.

### Functional decomposition into modules

1. *main* parses and validates command-line arguments, calls other modules 
2. *loadStruct* reads an `indexFilename` and loads the inverted-index data structure
3. *intersect* performs the intersection of two counters
4. *unite* perform union of two counters
5. *copy* makes a copy of a `counters_t` struct
6. *count* counts non-zero items in a `counters_t` struct
7. *sort* inserts `page_t` structs in the array while sorting them

### Pseudo code for logic/algorithmic flow

The querier runs as follows:

1. execute from a command line as shown in the User Interface
2. parse the command line, validate parameters
3. load the inverted index data structure from `indexFilename`
4. read and validate query
5. process query and display results

### Major data structures

* *hashtable* with words as keys and a *counters* module as item - *counters* keeps track of the number of times the word has apeared in a certain document
* *bag* of *counters* representing `andsequences`
* *counters* representing the final result of a query
* *page* stores docID, score pairs
* *array* stores an array of `page_t` structs and the current index in the iteration

### Testing plan

* test the querier with an incorrect number of command line arguments
* test the querier with an invalid pageDirectory
* test the querier with invalid queries
* test the querier with output from `fuzztest.c`
