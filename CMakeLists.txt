cmake_minimum_required(VERSION 3.20)

# Project Name
project(MazeGame)

# Set C++ version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the required Qt modules
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)

# Add the include directory
include_directories(${CMAKE_SOURCE_DIR}/include)

# Set sources and headers
set(SRC_FILES
    src/Maze.cpp
    src/GameWidget.cpp
    src/FlexibleIterator.cpp
    src/main.cpp
)

set(HEADER_FILES
    include/Maze.h
    include/GameWidget.h
    include/FlexibleIterator.h
)

set(UI_FILES
    ui/widget.ui
)

# Process headers with Q_OBJECT through moc
qt_wrap_cpp(MOC_SOURCES ${HEADER_FILES})

# Add the executable target
add_executable(MazeGame
    ${SRC_FILES}
    ${MOC_SOURCES}
    ${UI_FILES}
)

# Link Qt libraries
target_link_libraries(MazeGame PRIVATE Qt6::Widgets)
