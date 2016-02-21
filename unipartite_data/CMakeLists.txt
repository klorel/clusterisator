#INCLUDE_DIRECTORIES( . ../clustering )

FILE(
	GLOB_RECURSE 
	dataunipartite_files 
	./*.cpp
	./*.h
	./*.hxx
)


ADD_LIBRARY(dataunipartite STATIC ${dataunipartite_files} )