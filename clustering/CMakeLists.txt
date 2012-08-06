

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -DIL_STD -pedantic -Wall -Wextra -Wconversion -fmessage-length=0  -fPIC -Wno-strict-aliasing -Wno-unused-parameter")

file(
GLOB_RECURSE 
clustering_files 
../algos/*.cpp
../criterions/*.cpp
../mesures/*.cpp 
../neighborhoods/*.cpp
../utils/*.cpp
)


add_library(clustering STATIC ${clustering_files} )

target_link_libraries(clustering graclus1.2)
