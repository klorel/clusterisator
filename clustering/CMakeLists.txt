
FILE(
GLOB_RECURSE 
clustering_files 
../algos/*.cpp
../criterions/*.cpp
../data/*.cpp
../kmeans/*.cpp
../mesures/*.cpp 
../neighborhoods/*.cpp
../utils/*.cpp
)


ADD_LIBRARY(clustering STATIC ${clustering_files} )

TARGET_LINK_LIBRARIES(clustering )
