cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

#project(IFCBuilder)

#list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

# Setting the output for exe, dll, and lib files to the bin/Debug resp. bin/Release folders
# CAUTION: this is a fix so that dll files are always found; in the future, it will be better
# to create real install rules so that only dlls are copied into the correct folder, and .lib files not
SET(EXECUTABLE_OUTPUT_PATH ${CDP_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CDP_SOURCE_DIR}/bin)


set(srcs
	baseIfcObject.cpp
	boundingBoxIfc.cpp
	BRepIfc.cpp
	extrudedPolygonIfc.cpp
	IFCBuilder.cpp
	IFCBuilderTest.cpp
	IFCExport.cpp
	Point3d.cpp
	SimpleMesh.cpp
)

set(includes
	IFCBuilder.h
	IFCBuilderTest.h
	IFCEngine.h
	IFCExport.h
	Point3d.h
	SimpleMesh.h
)

include_directories("src")

#link_directories(${EXECUTABLE_OUTPUT_PATH})

add_executable(IFCBuilderTest ${srcs} ${includes} )
