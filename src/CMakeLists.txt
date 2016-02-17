# configure a header file to pass some of the CMake settings to the source code
configure_file (
	"SolaireConfig.h.in"
	"${PROJECT_BINARY_DIR}/SolaireConfig.h"
	)

set(Solaire_SOURCES
	${Solaire_SOURCES}
  ${CMAKE_CURRENT_SOURCE_DIR}/app.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/app_initialization.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/app_logic.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/clock.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  )

set(Solaire_HEADERS
  ${Solaire_HEADERS}
  ${CMAKE_CURRENT_SOURCE_DIR}/app.h
  ${CMAKE_CURRENT_SOURCE_DIR}/app_initialization.h
  ${CMAKE_CURRENT_SOURCE_DIR}/app_logic.h
  ${CMAKE_CURRENT_SOURCE_DIR}/clock.h
  )

set(SOLAIRE_SOURCES
	${Solaire_SOURCES}
	PARENT_SCOPE
  )

set(SOLAIRE_HEADERS
  ${Solaire_HEADERS}
  PARENT_SCOPE
  )
