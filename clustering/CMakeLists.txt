
FILE(
GLOB_RECURSE 
clustering_files 
../src/*.cpp
)


ADD_LIBRARY(clustering STATIC ${clustering_files} )

TARGET_LINK_LIBRARIES(clustering )
