#INCLUDE_DIRECTORIES( . ../clustering )

FILE(
	GLOB_RECURSE 
	dataunipartite_files 
	./*.cpp
	./*.hpp
	./*.h
)


ADD_LIBRARY(dataunipartite STATIC ${dataunipartite_files} )