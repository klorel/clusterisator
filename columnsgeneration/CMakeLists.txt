

FILE(  GLOB  columnsgeneration_files   ./*.cpp ./*.h ./*.hpp ./*.hxx )


ADD_LIBRARY(columnsgeneration STATIC  ${columnsgeneration_files} )
