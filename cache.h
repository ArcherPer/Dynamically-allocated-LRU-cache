/*|========================|*
 *|======| OVERVIEW |======|*
 *|========================|* 
 */

/*
MIT LICENSE

Copyright 2023 Archer Pergande.

Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the “Software”), to deal 
in the Software without restriction, including without limitation the rights 
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
of the Software, and to permit persons to whom the Software is furnished to 
do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all 
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR 
A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH 
THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

--------------------------------------------------------------------------------------

ABOUT

This code is an implementation of a dynamically memory allocated linked list cache using 
the least recently used algorithm. It is intended to be a proof of concept or template and not 
directly used in projects. Those who wish to use this lru cache are free to customize 
the HASH macro to their desired implementation. The "value" or "key" fields 
located in the lru_entry_t structure may be modified to reflect the data the 
programmer wishes to cache and the identifier of that data. These changes however, 
may need to be reflected in the cache.c file.

--------------------------------------------------------------------------------------

DEPENDENCIES

This code relies on the C Standard Library function "calloc" located in 
the stdlib.h header file. Some low level implementations such as "lru_cache_pop" and
"lru_cache_push" do not rely on any Standard Library functions.
*/

/*|========================|*
 *|======| HASHING |=======|*
 *|========================|* 
 */

/*
Hash calculation which determines what index a specific entry will
be stored at. This is a simple implementation and can easily be changed for
a more advanced hashing equation.
*/
#define HASH(key,size) key % size

/*|========================|*
 *|====| STATUS CODES |====|*
 *|========================|* 
 */

#define LRU_SUCCESS         0 /* The operation completed successfully. */
#define LRU_ERROR_NULL      1 /* One of the parameters were NULL. */
#define LRU_ERROR_ALLOC     2 /* Space could not be allocated for cache. */
#define LRU_ERROR_NOT_FOUND 3 /* The entry was not found in the cache. */
#define LRU_ERROR_FULL      4 /* The cache is full and cannot be added to. */

/*|========================|*
 *|=====| STRUCTURES |=====|*
 *|========================|* 
 */

/* A specific data set found in a lru cache. */
typedef struct _lru_entry_t {
    int key;                    /* Special identifier unique to this data. */
    int value;                  /* The data to be stored. */
    struct _lru_entry_t *next;  /* Pointer to the next data set. */
    struct _lru_entry_t *prev;  /* Pointer to the previous data set. */
    struct _lru_entry_t *chain; /* Pointer to the next data set whose HASH is equivalent to this one. */
} lru_entry_t;

/* A data structure for caching using the LRU method. */
typedef struct _lru_cache_t {
    lru_entry_t **bucket; /* An array of pointers to store linked lists. */
    lru_entry_t *head;    /* The most recently used data in the cache. */
    lru_entry_t *tail;    /* The least recently used data in the cache. */
    int bucket_count;     /* The size of the bucket array. */
    int list_count;       /* The current number of data sets in the cache. */
    int list_limit;       /* The maximum allowed number of data sets in the cache. */
} lru_cache_t;

/*|========================|*
 *|=====| PROTOTYPES |=====|*
 *|========================|* 
 */

/**
 * Allocates a lru cache structure.
 *
 * Parameters:
 * - bucket_count: The number of indexes in the lru cache (number of entries that will have an access time of (0)1).
 * - entry_max: The maximum number of entries allowed in the cache.
 *
 * Returns:
 * - !=NULL: A pointer to the newly allocated cache.
 * - ==NULL: Space could not be allocated for the cache.
 */
lru_cache_t *lru_cache_create(int bucket_count, int entry_max);

/**
 * Removes an entry from a lru cache but does not free the data.
 *
 * Parameters:
 * - lru - The cache to remove from.
 * - entry - The entry to remove from the lru cache.
 *
 * Returns:
 * - LRU_ERROR_NULL: One or more of the parameters are NULL.
 * - LRU_ERROR_NOT_FOUND: The entry does not exist within the cache.
 * - LRU_SUCCESS: The entry was successfully removed from the cache.
 */
char lru_cache_pop(lru_cache_t *lru, lru_entry_t *entry);

/**
 * Adds an existing entry to a lru cache.
 *
 * Parameters:
 * - lru: The cache to add to.
 * - entry: The data to add to the cache.
 *
 * Returns:
 * - LRU_ERROR_NULL: One or more of the parameters are NULL.
 * - LRU_SUCCESS: The entry was successfully added to the cache.
 */
char lru_cache_push(lru_cache_t *lru, lru_entry_t *entry);

/**
 * Searches a cache for an entry matching the specified key.
 *
 * Parameters:
 * - lru: The cache to search in.
 * - key: The value to search by.
 *
 * Returns:
 * - !=NULL: The entry was found in the cache and a pointer was returned.
 * - ==NULL: The entry was not found in the cache.
 */
lru_entry_t *lru_cache_search(lru_cache_t *lru, int key);

/**
 * Adds a new entry to a lru cache.
 *
 * Parameters:
 * - lru: The cache to add to.
 * - key: The unique identifier associated with the entry.
 * - value: The data to store.
 *
 * Returns:
 * - LRU_ERROR_NULL: One or more of the parameters are NULL.
 * - LRU_ERROR_ALLOC: Space could not be allocated for the new entry.
 * - LRU_SUCCESS: The entry was successfully added to the cache.
 */
char lru_cache_add(lru_cache_t *lru, int key, int value);

/**
 * Removes a entry from a lru cache.
 *
 * Parameters:
 * - lru: The cache to remove from.
 * - key: The unique identifier of the entry to remove.
 */
void lru_cache_remove(lru_cache_t *lru, int key);

/**
 * Removes all entries from a lru cache and deletes it.
 *
 * Parameters:
 * - lru: The cache to delete.
 */
void lru_cache_free(lru_cache_t *lru);