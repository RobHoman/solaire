set(Kato_SOURCES
	${Kato_SOURCES}
  ${CMAKE_CURRENT_SOURCE_DIR}/linux_platform.cpp
  PARENT_SCOPE
  )

set(Kato_HEADERS
  ${Kato_HEADERS}
  ${CMAKE_CURRENT_SOURCE_DIR}/linux_platform.h
  PARENT_SCOPE
  )
