cmake_minimum_required(VERSION 3.15)
project(tiny_engine)

set(CMAKE_CXX_STANDARD 20)

#-------EXECUTABLE----------------------------
add_executable(${PROJECT_NAME} main.cpp)

#--------OpenGL-------------------------------
set(OPENG_gl_LIBRARY /usr/lib64/libGL.so)
set(OPENGL_INCLUDE_DIR /usr/include)
find_package(OpenGL REQUIRED)

#--------GLFW (windows)------------------------
add_subdirectory(third-party/glfw)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

#--------GLAD (binding the opengl functions----

add_library(glad STATIC third-party/glad/src/gl.c)
target_include_directories(glad PUBLIC third-party/glad/include)
target_link_libraries(glad PRIVATE OpenGL:GL)

#---------LINKING LIBRARIES---------------------

target_link_libraries(${PROJECT_NAME} PRIVATE
	glfw
	glad
	OpenGL:GL
)
