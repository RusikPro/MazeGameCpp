# Add the common library
add_library(common STATIC
    include/common/FlexibleIterator.h
    include/common/Constants.h
    include/common/Point.h
    include/common/Timer.h
    src/FlexibleIterator.cpp
)

# Expose include directory
target_include_directories(common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
