#INCLUDE_DIRECTORIES( . ../data_bipartite ../data_unipartite)

FILE(
	GLOB_RECURSE 
	mip_solver_files 
	./*.cpp
	./*.h
    ./*.hxx
	)
ADD_LIBRARY(mip_solver STATIC ${mip_solver_files} ) 


###
# MIP SOLVER
### 
SET(MIP_LIB mip_solver)
IF( WIN32 )
	IF(NOT ${CPLEX_INC} STREQUAL "")
		SET(MIP_LIB ${MIP_LIB} ${CPLEX_LIB})
	ENDIF()
	IF(NOT ${XPRESS_LIB} STREQUAL "")
		SET(MIP_LIB ${MIP_LIB} ${XPRESS_LIB}/xprs.lib)
	ENDIF()
ELSE()
	IF(NOT ${CPLEX_INC} STREQUAL "")
		SET(MIP_LIB ${MIP_LIB} ${CPLEX_LIB} pthread ) 
	ENDIF()
	IF(NOT ${XPRESS_LIB} STREQUAL "")
		SET(MIP_LIB ${MIP_LIB} ${XPRESS_LIB}/xprs.a)
	ENDIF()
ENDIF()
SET(MIP_LIB ${MIP_LIB} PARENT_SCOPE)
MESSAGE("MIP_LIB is ${MIP_LIB}")