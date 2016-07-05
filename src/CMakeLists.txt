# configure a header file to pass some of the CMake settings to the source code
configure_file (
	"SolaireConfig.h.in"
	"${PROJECT_BINARY_DIR}/SolaireConfig.h"
	)

# Add all of the directories which aggregate the .cpp and .h files into Solaire_SOURCES and Solaire_HEADERS respectively
add_subdirectory(app)

# Add main.cpp
set(Solaire_SOURCES
	${Solaire_SOURCES}
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  )

# Set the all caps versions for the root CMakeLists file.
set(SOLAIRE_SOURCES
	${Solaire_SOURCES}
	PARENT_SCOPE
  )

set(SOLAIRE_HEADERS
  ${Solaire_HEADERS}
  PARENT_SCOPE
  )
