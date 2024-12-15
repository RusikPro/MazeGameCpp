# Enable automatic MOC processing for Qt
set(CMAKE_AUTOMOC ON)

# Add the gui library
add_library(gui STATIC
    include/gui/GameWidget.h
    src/GameWidget.cpp
)

# Link to Maze module
target_link_libraries(gui PUBLIC maze)

# Link Qt Widgets library
target_link_libraries(gui PRIVATE Qt6::Widgets)

# Expose include directory
target_include_directories(gui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
