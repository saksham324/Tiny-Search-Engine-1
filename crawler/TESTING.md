# TESTING for crawler.c

## Mark Gitau, January 2020

### Approach

All the testing is done in `testing.sh` which is called by typing `make test` on the command line while in the *crawler* directory. The test results can be found in `testing.out`.

The following wrong test cases are used to test the crawler:

* testing with the wrong number of command line arguments
* testing with an external URL
* testing with a non existent server
* testing with a non existent webpage

The following test cases are expected to pass:

* testing with a cross linked webpage - one that is a graph, with varying maxDepth
* testing the same cross linked webpage with a different seedURL
* testing a Wikipedia playground with varying maxDepth

To verify that the test cases were passed, `testing.sh` also counts the number of files produced per directory and compares them to the expected value.
