# TESTING for Querier

## Mark Gitau, April 2020

### Test Results

All the testing is done in `testing.sh` which is called by typing `make test` on the command line while in the *querier* directory. The test results can be found in `testing.out`.

The following wrong test cases are used to test the querier:

#### testing with the wrong number of command line arguments

a usage line is printed to stderr.

#### testing with an invalid pageDirectory

"Invalid pageDirectory" is printed to stderr.

#### testing with invalid queries

Error: bad character in query

Error: 'and' or 'or' cannot be first

Error: 'and' or 'or' cannot be last

Error: 'and' and 'or' cannot follow each other

#### testing with output from fuzzquery

```bash
./fuzzquery ../indexer/toscrape-depth-1-index 20 0 | ./querier ../tse-output/toscrape-depth-1 ../indexer/toscrape-depth-1-index
```

matching results are printed to stdout.
