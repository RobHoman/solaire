cmake_minimum_required (VERSION 3.0)
project (Solaire)
# The version number
set (Solaire_VERSION_MAJOR 1)
set (Solaire_VERSION_MINOR 0)

# add the binary tree to the search path for include files
include_directories("${PROJECT_BINARY_DIR}")

# Aggregate Solaire sources
add_subdirectory(src)

# add the executable
add_executable(Solaire ${SOLAIRE_SOURCES})

################
### Add deps ###
################
# GLFW
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
target_link_libraries(Solaire ${GLFW_STATIC_LIBRARIES})

# install the exe in bin
install(TARGETS Solaire DESTINATION bin)
