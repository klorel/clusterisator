CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(clusterisator)

OPTION( WITH_TESTS "Build tests" OFF)
OPTION( WITH_PROF "Build with profiling" OFF)
OPTION( WITH_DEBUG "Build with profiling" OFF)

IF( WITH_TESTS )
INCLUDE_DIRECTORIES( gmock-1.6.0/gtest/include )
ENDIF( WITH_TESTS )

INCLUDE_DIRECTORIES( src   )


SET ( EXECUTABLE_OUTPUT_PATH  ../bin )
SET ( LIBRARY_OUTPUT_PATH  ../lib    )

IF( WITH_DEBUG )
	SET ( CMAKE_BUILD_TYPE "DEBUG"     )
ELSE( WITH_DEBUG )
	SET ( CMAKE_BUILD_TYPE "RELEASE"     )
ENDIF( WITH_DEBUG )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pedantic -fmessage-length=0  -fPIC" )
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-strict-aliasing -Wno-unused-parameter")


IF( WITH_TESTS )
    ADD_SUBDIRECTORY(gmock-1.6.0)
ENDIF( WITH_TESTS )

IF( WITH_PROF )
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
ENDIF( WITH_PROF )


ADD_SUBDIRECTORY(clustering)
ADD_SUBDIRECTORY(programs)

IF( WITH_TESTS )
    ADD_SUBDIRECTORY(tests)
ENDIF( WITH_TESTS )

