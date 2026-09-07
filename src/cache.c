#define FUSE_USE_VERSION 35
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>
#include <time.h>
#include "../include/cache.h"

static char cache_path[PATH_MAX];
static char root_path[PATH_MAX];

typedef struct CacheNode {
char path[PATH_MAX];
time_t last_accessed;
struct CacheNode *next;
} CacheNode;

static CacheNode *head = NULL;
static int current_cache_size = 0;

void cache_init(const char *c_dir, const char *r_dir) {
snprintf(cache_path, sizeof(cache_path), "%s", c_dir);
snprintf(root_path, sizeof(root_path), "%s", r_dir);
system("mkdir -p ./shadow_cache");
}

void cache_evict_lru() {
if (!head) return;

CacheNode *prev = NULL;
CacheNode *curr = head;
CacheNode *lru_node = head;
CacheNode *lru_prev = NULL;

while (curr != NULL) {
if (curr->last_accessed < lru_node->last_accessed) {
lru_node = curr;
lru_prev = prev;
}
prev = curr;
curr = curr->next;
}

// Remove file from the cache directory
char target[PATH_MAX * 2];
snprintf(target, sizeof(target), "%s%s", cache_path, lru_node->path);
unlink(target);

// Remove node from the linked list
if (lru_prev == NULL) {
head = lru_node->next;
} else {
lru_prev->next = lru_node->next;
}

free(lru_node);
current_cache_size--;
}

int cache_get(const char *path) {
char cfile[PATH_MAX], rfile[PATH_MAX];
snprintf(cfile, sizeof(cfile), "%s%s", cache_path, path);
snprintf(rfile, sizeof(rfile), "%s%s", root_path, path);

time_t now = time(NULL);

// Check if file is already in cache (Cache Hit)
CacheNode *curr = head;
while (curr != NULL) {
if (strcmp(curr->path, path) == 0) {
curr->last_accessed = now; // Update access timestamp
return 1;
}
curr = curr->next;
}

// Cache Miss: Check capacity and evict LRU if full
if (current_cache_size >= MAX_CACHE_FILES) {
cache_evict_lru();
}

// Copy file from root storage to cache directory
char cmd[PATH_MAX * 3];
snprintf(cmd, sizeof(cmd), "cp %s %s", rfile, cfile);
if (system(cmd) != 0) return 0;

// Add new node to the front of tracking list
CacheNode *new_node = malloc(sizeof(CacheNode));
snprintf(new_node->path, sizeof(new_node->path), "%s", path);
new_node->last_accessed = now;
new_node->next = head;
head = new_node;
current_cache_size++;

return 1;
}
