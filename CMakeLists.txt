cmake_minimum_required(VERSION 2.6)
project(clusterisator)

INCLUDE_DIRECTORIES( gmock-1.6.0/gtest/include  algos criterions interfaces  mesures neighborhoods utils   )

set ( EXECUTABLE_OUTPUT_PATH  ../bin )
set ( LIBRARY_OUTPUT_PATH  ../lib    )
set ( CMAKE_BUILD_TYPE "RELEASE"     )

option( WITH_TESTS "Build tests" OFF)
if( WITH_TESTS )
add_subdirectory(gmock-1.6.0)
endif( WITH_TESTS )

#add_subdirectory(graclus1.2)
add_subdirectory(clustering)

if( WITH_TESTS )
add_subdirectory(tests)
endif( WITH_TESTS )

add_subdirectory(programs)
