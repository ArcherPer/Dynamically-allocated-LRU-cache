# Dynamically-allocated-LRU-cache
Template implementation of a dynamically memory allocated linked list cache using the least recently used (LRU) algorithm.

# About

This code is intended to be a proof of concept or template and not 
directly used in projects. Those who wish to use this LRU cache are free to customize 
the HASH macro to their desired implementation. The "value" or "key" fields 
located in the lru_entry_t structure may be modified to reflect the data the 
programmer wishes to cache and the identifier of that data. These changes however, 
may need to be reflected in the cache.c file.

# Dependencies

This code relies on the C Standard Library function "calloc" located in 
the stdlib.h header file. Some low level implementations such as "lru_cache_pop" and
"lru_cache_push" do not rely on any Standard Library functions.
