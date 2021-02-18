#include <stdint.h>
#include <stdlib.h>
#include "BracesCache.h"

void InitCache(braces_cache_t *cache){
    long long i;
    for(i = 0; i < MAX_CACHE_SIZE; i++){
        cache->cache[i] = -1;
    }
}

uint8_t cacheAdd(braces_cache_t *cache, uint64_t position, long long item){
    if(position >= MAX_CACHE_SIZE){return 0;}

    cache->cache[position] = item;
    return 1;
}

long long cacheGet(braces_cache_t *cache, uint64_t position){
    if(position >= MAX_CACHE_SIZE){return -1;}

    return cache->cache[position];
}

void freeCache(braces_cache_t *cache){
    free(cache->cache);
}