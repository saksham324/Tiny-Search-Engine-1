#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "webpage.h"
#include "memory.h"

// function prototypes
bool isValidDirectory(char * dirname);
bool pageSaver(webpage_t* page, char * dirname, const int id);


// checks if the given directory is valid by writing a file called .crawler into the directory
bool isValidDirectory(char * dirname)
{
    // the name of the file (.crawler or ./crawler depending on the formatting)
    char filename[11 + strlen(dirname)];
    strcpy(filename, dirname);
    char *ext;
    
    // user can give directory as either dirname/ or dirname
    if (dirname[strlen(dirname) - 1] != '/'){
        ext = "/.crawler";
        strcat(filename, ext);
    }
    else {
        ext = ".crawler";
        strcat(filename, ext);
    } 

    // opening the file
    FILE * fp = fopen(filename, "w");

    // if a valid fp is returned, the directory is valid.
    if (fp == NULL){
        fprintf(stderr, "Provide an existing pageDirectory.\n");
        return false;
    }
    else {
        fclose(fp);
        return true;
    }
}

// saves a page in the directory specified by dirname with a file name of id
bool pageSaver(webpage_t* page, char * dirname, const int id)
{
    bool status; // have we successfully saved the page?
    char filename[strlen(dirname) + 5];
    strcpy(filename, dirname);
    // if the filename does not end with a '/', concatenate it
    if (dirname[strlen(dirname) - 1] != '/') {
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
        fclose(fp);
        status = true;
    }
    return status;   
}