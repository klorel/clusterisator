
INCLUDE_DIRECTORIES( . ../clustering ../kmeans)


FILE(  GLOB  main_files   ./*.cpp )

SET( prog_names )
FOREACH( main ${main_files} ) 
	SET(prog "")
	GET_FILENAME_COMPONENT(prog ${main} NAME_WE)

	MESSAGE( "prog found : ${prog}" ) 
	ADD_EXECUTABLE( ${prog}  ${prog}.cpp )
	TARGET_LINK_LIBRARIES(${prog}  clustering kmeans)
	
ENDFOREACH( main )
