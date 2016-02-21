#INCLUDE_DIRECTORIES( . ../clustering ../bipartite ../cliquepartionning )

FILE(
	GLOB_RECURSE 
	databipartite_files 
	./*.cpp
	./*.h	
	./*.hxx )


ADD_LIBRARY(databipartite STATIC ${databipartite_files} )