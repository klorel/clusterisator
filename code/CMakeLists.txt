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


#Don't compile tests by default
option(WITH_TESTS "Build tests" OFF)
set(GMOCK_ROOT "" CACHE STRING "Root of gmock. Only needed to compile tests")
if ( WITH_TESTS )

  #Let include <gtest/gtest.h>
  LINK_DIRECTORIES(${GMOCK_ROOT}/make)

  #Actual test files
  file(GLOB_RECURSE tests_files tests/*)
  add_executable(tests ${tests_files})

  #Link against files to tests, and the libgmock_main.a which contains the main and which run every tests
  #FIXME: by default we have gmock_main.a, but here it is required to have the prefix "lib". It could be better to let get rid of this "lib"-constraint
  target_link_libraries(tests clustering gmock_main)
  include_directories(${GMOCK_ROOT}/include ${GMOCK_ROOT}/gtest/include)

  #To have the flag -pthread
  find_package (Threads)
  target_link_libraries(tests ${CMAKE_THREAD_LIBS_INIT})
endif ( WITH_TESTS )
