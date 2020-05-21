# Lab 6

## Mark Gitau, CS50 Spring 2020

### TSE Querier Implementation Spec

The TSE Querier reads the index produced by the Indexer and the page files produced by the Crawler, to interactively answer written queries entered by the user.

### Structs

* *hashtable* with words as keys and a *counters* module as item - *counters* keeps track of the number of times the word has apeared in a certain document
* *bag* of *counters* representing `andsequences`
* *counters* representing the final result of a query
* *page* stores docID, score pairs
* *array* stores an array of `page_t` structs and the current index in the iteration

### Modules

#### loadStruct

`loadStruct` reads an `indexFilename` and loads the inverted-index data structure

```c
static hashtable_t *loadStruct(FILE * fp);
```

#### intersect

`intersect` performs the intersection of two counters

```c
void intersect(void *arg, const int key, const int count);
```

the intersection refers to all the common elements of the two counters.

#### unite

`unite` performs the union of two counters

```c
void unite(void *arg, const int key, const int count);
```

the union refers to the total of all the elements in the two counters

#### copy

`copy` makes a copy of a `counters_t` struct. It is passed to `counters_iterate`

```c
void copy(void *arg, const int key, const int count);
```

#### count

`count` counts non-zero items in a `counters_t` struct

```c
void count(void *arg, const int key, const int count);
```

#### sort

`sort` inserts `page_t` structs in the array while sorting them using insertion sort

```c
void sort(void *arg, const int key, const int count); 
```

#### main

`main` parses and validates command-line arguments, calls other modules

The basic working of `main` is as follows:

1. Parse and validate command line arguments,  which involves opening files that need to be opened

2. Load the inverted-index datastructure by calling `loadStruct`

3. while the user as not quit,

    1. read the query, validate it and print it if valid

    2. initialize bag of `andsequences`

    3. compute each `andsequence` by berforming the intersection of counters and insert then into the bag

    4. keep extracting `andsequences` from the bag until empty and compute the running sum which is the union of all `andsequences`

    5. count the number of elements in the resulting counters struct by passing `count` to `counters_iterate`

    6. insert all the elements of the resulting counters into an array and sort them by passing `sort` into `counters_iterate`

    7. for each `docID` find its URL and print out the query results

4. manage memory
