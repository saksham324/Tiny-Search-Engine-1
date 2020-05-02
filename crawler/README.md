# Lab 4

## Mark Gitau, CS50 Spring 2020

### TSE Crawler

The TSE crawler is a standalone program that crawls the web and retrieves webpages starting from a “seed” URL. It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages, recursively, but limiting its exploration to a given “depth”.

### Compiling

The user should type `make` on the command line, in the *crawler* directory.
To test, type `make test`.

### Usage

`crawler seedURL pageDirectory maxDepth`

where:
    * `seedURL` is used as the initial URL (must be an internal URL)
    * `pageDirectory` is the pathname for an existing directory in which to write downloaded webpages
    * `maxDepth` is a non-negative integer representing the maximum crawl depth

output:
    * writes each explored page to the `pageDirectory` with a unique document ID, wherein the document id starts at 1 and increments * by 1 for each new page
    * the first line of the file is the URL
    * the second line of the file is the depth
    * the rest of the file is the page content (the HTML, unchanged)

### Exit status

0 - success
1 - Invalid command line arguments

### Assumptions

It is assumed that the `common.a` archive exists in the common directory.
It is assumed that the directory in which to store output files has already been created.

### Limitations

maxDepth has to be a relatively small number, especially when crawling sites with many embedded URLs. This is because there is a pause between page fetches, and a large maxDepth would take a long time.
