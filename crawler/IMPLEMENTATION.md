# Lab 4

## Mark Gitau, CS50, Spring 2020

### TSE Crawler Implementation Spec

The crawler crawls a website and retrieves webpages starting with a specified URL. It parses the initial webpage, extracts any embedded href URLs and retrieves those pages, and crawls the pages found at those URLs, but limiting itself to maxDepth hops from the seed URL.

### Structs

#### Webpage

a `webpage` stores the HTML, URL, and depth of a webpage.

#### Bag

the `bag` stores `webpage` structs that are yet to be crawled

#### Hashtable

the `hashtable` stores the URLs that have been encountered

### Modules

The following modules constitute the *crawler*:

#### Pagefetcher

fetches the contents (HTML) for a page from a URL and returns.

```c
// retrieves HTML from a URL and stores it in page->html
// returns true if successful, false otherwise
bool pageFetcher(webpage_t *page);
```

#### Pagescanner

* extracts the embedded URLS in a webpage
* normalizes the URLS
* checks if they are internal
* if internal, it makes a new `struct webpage` for each URL
* inserts the URL in the hashtable of URLs seen
* inserts the `struct webpage` in the bag of webpages to crawl

```c
void pageScanner(webpage_t * page, hashtable_t *ht, bag_t *bag);
```

#### Pagesaver

contained in the *common* directory, pagesaver outputs a page to the appropriate file. It saves the file with a file name of `id`.

```c
bool pageSaver(webpage_t* page, char * dirname, const int id);
```

#### main

`main` leverages all the other modules to give the crawler its functionality.
The basic working of `main` is as follows:

Parse and validate command line arguments

Initialize the `bag` and `hashtable` structs th described above

Make a new `webpage` with the `seedURL` as the URL, depth of 0, and `NULL` html

Insert the `webpage` into the `bag` and `hashtable`

while the `bag` contains a `webpage` to be crawled

    retrieve HTML from URL and store it in `webpage->html` (`pageFetcher`)

    save the webpage to a file (`pageSaver`)

    if the depth of the current webpage is less than `maxDepth`

        extract all embedded URLs and process them (`pageScanner`)

        delete `bag` and `hashtable`
