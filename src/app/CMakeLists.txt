set(Solaire_SOURCES
	${Solaire_SOURCES}
  ${CMAKE_CURRENT_SOURCE_DIR}/app.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/app_config.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/app_logic.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/clock.cpp
  PARENT_SCOPE
  )

set(Solaire_HEADERS
  ${Solaire_HEADERS}
  ${CMAKE_CURRENT_SOURCE_DIR}/app.h
  ${CMAKE_CURRENT_SOURCE_DIR}/app_config.h
  ${CMAKE_CURRENT_SOURCE_DIR}/app_logic.h
  ${CMAKE_CURRENT_SOURCE_DIR}/clock.h
  PARENT_SCOPE
  )
