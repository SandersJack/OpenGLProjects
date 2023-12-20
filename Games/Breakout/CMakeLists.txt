cmake_minimum_required(VERSION 3.2)

project(Breakout)

file(GLOB SOURCES src/*.cc)
file(GLOB headers src/*.hh)

include_directories(include)

add_executable(Breakout Breakout.cc ${SOURCES})

########################### OPGLTools ########################################
# Specify the relative path to the Controls library build directory
set(LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../OPGLTools/build)
# Link against the library
target_link_libraries(Breakout PRIVATE ${LIBRARY_DIR}/libOPGLTools.a)
# Specify the relative include directory for Controls library
target_include_directories(Breakout PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../OPGLTools/include)
########################### MathGL ########################################
# Specify the relative path to the Controls library build directory
set(LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../OPGLTools/build/MathGL)
# Link against the library
target_link_libraries(Breakout PRIVATE ${LIBRARY_DIR}/libMathGL.so)
# Specify the relative include directory for Controls library
target_include_directories(Breakout PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../OPGLTools/MathGL/include)
########################### Controls ########################################
# Specify the relative path to the Controls library build directory
set(LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../OPGLTools/build/Controls)
# Link against the library
target_link_libraries(Breakout PRIVATE ${LIBRARY_DIR}/libControls.so)
# Specify the relative include directory for Controls library
target_include_directories(Breakout PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../OPGLTools/Controls/include)


find_package(OpenGL REQUIRED OPTIONAL_COMPONENTS OpenGL)


find_package(GLEW REQUIRED)
#target_link_libraries(Starter GLEW::GLEW)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::OpenGL GLEW::GLEW ${GLFW_LIBRARIES})

find_package(Freetype REQUIRED)
target_link_libraries(Breakout PRIVATE Freetype::Freetype)
