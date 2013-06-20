

INCLUDE_DIRECTORIES( . ../clustering )

FILE(  GLOB  gencol_files   ./*.cpp ./*.h ./*.hpp ./*.hxx )


ADD_EXECUTABLE( gencol  ${gencol_files} )
TARGET_LINK_LIBRARIES(gencol  clustering ) 