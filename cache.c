#include <stdlib.h>
#include "cache.h"

/* static implementations */

static inline char static_pop_bucket(lru_cache_t *lru, lru_entry_t *entry) {
    int index = HASH(entry->key,lru->bucket_count);
    lru_entry_t *bucket = lru->bucket[index];

    /* bucket is empty */
    if (!bucket) {
        return LRU_ERROR_NOT_FOUND;
    }
    /* entry is first in bucket */
    else if (bucket == entry) {
        lru->bucket[index] = bucket->chain;
    }
    /* searching for entry in bucket */
    else {
        while (bucket->chain) {
            if (bucket->chain == entry) {
                break;
            }
            else {
                bucket = bucket->chain;
            }
        }
        if (!bucket->chain && bucket != entry) {
            return LRU_ERROR_NOT_FOUND;
        }
        bucket->chain = entry->chain;
    }
    entry->chain = NULL;
    return LRU_SUCCESS;
}

static inline void static_pop_list(lru_cache_t *lru, lru_entry_t *entry) {
    /* entry is the only data in cache */
    if (entry == lru->head && entry == lru->tail) {
        lru->head = NULL;
        lru->tail = NULL;
    }
    /* entry is the head of the cache */
    else if (entry == lru->head) {
        lru->head = entry->next;
        lru->head->prev = NULL;
    }
    /* entry is the tail of the cache */
    else if (entry == lru->tail) {
        lru->tail = entry->prev;
        lru->tail->next = NULL;
    }
    /* entry is somewhere in the middle */
    else {
        entry->prev->next = entry->next;
        entry->next->prev = entry->prev;
    }

    entry->next = NULL;
    entry->prev = NULL;
    lru->list_count--;
    return;
}

/* low level implementations */

char lru_cache_pop(lru_cache_t *lru, lru_entry_t *entry) {
    if (!lru || !entry) {
        return LRU_ERROR_NULL;
    }

    char lru_error = static_pop_bucket(lru,entry);
    if (lru_error) {
        return lru_error;
    }

    static_pop_list(lru,entry);
    return LRU_SUCCESS;
}

char lru_cache_push(lru_cache_t *lru, lru_entry_t *entry) {
    if (!lru || !entry) {
        return LRU_ERROR_NULL;
    }
    if (lru->list_count >= lru->list_limit) {
        return LRU_ERROR_FULL;
    }

    int index = HASH(entry->key,lru->bucket_count);

    /* adding to front of bucket */
    entry->chain = lru->bucket[index];
    lru->bucket[index] = entry;

    /* adding to head of list */
    if (lru->head == NULL && lru->tail == NULL) {
        lru->head = entry;
        lru->tail = entry;
    }
    else {
        lru->head->prev = entry;
        entry->next = lru->head;
        lru->head = entry;
    }

    lru->list_count++;
    return LRU_SUCCESS;
}

/* high level implementations */

lru_cache_t *lru_cache_create(int bucket_count, int entry_max) {
    lru_cache_t *lru = calloc(1,sizeof(lru_cache_t));
    if (!lru) {
        return NULL;
    }

    lru->bucket = calloc(bucket_count,sizeof(lru_entry_t*));
    if (!lru->bucket) {
        free(lru);
        return NULL;
    }

    lru->bucket_count = bucket_count;
    lru->list_limit = entry_max;
    return lru;
}

lru_entry_t *lru_cache_search(lru_cache_t *lru, int key) {
    if (!lru) {
        return NULL;
    }

    int index = HASH(key,lru->bucket_count);
    lru_entry_t *entry = lru->bucket[index];
    while (entry) {
        if (entry->key == key) {
            lru_cache_pop(lru,entry);
            lru_cache_push(lru,entry);
            break;
        }
        else {
            entry = entry->chain; 
        }
    }
    return entry;
}

char lru_cache_add(lru_cache_t *lru, int key, int value) {
    if (!lru) {
        return LRU_ERROR_NULL;
    }
    lru_entry_t *entry = NULL;

    /* creating new space */
    if (lru->list_count < lru->list_limit) {
        entry = calloc(1,sizeof(lru_entry_t));
    }
    if (entry) {
        lru->list_count++;
    }

    /* reusing old space if new space cannot be allocated */
    else if (lru->tail) {
        entry = lru->tail;
        lru_cache_pop(lru,entry);
    }
    else {
        return LRU_ERROR_ALLOC;
    }

    /* adding to cache */
    entry->key = key;
    entry->value = value;
    lru_cache_push(lru,entry);

    return LRU_SUCCESS;
}

void lru_cache_remove(lru_cache_t *lru, int key) {
    if (!lru) {
        return;
    }

    lru_entry_t *entry = lru_cache_search(lru,key);
    if (entry) {
        lru_cache_pop(lru,entry);
        free(entry);
        lru->list_limit--;
    }
    return;
}

void lru_cache_free(lru_cache_t *lru) {
    if (!lru) {
        return;
    }

    lru_entry_t *entry, *temp;
    for (int i = 0; i < lru->bucket_count; i++) {
        entry = lru->bucket[i];
        while (entry) {
            temp = entry;
            entry = entry->chain;
            free(temp);
        }
    }
    free(lru);
    return;
}