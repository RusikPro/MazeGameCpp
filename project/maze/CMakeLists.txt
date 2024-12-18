# Add the Maze module
add_library(maze STATIC
    include/maze/IMaze.h
    include/maze/IMazeFactory.h

    src/MazeFactory.h
    src/BaseMaze.h
    src/KruskalMaze.h
    src/EllerMaze.h

    src/MazeFactory.cpp
    src/BaseMaze.cpp
    src/EllerMaze.cpp
    src/KruskalMaze.cpp
)

# Link the Common library to Maze
target_link_libraries(maze PUBLIC common)

# Expose Maze include directory
target_include_directories(maze PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
