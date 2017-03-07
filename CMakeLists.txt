cmake_minimum_required( VERSION 2.8 )

project( Corpuscle )

# extlibs
set( CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/" ${CMAKE_MODULE_PATH} )
find_package( SFML 2 REQUIRED system window graphics network audio )
find_package( Boost REQUIRED )
include_directories( ${SFML_INCLUDE} ${Boost_INCLUDE_DIRS} )
include_directories( "${PROJECT_SOURCE_DIR}/extlibs/glm" )

include_directories( ${PROJECT_SOURCE_DIR}/include )

# GCC
if( CMAKE_COMPILER_IS_GNUCXX )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall" )
	if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
		# Use dwarf-2 because most code profilers don't support v3 or v4
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -gdwarf-2")
	else()
		# Optimize even if Release isn't specified.
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
	endif()
endif()

# Linux
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()


# Make sure our executables end up in the top directory.
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )

add_subdirectory( ${PROJECT_SOURCE_DIR}/src/mc )
add_subdirectory( ${PROJECT_SOURCE_DIR}/src/renderer )
add_subdirectory( ${PROJECT_SOURCE_DIR}/src/ui )
add_subdirectory( ${PROJECT_SOURCE_DIR}/src/console )

# Copy data directory to build cache directory to be able to run examples from
# there. Useful for testing stuff.
add_custom_target(
	export_files
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/data" "${PROJECT_BINARY_DIR}/data"
)
