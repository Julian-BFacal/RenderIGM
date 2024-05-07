# Set the name of the project and target:
SET(TARGET "spinningcube")
#Declare all source files the target consists of:
SET(TARGET_SRC
${TARGET}.cpp
#You can specify additional files here!
)
cmake_minimum_required( VERSION 2.6 )
project( ${TARGET} )
find_package( OpenThreads )
find_package( osg )
find_package( osgDB )
find_package( osgUtil )
find_package( osgViewer )
find_package( osgGA )
macro( config_project PROJNAME LIBNAME )
include_directories( ${${LIBNAME}_INCLUDE_DIR} )
target_link_libraries( ${PROJNAME} ${${LIBNAME}_LIBRARY} )
endmacro()
add_executable( ${TARGET} ${TARGET_SRC} )
config_project( ${TARGET} OPENTHREADS )
config_project( ${TARGET} OSG )
config_project( ${TARGET} OSGDB )
config_project( ${TARGET} OSGUTIL )
config_project( ${TARGET} OSGVIEWER )
config_project( ${TARGET} OSGGA )
