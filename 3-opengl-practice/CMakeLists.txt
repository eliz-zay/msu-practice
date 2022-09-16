cmake_minimum_required (VERSION 3.0)

set (CMAKE_CXX_STANDARD 17)

project (game)

find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)

add_subdirectory (external)

include(FindFreetype)

include_directories(
	external/glfw-3.1.2/include/
	external/glm-0.9.7.1/
	external/glew-1.13.0/include/
	external/glm-0.9.7.1/
	external/freetype-2.10.3
	${FREETYPE_INCLUDE_DIRS}
	external/stb_image/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW
	${FREETYPE_LIBRARIES}
)

file(GLOB src
	"src/main.cpp"
    "src/shader/*.hpp"
    "src/shader/*.cpp"
    "src/*.glsl"
)

add_executable(game
	${src}
)
target_link_libraries(game
	${ALL_LIBS}
)


