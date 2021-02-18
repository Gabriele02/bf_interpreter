#ifndef _BRACESCACHE
#define _BRACESCACHE

#include <stdint.h>
#include "../Instructions.h"

#define MAX_CACHE_SIZE (MAX_INST_SIZE / 2)

typedef struct {
    long long *cache;
} braces_cache_t;

void        InitCache (braces_cache_t *cache);
uint8_t     cacheAdd  (braces_cache_t *cache, uint64_t position, long long item);
long long   cacheGet  (braces_cache_t *cache, uint64_t position);
void        freeCache (braces_cache_t *cache);
#endif