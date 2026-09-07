#define FUSE_USE_VERSION 35
#include <fuse3/fuse.h>
#include "../include/cache.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <limits.h>

static pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;
static const char *root_dir = "./shadow_storage";
static void get_real_path(char *dest, const char *path) {
snprintf(dest, PATH_MAX, "%s%s", root_dir, path);
}

int shadow_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi) {
(void)fi;
if (strcmp(path, "/") != 0) {
cache_get(path); // Triggers cache check and populates on miss
}

char fpath[PATH_MAX];
snprintf(fpath, sizeof(fpath), "./shadow_cache%s", path);
struct stat st;
if (stat(fpath, &st) == -1) {
snprintf(fpath, sizeof(fpath), "./shadow_storage%s", path);
}

return (lstat(fpath, stbuf) == -1) ? -errno : 0;
}


int shadow_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
off_t offset, struct fuse_file_info *fi, enum fuse_readdir_flags flags) {
(void)offset;
(void)fi;
(void)flags;
char fpath[PATH_MAX];
get_real_path(fpath, path);

pthread_mutex_lock(&g_lock);
DIR *dp = opendir(fpath);
if (!dp) {
pthread_mutex_unlock(&g_lock);
return -errno;
}

struct dirent *de;
while ((de = readdir(dp)) != NULL) {
struct stat st;
memset(&st, 0, sizeof(st));
st.st_ino = de->d_ino;
st.st_mode = de->d_type << 12;
if (filler(buf, de->d_name, &st, 0, 0))
break;
}

closedir(dp);
pthread_mutex_unlock(&g_lock);
return 0;
}

int shadow_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
char fpath[PATH_MAX];
get_real_path(fpath, path);

pthread_mutex_lock(&g_lock);
int fd = open(fpath, fi->flags | O_CREAT | O_WRONLY, mode);
pthread_mutex_unlock(&g_lock);

if (fd == -1)
return -errno;

fi->fh = fd;
return 0;
}

int shadow_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
(void)path;
pthread_mutex_lock(&g_lock);
int res = pwrite(fi->fh, buf, size, offset);
pthread_mutex_unlock(&g_lock);

if (res == -1)
return -errno;

return res;
}

int shadow_release(const char *path, struct fuse_file_info *fi) {
(void)path;
pthread_mutex_lock(&g_lock);
int res = close(fi->fh);
pthread_mutex_unlock(&g_lock);

if (res == -1)
return -errno;

return 0;
}


