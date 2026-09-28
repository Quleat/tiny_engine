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

if(NOT WIN32)
	message("Compiling for linux")
	set(OPENGL_gl_LIBRARY "/usr/lib64/libOpenGL.so" CACHE FILEPATH "Path to OpenGL Library")
else()
	message("Compiling for Windows")
endif()
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
target_link_libraries(glad PRIVATE OpenGL::GL)

#--------GLM (math functions)---------------------------

target_include_directories(${PROJECT_NAME} PRIVATE third-party/glm)

#--------STB_IMAGE (loading images)--------------------

target_include_directories(${PROJECT_NAME} PRIVATE third-party/stb_image)
target_sources(${PROJECT_NAME} PRIVATE third-party/stb_image/stb_image_impl.cpp)
#set flag so we don't see the warnings from there 
set_source_files_properties(third-party/stb_image/stb_image_impl.cpp PROPERTIES COMPILE_OPTIONS "-w")

#--------ASSIMP (loading models)------------------------

set(ASSIMP_BUILD_TEST OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
add_subdirectory(
	"${CMAKE_SOURCE_DIR}/third-party/assimp"
	"${CMAKE_BINARY_DIR}/third-party/assimp"
	)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp::assimp)

#---------LINKING LIBRARIES-----------------------------

target_link_libraries(${PROJECT_NAME} PRIVATE
	glfw
	glad
	OpenGL::GL
)

#---------SETTING FLAGS DEPENDING ON THE BUILD TYPE------

target_compile_options(tiny_engine PRIVATE -O0 -g -Wall -Wpedantic -Wextra -Wshadow -Wdouble-promotion)

#----------COPYING SHADERS/TEXTURES/MODELS FOLDERS-------

add_custom_command(
    TARGET tiny_engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/shaders"
            "$<TARGET_FILE_DIR:tiny_engine>/shaders"
)
