cmake_minimum_required (VERSION 3.0)

message(CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE})
if("${CMAKE_TOOLCHAIN_FILE}" STREQUAL "")
  message(STATUS "No toolchain file specified; using defaults.")
  set(CMAKE_SYSTEM_NAME Linux)
  set(CMAKE_C_COMPILER clang)
  set(CMAKE_CXX_COMPILER clang++)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
else()
  message(STATUS "Using toolchain file ${CMAKE_TOOLCHAIN_FILE}")
endif()

project(Solaire C CXX)
# The version number
set (Solaire_VERSION_MAJOR 0)
set (Solaire_VERSION_MINOR 0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMakeModules)
message(CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH})

# define path to dependencies stored within the main repo
set(DEPS_PATH deps)

# add the binary tree to the search path for include files
include_directories("${PROJECT_BINARY_DIR}")

# aggregate header file dependencies and source header files before adding the executable
list(APPEND DEPS_HEADERS ${DEPS_PATH}/glad/include/glad/glad.h)

# aggregate dependencies sources to add to the executable
list(APPEND DEPS_SOURCES ${DEPS_PATH}/glad/src/glad.c)

# Aggregate Solaire sources. Sets SOLAIRE_HEADERS and SOLAIRE_SOURCES
add_subdirectory(src)
message(SOLAIRE_SOURCES: ${SOLAIRE_SOURCES})
message(SOLAIRE_HEADERS: ${SOLAIRE_HEADERS})
message(KATO_SOURCES: ${KATO_SOURCES})
message(KATO_HEADERS: ${KATO_HEADERS})

add_library(Solaire SHARED ${DEPS_HEADERS} ${DEPS_SOURCES} ${SOLAIRE_HEADERS} ${SOLAIRE_SOURCES})
target_include_directories(Solaire PRIVATE src)

add_executable(Kato ${DEPS_HEADERS} ${DEPS_SOURCES} ${KATO_HEADERS} ${KATO_SOURCES})
target_include_directories(Kato PRIVATE src)

################
### Add deps ###
################
# TODO(phil): Document here what -ldl and -lm are... I don't remember.
target_link_libraries(Solaire -ldl -lm)
target_link_libraries(Kato -ldl -lm)

# SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
target_include_directories(Kato PRIVATE
  ${SDL2_INCLUDE_DIR}
  ${SDL2_IMAGE_INCLUDE_DIR}
  ${SDL2_TTF_INCLUDE_DIR})
target_link_libraries(Kato ${SDL2_LIBRARY}
                           ${SDL2_IMAGE_LIBRARIES}
                           ${SDL2_TTF_LIBRARIES})
# glm
target_include_directories(Solaire PRIVATE ${DEPS_PATH}/glm)

# glad
target_include_directories(Solaire PRIVATE ${DEPS_PATH}/glad/include)
target_include_directories(Kato PRIVATE ${DEPS_PATH}/glad/include)
