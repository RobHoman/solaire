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
list(APPEND DEPS_HEADERS ${DEPS_PATH}/glad/include/glad/glad.h)

# aggregate dependencies sources to add to the executable
list(APPEND DEPS_SOURCES ${DEPS_PATH}/glad/src/glad.c)

# Aggregate Solaire sources
add_subdirectory(src)

# add the executable
add_executable(Solaire ${DEPS_HEADERS} ${DEPS_SOURCES} ${SOLAIRE_HEADERS} ${SOLAIRE_SOURCES})

################
### Add deps ###
################
# GLFW
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
target_link_libraries(Solaire ${GLFW_STATIC_LIBRARIES})

# glm
find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

# glad
include_directories(${DEPS_PATH}/glad/include)

# easyloggingpp
include_directories(${DEPS_PATH}/easyloggingpp)

# rapidjson
include_directories(${DEPS_PATH}/rapidjson)

# install the exe in bin
install(TARGETS Solaire DESTINATION bin)
