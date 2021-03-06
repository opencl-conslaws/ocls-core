CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(python_Framework)

IF(NOT LIBRARY_OUTPUT_PATH)
	SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
ENDIF(NOT LIBRARY_OUTPUT_PATH)

FIND_PACKAGE(PythonLibs REQUIRED)

FIND_PATH(OCLS_C_INCLUDE_DIR NAMES c_framework.h DOC "Path to directory where framework.h can be found")
FIND_LIBRARY(OCLS_C_STATIC_LIBRARIES NAMES CFramework.obj CFrameworkd.obj CFramework.a CFrameworkd.a DOC "Path to directory where framework libraries can be found")

FIND_PATH(C_VISUALIZER_INCLUDE_DIR NAMES c_visualizer.h DOC "Path to directory where cvisualizer.h can be found")
FIND_LIBRARY(C_VISUALIZER_LIBRARIES NAMES CVisualizer.lib CVisualizer.so DOC "Path to directory where visualizer libraries can be found")

SET(OCLS_PYTHON_INCLUDE_DIR "${WRAPPER_DIR}/python_framework/include" CACHE PATH " ")
SET(OCLS_PYTHON_INTERFACE_DIR "${WRAPPER_DIR}/python_framework/interface" CACHE PATH " ")

find_program(PYTHON "python")
find_program(SWIG "swig")

if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
    SET(POSTFIX "d")
endif (CMAKE_BUILD_TYPE EQUAL "DEBUG")

if (PYTHON)
    if (SWIG)

        if (OCLS_USE_VIS)
            set(SETUP_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/setup_vis.py.in")
        else()
            set(SETUP_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in")
        endif()
        set(SETUP_PY    "${CMAKE_CURRENT_SOURCE_DIR}/setup.py")

        set(DEPS        "${OCLS_C_STATIC_LIBRARIES}_s")
        set(OUTPUT      "${LIBRARY_OUTPUT_PATH}/python")

        IF(APPLE)
            set(RPATH      "")
        ELSE()
            set(RPATH      "-rpath=${LIBRARY_OUTPUT_PATH}")
        ENDIF()
        configure_file(${SETUP_PY_IN} ${SETUP_PY})

        if(OCLS_USE_VIS)
            add_custom_command(OUTPUT ${OUTPUT}
                               COMMAND ${SWIG} -outdir ${CMAKE_CURRENT_SOURCE_DIR}/src/ocls_framework -lcpointer.i -python ${OCLS_PYTHON_INTERFACE_DIR}/framework_interface.i
                               COMMAND ${SWIG} -outdir ${CMAKE_CURRENT_SOURCE_DIR}/src/ocls_framework/visualizer -lcpointer.i -python ${OCLS_PYTHON_INTERFACE_DIR}/visualizer_interface.i
                               COMMAND ${PYTHON} ${SETUP_PY} build_ext --inplace
                               COMMAND ${CMAKE_COMMAND} -E touch ${OUTPUT}
                               DEPENDS ${DEPS})
        else()
            add_custom_command(OUTPUT ${OUTPUT}
                               COMMAND ${SWIG} -outdir ${CMAKE_CURRENT_SOURCE_DIR}/src/ocls_framework -lcpointer.i -python ${OCLS_PYTHON_INTERFACE_DIR}/framework_interface.i
                               COMMAND ${PYTHON} ${SETUP_PY} build_ext --inplace
                               COMMAND ${CMAKE_COMMAND} -E touch ${OUTPUT}
                               DEPENDS ${DEPS})
        endif()

        add_custom_target(PythonFramework ALL DEPENDS ${OUTPUT})
        SET_TARGET_PROPERTIES(PythonFramework PROPERTIES DEBUG_POSTFIX "d")

        install(CODE "execute_process(COMMAND ${PYTHON} ${SETUP_PY} install)")

    endif()
endif()