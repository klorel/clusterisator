
file(GLOB_RECURSE tests_files ./*.cc )

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -DIL_STD -pedantic -Wall -Wextra -Wconversion -fmessage-length=0  -fPIC -Wno-strict-aliasing -Wno-unused-parameter")


add_executable(unittests ${tests_files} )

#To have the flag -pthread
find_package (Threads)
target_link_libraries(unittests ${CMAKE_THREAD_LIBS_INIT} gmock_main)

