# Source files
set( SRC_DIR "${PROJECT_SOURCE_DIR}/src/renderer" )
set( INC_DIR "${PROJECT_SOURCE_DIR}/include/renderer" )
include_directories( ${INC_DIR} )

set( SOURCES
	${INC_DIR}/aabb.hpp
	${INC_DIR}/bvh.hpp
	${INC_DIR}/camera.hpp
	${INC_DIR}/cuboid.hpp
	${INC_DIR}/object.hpp
	${INC_DIR}/random.hpp
	${INC_DIR}/renderer.hpp
	${INC_DIR}/rendermanager.hpp
	${INC_DIR}/random.inl
	${INC_DIR}/ray.hpp
	${INC_DIR}/ray.inl
	${INC_DIR}/scene.hpp
	${INC_DIR}/sphere.hpp
	${INC_DIR}/utility.hpp
	${INC_DIR}/utility.inl
	${SRC_DIR}/aabb.cpp
	${SRC_DIR}/bvh.cpp
	${SRC_DIR}/camera.cpp
	${SRC_DIR}/cuboid.cpp
	${SRC_DIR}/object.cpp
	${SRC_DIR}/renderer.cpp
	${SRC_DIR}/rendermanager.cpp
	${SRC_DIR}/scene.cpp
	${SRC_DIR}/sphere.cpp
	${SRC_DIR}/utility.cpp
)

add_library( corpuscle-renderer ${SOURCES} )
target_link_libraries( corpuscle-renderer corpuscle-nbt ${SFML_LIBRARIES} ${Boost_LIBRARIES} )
