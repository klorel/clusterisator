CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(clusterisator)

INCLUDE_DIRECTORIES( gmock-1.6.0/gtest/include   )

SET ( EXECUTABLE_OUTPUT_PATH  ../bin )
SET ( LIBRARY_OUTPUT_PATH  ../lib    )
SET ( CMAKE_BUILD_TYPE "RELEASE"     )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pedantic -fmessage-length=0  -fPIC -UNDEBUG" )
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-strict-aliasing -Wno-unused-parameter")

IF( WITH_TESTS )
    ADD_SUBDIRECTORY(gmock-1.6.0)
ENDIF( WITH_TESTS )

#INCLUDE_DIRECTORIES(  )


OPTION( WITH_TESTS "Build tests" ON)


ADD_SUBDIRECTORY(clustering)
ADD_SUBDIRECTORY(programs)

IF( WITH_TESTS )
    ADD_SUBDIRECTORY(tests)
ENDIF( WITH_TESTS )

