INCLUDE_DIRECTORIES( /home/manuel/Csdp/include . ../clustering ../data_bipartite ../data_unipartite)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pedantic -fmessage-length=0  -fPIC" )
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-conversion -Wno-sign-compare")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-strict-aliasing -Wno-unused-parameter")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3  -DNOSHORTS -DUSEGETTIME")

MESSAGE("CMAKE_CXX_FLAGS : ${CMAKE_CXX_FLAGS}")

ADD_LIBRARY( csdpinterface STATIC CsdpInterface.cpp CsdpInterface.h)
TARGET_LINK_LIBRARIES(csdpinterface /home/manuel/Csdp/lib/libsdp.a gfortran blas lapack m )

ADD_EXECUTABLE( divisive main.cpp)
TARGET_LINK_LIBRARIES( divisive csdpinterface dataunipartite clustering)
