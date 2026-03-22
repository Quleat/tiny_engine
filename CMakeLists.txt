cmake_minimum_required(VERSION 3.15)
project(tiny_engine)

set(CMAKE_CXX_STANDARD 20)

#-------EXECUTABLE----------------------------
add_executable(${PROJECT_NAME} main.cpp)

#--------OpenGL-------------------------------
set(OPENGL_gl_LIBRARY "/usr/lib64/libOpenGL.so" CACHE FILEPATH "Path to OpenGL Library")
find_package(OpenGL REQUIRED COMPONENTS OpenGL)


#--------GLFW (windows)------------------------
if(EXISTS third-party/glfw/CMakeLists.txt)
	message("Using glfw from the third-party folder")
	add_subdirectory(third-party/glfw)

	set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
else()
	message("Using packaged glfw")
	find_package(glfw3 REQUIRED)
endif()

#--------GLAD (binding the opengl functions----

add_library(glad STATIC third-party/glad/src/glad.c)
target_include_directories(glad PUBLIC third-party/glad/include)
target_link_libraries(glad PRIVATE OpenGL::OpenGL)

#---------LINKING LIBRARIES---------------------

target_link_libraries(${PROJECT_NAME} PRIVATE
	glfw
	glad
	OpenGL::OpenGL
)
