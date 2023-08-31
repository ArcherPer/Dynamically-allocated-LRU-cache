#include <stdio.h>
#include "cache.h"

void lru_cache_print_buckets(lru_cache_t *lru) {
    lru_entry_t *entry;
    for (int i = 0; i < lru->bucket_count; i++) {
        entry = lru->bucket[i];
        while (entry) {
            printf("bucket %i | key %i | value %i\n",i,entry->key,entry->value);
            entry = entry->chain;
        }
    }
    return;
}

void lru_cache_print_list(lru_cache_t *lru) {
    lru_entry_t *entry = lru->head;
    int i = 0;
    while (entry) {
        printf("list index %i | key %i | value %i\n",i,entry->key,entry->value);
        entry = entry->next;
        i++;
    }
    return;
}

int main() {
    /* test stuff here */   
    return 0;
}