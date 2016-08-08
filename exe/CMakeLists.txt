
###
# MIP SOLVER
### 
SET(MIP_LIB mip_solver )
IF( WIN32 )
	IF(NOT ${CPLEX_LIB} STREQUAL "")
		SET(MIP_LIB ${MIP_LIB} ${CPLEX_LIB}/ilocplex.lib ${CPLEX_LIB}/cplex124.lib)
	ELSEIF(NOT ${XPRESS_LIB} STREQUAL "")
		SET(MIP_LIB ${MIP_LIB} ${XPRESS_LIB}/xprs.lib)
	ENDIF()
ELSE()
	IF(NOT ${CPLEX_LIB} STREQUAL "")
		SET(MIP_LIB ${MIP_LIB} ${CPLEX_LIB}/libilocplex.a ${CPLEX_LIB}/libcplex.a pthread ) 
	ELSEIF(NOT ${XPRESS_LIB} STREQUAL "")
		SET(MIP_LIB ${MIP_LIB} ${XPRESS_LIB}/xprs.a)
	ENDIF()
ENDIF()

MESSAGE("MIP_LIB is ${MIP_LIB}")

ADD_EXECUTABLE( unipartite  main_unipartite.cpp )
ADD_EXECUTABLE( unipartite_cpp  main_unipartite_cpp.cpp )

TARGET_LINK_LIBRARIES(unipartite     dataunipartite columnsgeneration clustering ${MIP_LIB})
TARGET_LINK_LIBRARIES(unipartite_cpp dataunipartite columnsgeneration clustering ${MIP_LIB})

ADD_EXECUTABLE( bipartite  main_bipartite.cpp )
ADD_EXECUTABLE( bipartite_cpp  main_bipartite_cpp.cpp )

TARGET_LINK_LIBRARIES(bipartite     databipartite columnsgeneration clustering ${MIP_LIB})
TARGET_LINK_LIBRARIES(bipartite_cpp databipartite columnsgeneration clustering ${MIP_LIB})