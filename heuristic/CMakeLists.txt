
INCLUDE_DIRECTORIES( . ../clustering ../data_bipartite ../bipartite ../cliquepartionning)

FILE(
	GLOB
	heuristic_files
   ./*.cpp
   ./*.h
   ./*.hxx 
   )


ADD_LIBRARY(heuristic STATIC ${heuristic_files} )


