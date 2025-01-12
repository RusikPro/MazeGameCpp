cmake_minimum_required(VERSION 3.10)

project(storage)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include)

# Add the storage library
add_library(storage STATIC
    include/storage/MazeStorage.h
    src/MazeStorage.cpp
)

# Link to Maze module
target_link_libraries(storage PUBLIC maze)

# Expose include directory
target_include_directories(storage PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
