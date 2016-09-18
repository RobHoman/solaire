# Add all of the directories which aggregate the .cpp and .h files into Solaire_SOURCES and Solaire_HEADERS respectively. Ditto for Kato.
add_subdirectory(app)
add_subdirectory(common)
add_subdirectory(kato)

# Set the all caps versions for the root CMakeLists file.
set(SOLAIRE_SOURCES
	${Solaire_SOURCES}
	PARENT_SCOPE
  )

set(SOLAIRE_HEADERS
  ${Solaire_HEADERS}
  PARENT_SCOPE
  )

set(KATO_SOURCES
	${Kato_SOURCES}
	PARENT_SCOPE
  )

set(KATO_HEADERS
  ${Kato_HEADERS}
  PARENT_SCOPE
  )
