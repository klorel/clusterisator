
# SET(CPLEX_INC "C:/Program Files/IBM/ILOG/CPLEX_Studio124/cplex")
# SET(CPLEX_LIB "C:/Program Files/IBM/ILOG/CPLEX_Studio124/cplex")

SET(CPLEX_INC "C:/Program Files (x86)/IBM/ILOG/CPLEX_Studio124/cplex/include/ilcplex")
SET(CPLEX_LIB "C:/Program Files (x86)/IBM/ILOG/CPLEX_Studio124/cplex/lib/x86_windows_vs2010/stat_mtd")

#SET(CPLEX_INC "/opt/cplex124/cplex/include/ilcplex")
#SET(CPLEX_LIB "/opt/cplex124/cplex/lib/x86-64_sles10_4.1/static_pic")

INCLUDE_DIRECTORIES( . ../clustering ${CPLEX_INC})

FILE(  GLOB  gencol_files   ./*.cpp ./*.h ./*.hpp ./*.hxx )


ADD_EXECUTABLE( bipartite  ${gencol_files} )
TARGET_LINK_LIBRARIES(bipartite  clustering ${CPLEX_LIB}/ilocplex.lib ${CPLEX_LIB}/cplex124.lib ) 
#TARGET_LINK_LIBRARIES(bipartite  clustering ${CPLEX_LIB}/libilocplex.a ${CPLEX_LIB}/libcplex.a pthread ) 
