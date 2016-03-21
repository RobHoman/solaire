cmake_minimum_required (VERSION 3.0)

if(${CMAKE_TOOLCHAIN_FILE})
  message(STATUS "Using toolchain file ${CMAKE_TOOLCHAIN_FILE}")
else()
  message(STATUS "No toolchain file specified; using defaults.")
  set(CMAKE_SYSTEM_NAME Linux)
  set(CMAKE_C_COMPILER clang)
  set(CMAKE_CXX_COMPILER clang++)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

project (Solaire)
# The version number
set (Solaire_VERSION_MAJOR 1)
set (Solaire_VERSION_MINOR 0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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
add_subdirectory(${DEPS_PATH}/glfw)
include_directories(Solaire PRIVATE ${DEPS_PATH}/glfw/include)
target_link_libraries(Solaire ${GLFW_STATIC_LIBRARIES})

# glm
target_include_directories(Solaire PRIVATE ${DEPS_PATH}/glm)

# glad
target_include_directories(Solaire PRIVATE ${DEPS_PATH}/glad/include)

# easyloggingpp
target_include_directories(Solaire PRIVATE ${DEPS_PATH}/easyloggingpp)

# rapidjson
target_include_directories(Solaire PRIVATE ${DEPS_PATH}/rapidjson/include)

# install the exe in bin
install(TARGETS Solaire DESTINATION bin)
