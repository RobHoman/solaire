cmake_minimum_required (VERSION 3.0)
project (Solaire)
# The version number
set (Solaire_VERSION_MAJOR 1)
set (Solaire_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings to the source code
configure_file (
	"${PROJECT_SOURCE_DIR}/SolaireConfig.h.in"
	"${PROJECT_BINARY_DIR}/SolaireConfig.h"
	)

# add the binary tree to the search path for include files
# so that we will find SolaireConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# add the executable
add_executable(Solaire main.cpp)
