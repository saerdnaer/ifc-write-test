cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

#project(IFCBuilder)

#list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

# Setting the output for exe, dll, and lib files to the bin/Debug resp. bin/Release folders
# CAUTION: this is a fix so that dll files are always found; in the future, it will be better
# to create real install rules so that only dlls are copied into the correct folder, and .lib files not
SET(EXECUTABLE_OUTPUT_PATH bin)
SET(LIBRARY_OUTPUT_PATH bin)

set(srcs
	IFCBuilderTest.cpp
	IFCBuilder.cpp
	baseIfcObject.cpp
	boundingBoxIfc.cpp
	BRepIfc.cpp
	extrudedPolygonIfc.cpp
	Point3d.cpp
	SimpleMesh.cpp
	CDP_Building.cpp
)

set(includes
	IFCBuilder.h
	IFCBuilderTest.h
	IFCEngine.h
	Point3d.h
	SimpleMesh.h
	CDP_Building.h
)

#include_directories(".")

FIND_PACKAGE( Boost 1.40 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

link_directories(${EXECUTABLE_OUTPUT_PATH})

add_executable(IFCBuilderTest WIN32 ${srcs} ${includes} )
TARGET_LINK_LIBRARIES(IFCBuilderTest ${Boost_LIBRARIES} ../IFCEngine )