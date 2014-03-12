INCLUDE_DIRECTORIES( . ../data_bipartite ../data_unipartite)

FILE(
	GLOB_RECURSE 
	clustering_files 
	./*.cpp
	./*.hpp
	./*.h
)


ADD_LIBRARY(clustering STATIC ${clustering_files} ) 