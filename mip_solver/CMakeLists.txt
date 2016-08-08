#INCLUDE_DIRECTORIES( . ../data_bipartite ../data_unipartite)

FILE(
	GLOB_RECURSE 
	mip_solver_files 
	./*.cpp
	./*.h
    ./*.hxx
	)



ADD_LIBRARY(mip_solver STATIC ${mip_solver_files} ) 