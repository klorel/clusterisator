

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pedantic -Wall -Wextra -Wconversion -fmessage-length=0  -fPIC -Wno-strict-aliasing -Wno-unused-parameter")


add_executable( exe  main.cpp )
target_link_libraries(exe  clustering )