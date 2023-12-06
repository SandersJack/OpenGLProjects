cmake_minimum_required(VERSION 3.2)

project(OPGLTools)

file(GLOB SOURCES src/*.cc)
include_directories(include)
# Add your source files
add_library(OPGLTools STATIC ${SOURCES})

# Include directory for header files
target_include_directories(OPGLTools PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# Add the source and include directories
add_subdirectory(Controls)

# Include directory for header files
include_directories(Controls/include)
target_link_libraries(OPGLTools PRIVATE Controls)

find_package(OpenGL REQUIRED OPTIONAL_COMPONENTS OpenGL)


find_package(GLEW REQUIRED)
#target_link_libraries(Starter GLEW::GLEW)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::OpenGL GLEW::GLEW ${GLFW_LIBRARIES})

find_package(Freetype REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Freetype::Freetype)