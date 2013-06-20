
FILE(  GLOB  projet_zz_files   ./*.cpp )


ADD_EXECUTABLE( projet_zz  ${projet_zz_files} )
TARGET_LINK_LIBRARIES(projet_zz  clustering )