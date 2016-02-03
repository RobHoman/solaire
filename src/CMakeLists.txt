# configure a header file to pass some of the CMake settings to the source code
configure_file (
	"SolaireConfig.h.in"
	"${PROJECT_BINARY_DIR}/SolaireConfig.h"
	)

set(Solaire_SOURCES
	${Solaire_SOURCES}
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

set(SOLAIRE_SOURCES
	${Solaire_SOURCES}
	PARENT_SCOPE)
