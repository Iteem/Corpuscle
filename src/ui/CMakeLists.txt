find_package( SFGUI REQUIRED )
include_directories( ${SFGUI_INCLUDE_DIR} )

# Source files
set( SRC_DIR "${PROJECT_SOURCE_DIR}/src/ui" )
set( INC_DIR "${PROJECT_SOURCE_DIR}/include/ui" )
include_directories( ${INC_DIR} )

set( SOURCES
	${SRC_DIR}/main.cpp
)

add_executable( ${CMAKE_PROJECT_NAME} ${SOURCES})
target_link_libraries( ${CMAKE_PROJECT_NAME} ${SFML_LIBRARIES} ${THOR_LIBRARY} ${SFGUI_LIBRARY} ${Boost_LIBRARIES} )
target_link_libraries ( ${CMAKE_PROJECT_NAME} corpuscle-renderer) 