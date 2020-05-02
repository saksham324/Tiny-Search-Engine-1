#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "webpage.h"

// checks to see if the directory exists and can be written into
bool isValidDirectory(char * dirname);

// saves a webpage
bool pageSaver(webpage_t* page, const char * dirname, const int id);