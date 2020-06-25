## Mark Gitau

### TSE Querier

The TSE Querier reads the index produced by the Indexer and the page files produced by the Crawler, to interactively answer written queries entered by the user.

### Compiling

The user should type `make` on the command line, in the *querier* directory.
To test, type `make test`.

### Usage

`./querier pageDirectory indexFilename`

where:

* `pageDirectory` is the pathname of a directory produced by the Crawler
* `indexFilename` is the pathname of a file produced by the Indexer

output:

query results are printed to stdout, with each line representing one query result.

### Exit status

0 - success

1 - invalid command line arguments

2 - error loading inverted-index data structure

### Assumptions

* It is assumed that `indexFilename` is formatted correctly according to how it is specified in the *indexer* module
* It is assumed that the contents of `pageDirectory` are formatted correctly according to how it is specified in the *crawler* module

### Functionality

The TSE Querier meets full specs as specified in `REQUIREMENTS.md`.

### Special note

* The TSE Querier by Mark Gitau was implemented in exactly **420** lines of code. Some would find that cool.
