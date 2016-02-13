

FILE(  GLOB  gencol_files   ./*.cpp ./*.h ./*.hpp ./*.hxx )


ADD_EXECUTABLE( gencol  ${gencol_files} )
IF( WIN32 )
	TARGET_LINK_LIBRARIES(gencol databipartite clustering heuristic ${CPLEX_LIB}/ilocplex.lib ${CPLEX_LIB}/cplex124.lib ) 
ELSE()
	TARGET_LINK_LIBRARIES(gencol databipartite clustering  heuristic ${CPLEX_LIB}/libilocplex.a ${CPLEX_LIB}/libcplex.a pthread ) 
ENDIF()
