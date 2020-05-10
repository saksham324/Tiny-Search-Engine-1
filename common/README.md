# Lab 4

## Mark Gitau, CS50 Spring 2020

### TSE Common

The Common directory contains pagedir.c and pagedir.h, whose job is to save webpages.

### Compiling

The user should type `make` on the command line, in the *common* directory.

### Usage

Running the `make` command creates common.a. The functions in pagedir.h can be used by any other modules.

```c
// checks to see if the directory exists and can be written into
bool isValidDirectory(char * dirname);

// saves a webpage
bool pageSaver(webpage_t* page, const char * dirname, const int id);

// loads a file creted by crawler into a webpage struct
webpage_t *loadPage(char *pathname);
```

### Assumptions

No assumptions beyond what is stated in the specifications.
