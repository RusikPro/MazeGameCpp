cmake_minimum_required(VERSION 3.20)

# Project Name
project(MazeGame)

# Set C++ version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the required Qt modules
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)

# Add the project directory (modules are organized here)
add_subdirectory(project)

# Add main executable
add_executable(MazeGame
    project/main.cpp
)

# Link all module libraries and Qt libraries
target_link_libraries(MazeGame
    PRIVATE
    gui
    maze
    Qt6::Widgets
)
