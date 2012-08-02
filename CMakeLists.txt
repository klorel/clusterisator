cmake_minimum_required(VERSION 2.6)
project(clusterisator)

set(CMAKE_BUILD_TYPE "RELEASE")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -DIL_STD -pedantic -Wall -Wextra -Wconversion -fmessage-length=0  -fPIC -Wno-strict-aliasing -Wno-unused-parameter")

set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
set(LIBRARY_OUTPUT_PATH lib/${CMAKE_BUILD_TYPE})
LINK_DIRECTORIES( ~/graclus1.2 )

# includes
INCLUDE_DIRECTORIES(inc inc/libgraclus)

# code sources
file(GLOB_RECURSE clustering_files src/* )

# libs
add_library(clustering STATIC ${clustering_files} )

# link
target_link_libraries(clustering  multilevel metis)

add_executable( exe  main.cpp )
target_link_libraries(exe  clustering )


