
#define FUSE_USE_VERSION 35
#include <fuse3/fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../include/fs_operations.h"

static const struct fuse_operations shadow_oper = {
.getattr = shadow_getattr,
.readdir = shadow_readdir,
.create = shadow_create,
.write = shadow_write,
.release = shadow_release,
};

int main(int argc, char *argv[]) {
system("mkdir -p ./shadow_storage");
return fuse_main(argc, argv, &shadow_oper, NULL);
}


