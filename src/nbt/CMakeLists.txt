# Source files
find_package( zlib REQUIRED )
include_directories( ${SFML_INCLUDE} ${Boost_INCLUDE_DIRS} )

set( SRC_DIR "${PROJECT_SOURCE_DIR}/src/nbt" )
set( INC_DIR "${PROJECT_SOURCE_DIR}/include/nbt" )
include_directories( ${INC_DIR} )

set( SOURCES
	${INC_DIR}/chunk.hpp
	${INC_DIR}/nbt.hpp
	${INC_DIR}/node.hpp
	${SRC_DIR}/chunk.cpp
	${SRC_DIR}/nbt.cpp
	${SRC_DIR}/node.cpp
)

add_library( corpuscle-nbt ${SOURCES} )
target_link_libraries( corpuscle-nbt ${SFML_LIBRARIES} ${Boost_LIBRARIES} ${ZLIB_LIBRARIES} )
