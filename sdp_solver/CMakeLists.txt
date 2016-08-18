
FILE(
	GLOB
	SDP_FILES
	./*.cc
	./*.h  
	./*.hxx
	)

ADD_LIBRARY(sdp_modeler STATIC ${SDP_FILES} )
