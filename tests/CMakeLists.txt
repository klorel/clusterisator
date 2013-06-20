
FILE(GLOB_RECURSE tests_files ./*.cpp )

ADD_EXECUTABLE(unittests ${tests_files} )
ADD_TEST(utests ../bin/unittests)

#To have the flag -
FIND_PACKAGE (Threads)
TARGET_LINK_LIBRARIES(unittests ${CMAKE_THREAD_LIBS_INIT} gmock_main clustering)

