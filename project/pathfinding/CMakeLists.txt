# Add the PathFinding module
add_library(pathfinding STATIC
    include/pathfinding/PathFinder.h

    src/Frontier.h
    src/Frontier.cpp

    src/PathFinder.cpp
)

# Link the Common library to Maze
target_link_libraries(pathfinding PUBLIC maze common)

# Expose Maze include directory
target_include_directories(pathfinding PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
