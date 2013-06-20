
INCLUDE_DIRECTORIES( . ../clustering )

FILE(
	GLOB_RECURSE 
	kmeans_files 
	./*.cpp
	./*.hpp
	./*.h
)


ADD_LIBRARY(kmeans STATIC ${kmeans_files} )
