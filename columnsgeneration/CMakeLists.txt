

FILE(
	GLOB
	columnsgeneration_files
	./*.cpp 
	./*.h  
	./*.hxx
	)

ADD_LIBRARY(columnsgeneration STATIC  ${columnsgeneration_files} )

