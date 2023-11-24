cmake_minimum_required(VERSION 3.2)

#find_package(OpenGL REQUIRED)
#find_package(GLEW REQUIRED)
#find_package(GLFW REQUIRED)

project(Starter)

file(GLOB SOURCES src/*.cc)
file(GLOB headers src/*.hh)

include_directories(include)

add_executable(Starter Starter.cc ${SOURCES})


find_package(OpenGL REQUIRED OPTIONAL_COMPONENTS OpenGL)


find_package(GLEW REQUIRED)
#target_link_libraries(Starter GLEW::GLEW)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::OpenGL GLEW::GLEW ${GLFW_LIBRARIES})