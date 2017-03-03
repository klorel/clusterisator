
MESSAGE("IN EXE MIP_LIB is ${MIP_LIB}")



ADD_EXECUTABLE( unipartite  main_unipartite.cpp )
ADD_EXECUTABLE( unipartite_cpp  main_unipartite_cpp.cpp )

TARGET_LINK_LIBRARIES(unipartite     dataunipartite columnsgeneration clustering ${MIP_LIB})
TARGET_LINK_LIBRARIES(unipartite_cpp dataunipartite columnsgeneration clustering ${MIP_LIB})

ADD_EXECUTABLE( bipartite  main_bipartite.cpp )
ADD_EXECUTABLE( bipartite_cpp  main_bipartite_cpp.cpp )
ADD_EXECUTABLE( bipartite_random  main_bipartite_random.cpp )

TARGET_LINK_LIBRARIES(bipartite     heuristic databipartite columnsgeneration clustering ${MIP_LIB})
TARGET_LINK_LIBRARIES(bipartite_cpp databipartite columnsgeneration clustering ${MIP_LIB})
TARGET_LINK_LIBRARIES(bipartite_random databipartite columnsgeneration clustering ${MIP_LIB})


#ADD_EXECUTABLE( qcr  main_qcr.cpp )
#TARGET_LINK_LIBRARIES(qcr databipartite columnsgeneration clustering sdp_modeler ${MIP_LIB} $ENV{MOSEKDIR}/../bin/mosek7_0.lib)