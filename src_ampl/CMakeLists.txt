
MESSAGE("IN EXE MIP_LIB is ${MIP_LIB}")

FILE(
	GLOB
	ampl_bipartite_files
	./*.cpp 
	./*.h  
	./*.hxx
	)
SET(AMPL_INC $ENV{AMPLINC})
SET(AMPL_LIB $ENV{AMPLLIB})

MESSAGE("AMPL_INC is ${AMPL_INC}")
MESSAGE("AMPL_LIB is ${AMPL_LIB}")

INCLUDE_DIRECTORIES(. ${AMPL_INC})
ADD_EXECUTABLE( ampl_bipartite  ${ampl_bipartite_files} )

TARGET_LINK_LIBRARIES(ampl_bipartite     heuristic databipartite columnsgeneration clustering ${MIP_LIB} ${AMPL_LIB}/ampl-1.3.1.0.lib)