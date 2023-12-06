cmake_minimum_required(VERSION 3.2)

#find_package(OpenGL REQUIRED)
#find_package(GLEW REQUIRED)
#find_package(GLFW REQUIRED)

project(Ball)

file(GLOB SOURCES src/*.cc)
file(GLOB headers src/*.hh)

include_directories(include)

add_executable(Ball Ball.cc ${SOURCES})

########################### OPGLTools ########################################
# Specify the relative path to the Controls library build directory
set(LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../OPGLTools/build)
# Link against the library
target_link_libraries(Ball PRIVATE ${LIBRARY_DIR}/libOPGLTools.a)
# Specify the relative include directory for Controls library
target_include_directories(Ball PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../OPGLTools/include)
########################### Controls ########################################
# Specify the relative path to the Controls library build directory
set(LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../OPGLTools/build/Controls)
# Link against the library
target_link_libraries(Ball PRIVATE ${LIBRARY_DIR}/libControls.so)
# Specify the relative include directory for Controls library
target_include_directories(Ball PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../OPGLTools/Controls/include)


find_package(OpenGL REQUIRED OPTIONAL_COMPONENTS OpenGL)


find_package(GLEW REQUIRED)
#target_link_libraries(Starter GLEW::GLEW)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::OpenGL GLEW::GLEW ${GLFW_LIBRARIES})

find_package(Freetype REQUIRED)
target_link_libraries(Ball PRIVATE Freetype::Freetype)
