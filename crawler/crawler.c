#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "bag.h"
#include "pagedir.h"
#include "hashtable.h"
#include "webpage.h"
#include "memory.h"

// function prototypes
bool pageFetcher(webpage_t *page);
void pageScanner(webpage_t * page, hashtable_t *ht, bag_t *bag);
void URLDelete(void *url);


int main(const int argc, char *argv[])
{
    // command-line arguments
    int maxDepth;
    char *seedURL;
    char pageDirectory[100];
    int id = 1;

    // too few arguments given
    if (argc != 4) {
        fprintf(stderr, "usage: ./crawler seedURL pageDirectory maxDepth\n");
        return 1;
    }

    // validating maxDepth
    maxDepth = atoi(argv[3]);
    if (maxDepth < 0){
        fprintf(stderr, "maxDepth has to be > 0.\n");
        return 1;
    }

    // validating seedURL.
    if (IsInternalURL(argv[1])) {
        seedURL = assertp(count_malloc((strlen(argv[1]) + 1)*sizeof(char)), "seedURL");
        strcpy(seedURL, argv[1]);
    }
    else {
        fprintf(stderr, "Invalid seedURL.\n");
        return 1;
    }
  
    // validating directory
    strcpy(pageDirectory, argv[2]);
    if (! isValidDirectory(argv[2])) {
        return 1;
    }


    bag_t *pagesToCrawl = bag_new();  // bag of pages that need to be crawled

    hashtable_t *seen = hashtable_new(500);  // hashtable of URLs that have been seen

    webpage_t *page = webpage_new(seedURL, 0, NULL); // new webpage with seedURL as URL

    bag_insert(pagesToCrawl, page);  // insert new webpage into the bag

    hashtable_insert(seen, seedURL, ""); // insert seedURL into the hashtable


    webpage_t *webPage; // the last extracted webpage

    // while there are more webpages to be crawled
    while ((webPage = bag_extract(pagesToCrawl)) != NULL) {
        
        // retrieve HTML from URL and store it in webpage->html
        if (pageFetcher(webPage)) {

            // making a copy of the directory to avoid changing it directly
            char directory[100];
            strcpy(directory, pageDirectory);

            // saving the page to a file
            pageSaver(webPage, directory, id);

            id ++;  //increment id
        }
        // if the depth of the current webpage is less than maxDepth
        if (webpage_getDepth(webPage) < maxDepth){

            // extract all embedded URLs and process them
            pageScanner(webPage, seen, pagesToCrawl);
        }
        webpage_delete(webPage);
    }
    bag_delete(pagesToCrawl, webpage_delete);   //delete bag
    hashtable_delete(seen, URLDelete);  // delete hashtable
    return 0;  
}

// retrieves HTML from a URL and stores it in page->html
// returns true if successful, false otherwise
bool pageFetcher(webpage_t *page){
    return webpage_fetch(page);
}

// extracts all embedded URLS from page, normalizes and checks if they are internal,
// if they are, it makes webpage structs for each and inserts them into the hashtable and bag
void pageScanner(webpage_t * page, hashtable_t *ht, bag_t *bag)
{
    int pos = 0;
    char *result;

    // while there are more embedded URLS
    while ((result = webpage_getNextURL(page, &pos)) != NULL) {

        // check to see if it's internal
        if (IsInternalURL(result)){
            
            if(hashtable_insert(ht, result, "")){
                webpage_t *new = webpage_new(result, webpage_getDepth(page) + 1, NULL);
                bag_insert(bag, new);
            }
            else {
                // free the URL if it wasn't inserted in the hashtable
                free(result);
            }
        }
        else {
            // free the URL if it's not internal
            free(result);
        }
    }
}

// deletes all the items in the hashtable
void URLDelete(void *url){
    if (url != NULL){
        url = NULL;
    }
}

