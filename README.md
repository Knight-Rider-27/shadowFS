shadowFS

A high-performance, dual-tier user-space filesystem implemented in C using the FUSE (Filesystem in Userspace) framework. ⁠shadowFS⁠ bridges persistent disk storage (⁠shadow_storage⁠) with a high-speed LRU-cached memory/disk tier (⁠shadow_cache⁠) to optimize file access latencies.

Features

 Dual-Tier Architecture: Automatically manages file reads and writes across a persistent storage backend and a local cache layer.

 LRU Caching Mechanism: Implements Least Recently Used eviction policies to manage cache capacity efficiently.

 FUSE 3.x Integration: Standard POSIX compliance for seamless mounting, reading, writing, and directory traversal.

 Modular Codebase: Clean separation of filesystem callback hooks and core caching algorithms.

Project Structure

shadowFS/
├── CMakeLists.txt # Build configuration
├── include/ # Header files (cache.h, fs_operations.h)
├── src/ # Source implementation (main.c, cache.c, fs_operations.c)
├── shadow_storage/ # Persistent backend storage tier
└── shadow_cache/ # High-speed LRU cache tier

Building and Installation

Prerequisites

 C11 compatible compiler (GCC / Clang)

 CMake (version 3.10 or higher)

 FUSE 3 development libraries (⁠libfuse3-dev⁠)

Compilation

mkdir -p build && cd build
cmake ..
make

Usage

Mount the filesystem by specifying the binary and an empty mount point directory:

./shadowfs /home/knightrider27/shadow_mount

To cleanly unmount the filesystem:

fusermount3 -u /home/knightrider27/shadow_mount


