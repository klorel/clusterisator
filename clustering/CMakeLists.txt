INCLUDE_DIRECTORIES( . )

FILE(
	GLOB_RECURSE 
	clustering_files 
	./*.cpp
	./*.hpp
	./*.h
)


ADD_LIBRARY(clustering STATIC ${clustering_files} )