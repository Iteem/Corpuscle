cmake_minimum_required( VERSION 2.8 )

project( Corpuscle )

# extlibs
set( CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/" ${CMAKE_MODULE_PATH} )
find_package( SFML 2 REQUIRED system window graphics network audio )
find_package( Thor REQUIRED )
find_package( Boost REQUIRED )
include_directories( ${SFML_INCLUDE} ${THOR_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} )

# Source files
set( SRC_DIR "${PROJECT_SOURCE_DIR}/src/" )
set( INC_DIR "${PROJECT_SOURCE_DIR}/include/" )
include_directories( ${INC_DIR} )

set( SOURCES
	${INC_DIR}/cuboid.hpp
	${INC_DIR}/object.hpp
	${INC_DIR}/random.hpp
	${INC_DIR}/renderer.hpp
	${INC_DIR}/rendermanager.hpp
	${INC_DIR}/random.inl
	${INC_DIR}/ray.hpp
	${INC_DIR}/scene.hpp
	${INC_DIR}/sphere.hpp
	${INC_DIR}/utility.hpp
	${INC_DIR}/utility.inl
	${SRC_DIR}/cuboid.cpp
	${SRC_DIR}/main.cpp
	${SRC_DIR}/object.cpp
	${SRC_DIR}/ray.cpp
	${SRC_DIR}/renderer.cpp
	${SRC_DIR}/rendermanager.cpp
	${SRC_DIR}/scene.cpp
	${SRC_DIR}/sphere.cpp
	${SRC_DIR}/utility.cpp
)

# GCC
if( CMAKE_COMPILER_IS_GNUCXX )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall" )
endif()

add_executable( ${CMAKE_PROJECT_NAME} ${SOURCES})
target_link_libraries( ${CMAKE_PROJECT_NAME} ${SFML_LIBRARIES} ${THOR_LIBRARY} ${Boost_LIBRARIES} )

# Copy data directory to build cache directory to be able to run examples from
# there. Useful for testing stuff.
add_custom_target(
	export_files
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/data" "${PROJECT_BINARY_DIR}/data"
)
