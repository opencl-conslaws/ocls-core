PROJECT(simple_plot)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# - Find packages we need
FIND_PACKAGE(GLM REQUIRED)

IF(NOT LIBRARY_OUTPUT_PATH)
	SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
ENDIF(NOT LIBRARY_OUTPUT_PATH)

FIND_PATH(OCLS_INCLUDE_DIR NAMES Framework.h DOC "Path to directory where framework.h can be found")
FIND_LIBRARY(OCLS_LIBRARIES NAMES Framework.lib Framework.so DOC "Path to directory where framework libraries can be found")

SET(SIMPLE_PLOT_INCLUDE_DIR "${MODULE_DIR}/simple_plot/include" CACHE PATH " ")

INCLUDE_DIRECTORIES(
	${OCLS_INCLUDE_DIR}
	${SIMPLE_PLOT_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR})


SET(glsl_srcs "") 
FILE(GLOB simple_plot_vglsl shaders/*.glsl)
FOREACH(filename ${simple_plot_vglsl})
    GET_FILENAME_COMPONENT(target ${filename} NAME_WE)
    SET(name "${CMAKE_CURRENT_SOURCE_DIR}/include/glsl_src/${target}.h")
    SET(glsl_srcs ${glsl_srcs} ${name})
	GET_TARGET_PROPERTY(TXT_TO_H_EXE TxtToH LOCATION)
	ADD_CUSTOM_COMMAND(
	  OUTPUT ${name}
		COMMAND ${TXT_TO_H_EXE} ${filename} ${name}
	  DEPENDS TxtToH ${filename}
	)
ENDFOREACH(filename ${simple_plot_vglsl})

LINK_DIRECTORIES(
	${OCLS_LIB_DIR})

FILE(GLOB SIMPLE_PLOT_SRC "src/*.cpp")
ADD_LIBRARY (SimplePlot STATIC ${SIMPLE_PLOT_SRC} ${glsl_srcs})
SET_TARGET_PROPERTIES(SimplePlot PROPERTIES DEBUG_POSTFIX "d")
INSTALL(TARGETS SimplePlot DESTINATION modules/lib)
