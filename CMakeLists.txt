cmake_minimum_required(VERSION 3.15)
project(shadowFS C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find libfuse3 dependency
find_package(PkgConfig REQUIRED)
pkg_check_modules(FUSE3 REQUIRED fuse3)

# Define executable from your source files
add_executable(shadowfs 
    src/main.c
    src/fs_operations.c
    src/cache.cm
)

# Link include directories and fuse libraries
target_include_directories(shadowfs PRIVATE ${FUSE3_INCLUDE_DIRS})
target_link_libraries(shadowfs PRIVATE ${FUSE3_LIBRARIES})


