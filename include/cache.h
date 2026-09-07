#ifndef CACHE_H
#define CACHE_H

#define MAX_CACHE_FILES 10

void cache_init(const char *c_dir, const char *r_dir);
int cache_get(const char *path);
void cache_evict_lru();

#endif // CACHE_H

