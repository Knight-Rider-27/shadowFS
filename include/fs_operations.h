#ifndef FS_OPERATIONS_H
#define FS_OPERATIONS_H

#define FUSE_USE_VERSION 35
#include <fuse3/fuse.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int shadow_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi);
int shadow_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi, enum fuse_readdir_flags flags);
int shadow_create(const char *path, mode_t mode, struct fuse_file_info *fi);
int shadow_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi);
int shadow_release(const char *path, struct fuse_file_info *fi);

#endif // FS_OPERATIONS_H
