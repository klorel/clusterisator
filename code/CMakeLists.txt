cmake_minimum_required(VERSION 2.6)
project(code)

set(CMAKE_BUILD_TYPE "RELEASE")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -DIL_STD -pedantic -Wall -Wextra -Wconversion -fmessage-length=0  -fPIC -Wno-strict-aliasing -Wno-unused-parameter")

# includes
INCLUDE_DIRECTORIES(inc ../graclus1.2)

# code sources
file(GLOB_RECURSE clustering_files src/* )

# libs
add_library(clustering STATIC ${clustering_files} )

# link
target_link_libraries(clustering graclus1.2)

add_executable( exe  main.cpp )
target_link_libraries(exe  clustering )


