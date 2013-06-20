

INCLUDE_DIRECTORIES( . ../clustering )

FILE(  GLOB  gencol_files   ./*.cpp ./*.hpp ./*.hxx )


ADD_EXECUTABLE( gencol  ${gencol_files} )
TARGET_LINK_LIBRARIES(gencol  clustering ) 