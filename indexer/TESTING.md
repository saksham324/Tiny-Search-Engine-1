# TESTING for indexer

## Mark Gitau, April 2020

### Test Results

All the testing is done in `testing.sh` which is called by typing `make test` on the command line while in the *indexer* directory. The test results can be found in `testing.out`.

The following wrong test cases are used to test the indexer:

#### testing with the wrong number of command line arguments

a usage line is printed to stderr.

#### testing with an invalid pageDirectory

"Invalid pageDirectory" is printed to stderr.

The following test cases are expected to pass:

#### testing with letters-depth-6 from crawler output

The output is generated and compared to the output of `indextest`. If there are no differences, the line "no differences" is printed to stdout and all the output files cleaned up.

#### testing with wikipedia-depth-1 from crawler output

The output is generated and compared to the output of `indextest`. If there are no differences, the line "no differences" is printed to stdout and all the output files cleaned up.
