find_package( OpenGL REQUIRED )
include_directories( ${OPENGL_INCLUDE_DIRS} )

# Source files
set( SRC_DIR "${PROJECT_SOURCE_DIR}/src/ui" )
set( INC_DIR "${PROJECT_SOURCE_DIR}/include/ui" )
include_directories( ${INC_DIR} )

# imgui
set( IMGUI_SRC_DIR "${PROJECT_SOURCE_DIR}/extlibs/imgui" )
set( IMGUI_INC_DIR "${PROJECT_SOURCE_DIR}/extlibs/imgui" )
include_directories( ${IMGUI_INC_DIR} )

set( SOURCES
	${SRC_DIR}/main.cpp

	${IMGUI_SRC_DIR}/imgui.cpp
	${IMGUI_SRC_DIR}/imgui_draw.cpp
	${IMGUI_SRC_DIR}/imgui-SFML.cpp
	${IMGUI_SRC_DIR}/imgui_demo.cpp
)

add_executable( ${CMAKE_PROJECT_NAME} ${SOURCES})
target_link_libraries( ${CMAKE_PROJECT_NAME} ${OPENGL_LIBRARIES} ${SFML_LIBRARIES} ${Boost_LIBRARIES} )
target_link_libraries ( ${CMAKE_PROJECT_NAME} corpuscle-renderer ) 