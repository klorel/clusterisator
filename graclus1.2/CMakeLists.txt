cmake_minimum_required(VERSION 2.6)
project(graclus)

set(CMAKE_BUILD_TYPE "RELEASE")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pedantic  -Wall -Wextra  -fmessage-length=0 -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-uninitialized -Wno-conversion -Wno-unused-parameter -Wno-unused-variable -Wno-write-strings -Wno-unused-but-set-variable -Wno-parentheses -Wno-missing-braces -Wno-format-security -Wno-extra -Wno-unused-result") 

set(CMAKE_CXX_COMPILER "g++")

#set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
#set(LIBRARY_OUTPUT_PATH lib/${CMAKE_BUILD_TYPE})

# includes
INCLUDE_DIRECTORIES(metisLib multilevelLib programs )

# code sources
file(GLOB_RECURSE metis_files metisLib/* )
file(GLOB_RECURSE multilevel_files multilevelLib/* )
file(GLOB_RECURSE programs_files programs/* )

# libs
#add_library(metis STATIC ${metis_files} )
#add_library(multilevel STATIC ${multilevel_files} )


add_library(graclus1.2 STATIC ${metis_files} ${multilevel_files} AbstractGraclus.cpp IGraclus.cpp)
target_link_libraries(graclus1.2  m)

add_executable( graclus  ${programs_files} )
target_link_libraries(graclus graclus1.2)

