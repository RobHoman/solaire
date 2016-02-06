cmake_minimum_required (VERSION 3.0)

project (Solaire)
# The version number
set (Solaire_VERSION_MAJOR 1)
set (Solaire_VERSION_MINOR 0)

# define path to dependencies stored within the main repo
set(DEPS_PATH deps)

# add the binary tree to the search path for include files
include_directories("${PROJECT_BINARY_DIR}")

# aggregate header file dependencies and source header files before adding the executable
list(APPEND DEPS_HEADERS ${DEPS_PATH}/easyloggingpp/easylogging++.h)

# Aggregate Solaire sources
add_subdirectory(src)

# add the executable
add_executable(Solaire ${SOLAIRE_SOURCES} ${DEPS_HEADERS})

################
### Add deps ###
################
# GLFW
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
target_link_libraries(Solaire ${GLFW_STATIC_LIBRARIES})

# GLM
find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

# easyloggingpp
include_directories(${DEPS_PATH}/easyloggingpp)

#GLAD

# install the exe in bin
install(TARGETS Solaire DESTINATION bin)
