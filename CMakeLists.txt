cmake_minimum_required(VERSION 2.6)
project(clusterisator)

INCLUDE_DIRECTORIES( gmock-1.6.0/gtest/include code/inc graclus1.2 interfaces )

set ( EXECUTABLE_OUTPUT_PATH  ../bin )
set ( LIBRARY_OUTPUT_PATH  ../lib    )
set ( CMAKE_BUILD_TYPE "RELEASE"     )

add_subdirectory(gmock-1.6.0)

add_subdirectory(graclus1.2)
add_subdirectory(code)

add_subdirectory(tests)
