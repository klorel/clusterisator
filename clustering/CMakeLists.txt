
FILE(
	GLOB_RECURSE 
	clustering_files 
	../src/*.cpp
	../src/*.hpp
	../src/*.h
)


ADD_LIBRARY(clustering STATIC ${clustering_files} )

TARGET_LINK_LIBRARIES(clustering )
