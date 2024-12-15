# Add the Maze module
add_library(maze STATIC
    include/maze/Maze.h
    src/Maze.cpp
)

# Link the Common library to Maze
target_link_libraries(maze PUBLIC common)

# Expose Maze include directory
target_include_directories(maze PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
