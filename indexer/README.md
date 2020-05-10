# Lab 5

## Mark Gitau, CS50 Spring 2020

### TSE Indexer

The TSE indexer is a standalone program that reads the document files produced by the TSE crawler, builds an index, and writes that index to a file. Its companion, the *index tester*, loads an index file produced by the indexer and saves it to another file.

### Compiling

The user should type `make` on the command line, in the *indexer* directory.
To test, type `make test`.

### Usage

#### indexer

`./indexer pageDirectory indexFilename`

where:

* `pageDirectory` is the pathname of a directory produced by the Crawler
* `indexFilename` is the pathname of a file into which the index should be written

output:

a file named `indexFilename` that contains contents whose format is specified in `REQUIREMENTS.md`.

#### index tester

`./indextest oldIndexFilename newIndexFilename`

where:

* `oldIndexFilename` is the name of a file produced by the indexer
* `newIndexFilename` is the name of a file into which the index should be written

output:

a file named `newIndexFilename` which has the same contents as `oldIndexFilename`, though order of the contents may vary.

### Exit status

0 - success
1 - invalid command line arguments

### Assumptions

It is assumed that the `common.a` archive exists in the common directory.
It is assumed that the crawler module works correctly, and that its output is correctly formatted.

### Limitations

The indexer only considers words whose length is at least three characters.
