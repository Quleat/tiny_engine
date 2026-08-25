cmake_minimum_required(VERSION 3.15)
project(tiny_engine)
set(CMAKE_CXX_STANDARD 20)
#-------EXECUTABLE----------------------------
set(SOURCE_FILES "src/")
set(INCLUDE_FILES "include/")
add_executable(${PROJECT_NAME} 
    ${SOURCE_FILES}shader.cpp
    ${SOURCE_FILES}mesh.cpp
	${SOURCE_FILES}main.cpp
    ${SOURCE_FILES}model.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_FILES})

#--------OpenGL-------------------------------
set(OPENGL_gl_LIBRARY "/usr/lib64/libOpenGL.so" CACHE FILEPATH "Path to OpenGL Library")
find_package(OpenGL REQUIRED COMPONENTS OpenGL)

#--------GLFW (windows)---------------------------------
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/third-party/glfw/CMakeLists.txt)
	message("Using glfw from the third-party folder")
	add_subdirectory(third-party/glfw)

	set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
else()
	message("Using packaged glfw")
	find_package(glfw3 REQUIRED)
endif()

#--------GLAD (binding the opengl functions)------------

add_library(glad STATIC third-party/glad/src/glad.c)
target_include_directories(glad PUBLIC third-party/glad/include)
target_link_libraries(glad PRIVATE OpenGL::OpenGL)

#--------GLM (math functions)---------------------------

target_include_directories(${PROJECT_NAME} PRIVATE third-party/glm)

#--------STB_IMAGE (loading images)--------------------

target_include_directories(${PROJECT_NAME} PRIVATE third-party/stb_image)
target_sources(${PROJECT_NAME} PRIVATE third-party/stb_image/stb_image_impl.cpp)
#set flag so we don't see the warnings from there 
set_source_files_properties(third-party/stb_image/stb_image_impl.cpp PROPERTIES COMPILE_OPTIONS "-w")

#--------ASSIMP (loading models)------------------------

add_library(assimp SHARED IMPORTED)

set_target_properties(assimp PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/third-party/assimp/libassimp.so"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/third-party/assimp/include"
)

target_link_libraries(${PROJECT_NAME} PRIVATE assimp)

#target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third-party/assimp/include)

#---------LINKING LIBRARIES-----------------------------

target_link_libraries(${PROJECT_NAME} PRIVATE
	glfw
	glad
	OpenGL::OpenGL
)

#---------SETTING FLAGS DEPENDING ON THE BUILD TYPE------

target_compile_options(tiny_engine PRIVATE -O0 -g -pg -Wall -Wpedantic -Wextra -Wshadow -Wdouble-promotion)
