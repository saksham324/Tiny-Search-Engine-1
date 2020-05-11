#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "pagedir.h"
#include "hashtable.h"
#include "webpage.h"
#include "memory.h"
#include "dirent.h"
#include "index.h"

int main(const int argc, char *argv[])
{
    // Validating number of command line arguments
    if (argc != 3){
        fprintf(stderr, "usage ./indexer pageDirectory indexFilename\n");
        return 1;
    }

    // Validating PageDirectory
    DIR* dir = opendir(argv[1]);
    if (dir) {
        //Directory exists
        char pageDirectory[10 + strlen(argv[1])];

        strcpy(pageDirectory, argv[1]);

        // checking if .crawler file exists in the directory
        if (pageDirectory[strlen(pageDirectory) - 1] != '/'){
            strcat(pageDirectory, "/.crawler");
        }
        else {
            strcat(pageDirectory, ".crawler");
        }

        FILE *fp = fopen(pageDirectory, "r");
        if (fp){
            //  .crawler exists
            fclose(fp);
        }
        else {
            fprintf(stderr, "Invalid pageDirectory.\n");
            return 1;
        }
        closedir(dir);
    }
    else {
        fprintf(stderr, "Invalid pageDirectory.\n");
        return 1;
    }


    // Validating indexFilename
    FILE *fp = fopen(argv[2], "w");
    if (! fp){
        fprintf(stderr, "Invalid indexFilename.\n");
        return 1;
    }
    // write the index file
    writeIndex(fp, argv[1]);
    fclose(fp);
    return 0;
}