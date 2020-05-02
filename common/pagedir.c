#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "webpage.h"

// function prototypes
bool isValidDirectory(char * dirname);
bool pageSaver(webpage_t* page, char * dirname, const int id);


// checks if the given directory is valid by writing a file called .crawler into the directory
bool isValidDirectory(char * dirname)
{
    char * filename; // the name of the file (.crawler or ./crawler depending on the formatting)

    // user can give directory as either dirname/ or dirname
    if (dirname[strlen(dirname) - 1] != '/'){
        filename = "/.crawler";
    }
    else {
        filename = ".crawler";
    } 

    // concatenating the dirname and filename into an absolute path
    strcat(dirname, filename);

    // opening the file
    FILE * fp = fopen(dirname, "w");

    // if a valid fp is returned, the directory is valid.
    if (fp == NULL){
        fprintf(stderr, "Provide a valid Directory.\n");
        return false;
    }
    else {
        return true;
    }
}

// saves a page in the directory specified by dirname with a fie name of id
bool pageSaver(webpage_t* page, char * filename, const int id)
{
    bool status; // have we successfully saved the page?

    // if the filename does not end with a '/', concatenate it
    if (filename[strlen(filename) - 1] != '/') {
        strcat(filename, "/");
    }

    char idName[4];

    // converting int id to string  
    sprintf(idName, "%d", id);

    // concatenating filename and id into an absolute path
    strcat(filename, idName);

    FILE *fp = fopen(filename, "w");

    if (fp == NULL){
        fprintf(stderr, "pageSaver: Error saving the webpage.\n");
        status = false;
    }
    // if the file was successfully created, start writing into it
    else {
        fprintf(fp, webpage_getURL(page));
        fprintf(fp, "\n");
        fprintf(fp, "%d\n", webpage_getDepth(page));
        fprintf(fp, webpage_getHTML(page));
        status = true;
    }
    return status;
    
}