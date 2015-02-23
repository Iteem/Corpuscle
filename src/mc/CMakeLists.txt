# Source files
find_package( zlib REQUIRED )
include_directories( ${SFML_INCLUDE} ${Boost_INCLUDE_DIRS} )

set( SRC_DIR "${PROJECT_SOURCE_DIR}/src/mc" )
set( INC_DIR "${PROJECT_SOURCE_DIR}/include/mc" )
include_directories( ${INC_DIR} )

set( SOURCES
	${INC_DIR}/chunk.hpp
	${INC_DIR}/map.hpp
	${INC_DIR}/nbt.hpp
	${INC_DIR}/node.hpp
	${SRC_DIR}/chunk.cpp
	${SRC_DIR}/map.cpp
	${SRC_DIR}/nbt.cpp
	${SRC_DIR}/node.cpp
)

add_library( corpuscle-mc ${SOURCES} )
target_link_libraries( corpuscle-mc ${SFML_LIBRARIES} ${Boost_LIBRARIES} ${ZLIB_LIBRARIES} )
