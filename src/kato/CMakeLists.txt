set(Solaire_SOURCES
	${Solaire_SOURCES}
  ${CMAKE_CURRENT_SOURCE_DIR}/linux_platform.cpp
  PARENT_SCOPE
  )

set(Solaire_HEADERS
  ${Solaire_HEADERS}
  ${CMAKE_CURRENT_SOURCE_DIR}/platform_services.h
  PARENT_SCOPE
  )
