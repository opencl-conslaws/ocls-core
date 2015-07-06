PROJECT(demo)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#Set where executables end up
IF(NOT EXECUTABLE_OUTPUT_PATH)
	SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
ENDIF(NOT EXECUTABLE_OUTPUT_PATH)

if (OCLS_USE_VIS)

    FIND_PATH(OCLS_INCLUDE_DIR NAMES Framework.h DOC "Path to directory where framework.h can be found")
    FIND_LIBRARY(OCLS_LIBRARIES NAMES Framework.lib Framework.so DOC "Path to directory where framework libraries can be found")
    
    FIND_PATH(C_VISUALIZER_INCLUDE_DIR NAMES c_visualizer.h DOC "Path to directory where cvisualizer.h can be found")
    FIND_LIBRARY(C_VISUALIZER_LIBRARIES NAMES CVisualizer.lib CVisualizer.so DOC "Path to directory where visualizer libraries can be found")
    
    FIND_PATH(OCLS_C_INCLUDE_DIR NAMES cframework.h DOC "Path to directory where framework.h can be found")
    FIND_LIBRARY(OCLS_C_LIBRARIES NAMES CFramework.lib CFramework.so DOC "Path to directory where framework libraries can be found")
    
    FIND_PATH(SIMPLE_PLOT_INCLUDE_DIR NAMES SimplePlotModule.h DOC "Path to directory where SimplePlotModule.h can be found")
    
    FIND_LIBRARY(SIMPLE_PLOT_LIBRARY_DEBUG
    	NAMES "SimplePlotd.obj" "libSimplePlotd.a"
    	PATHS "${PROJECT_SOURCE_DIR}/modules/lib/")
    FIND_LIBRARY(SIMPLE_PLOT_LIBRARY_RELEASE
    	NAMES "SimplePlot.obj" "libSimplePlot.a"
    	PATHS "${PROJECT_SOURCE_DIR}/modules/lib/")
    
    IF(NOT SIMPLE_PLOT_LIBRARY_DEBUG AND NOT SIMPLE_PLOT_LIBRARY_RELEASE) 
    	SET(SIMPLE_PLOT_LIBRARIES SimplePlot CACHE FILEPATH " " FORCE)
    	LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})
    ELSE()
    	SET(SIMPLE_PLOT_LIBRARIES debug ${SIMPLE_PLOT_LIBRARY_DEBUG} optimized ${SIMPLE_PLOT_LIBRARY_RELEASE} CACHE FILEPATH "Path to SimplePlot libraries")
    ENDIF()
    
    INCLUDE_DIRECTORIES(
    	${demo_SOURCE_DIR}/include
    	${OCLS_INCLUDE_DIR}
    	${SIMPLE_PLOT_INCLUDE_DIR}
    	${CMAKE_CURRENT_BINARY_DIR})
    
    LINK_DIRECTORIES(
    	${OCLS_LIB_DIR}
    	${MODULE_LIB_DIR})
    
    ADD_EXECUTABLE(demo src/main.cpp)
    TARGET_LINK_LIBRARIES(demo 
    	${OCLS_LIBRARIES}
    	${SIMPLE_PLOT_LIBRARIES})
    
    
    ADD_EXECUTABLE(burger1D src/burger1D.cpp)
    TARGET_LINK_LIBRARIES(burger1D
    	${OCLS_LIBRARIES}
    	${SIMPLE_PLOT_LIBRARIES})
    
    ADD_EXECUTABLE(burger2D src/burger2D.cpp)
    TARGET_LINK_LIBRARIES(burger2D
    	${OCLS_LIBRARIES}
    	${SIMPLE_PLOT_LIBRARIES})
    
    
    
    INCLUDE_DIRECTORIES(
    	${demo_SOURCE_DIR}/include
    	${OCLS_C_INCLUDE_DIR}
    	${C_VISUALIZER_INCLUDE_DIR}
    	${CMAKE_CURRENT_BINARY_DIR})
    
    LINK_DIRECTORIES(
        ${OCLS_C_LIB_DIR})
    
    ADD_EXECUTABLE(cdemo src/cmain.c)
    TARGET_LINK_LIBRARIES(cdemo
        ${OCLS_C_LIBRARIES}
        ${C_VISUALIZER_LIBRARIES})

 ENDIF(OCLS_USE_VIS) 