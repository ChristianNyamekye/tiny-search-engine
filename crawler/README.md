# Christian Nyamekye
## CS50 Spring 2023, Lab 3

GitHub username: ChristianNyamekye

*
Crawler directory.

### crawler:
The Crawler is implemented in one file crawler.c, with four functions; `parseArgs`, `crawl`, `pageScan`, and `main`

#### main
The main function simply calls parseArgs and crawl, then exits zero.

#### parseArgs
Given arguments from the command line, `parseArgs` extracts them into the function parameters; return only if successful.
if any trouble is found, print an error to stderr and exit non-zero.

#### crawl
Does the real work of crawling from seedURL to maxDepth and saving pages in pageDirectory. 

#### pageScan
This function implements the pagescanner mentioned in the design. Given a webpage, `pageScan` scans the given page to extract any links (URLs), ignoring non-internal URLs; for any URL not already seen before (i.e., not in the hashtable), add the URL to both the hashtable pages_seen and to the bag pages_to_crawl. Pseudocode:

*
