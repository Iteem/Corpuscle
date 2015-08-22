# Source files
set( SRC_DIR "${PROJECT_SOURCE_DIR}/src/console" )
set( INC_DIR "${PROJECT_SOURCE_DIR}/include/console" )
include_directories( ${INC_DIR} )

set( SOURCES
	${SRC_DIR}/main.cpp
)

add_executable( corpuscle-console ${SOURCES})
target_link_libraries( corpuscle-console ${SFML_LIBRARIES} ${SFGUI_LIBRARY} ${Boost_LIBRARIES} )
target_link_libraries( corpuscle-console corpuscle-renderer )