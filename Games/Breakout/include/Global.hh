#ifndef Global_HPP
#define Global_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

typedef unsigned int uint;

static const char* glErrorToString(GLenum error) {
    switch (error) {
        case GL_NO_ERROR:
            return "GL_NO_ERROR";
        case GL_INVALID_ENUM:
            return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:
            return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION:
            return "GL_INVALID_OPERATION";
        case GL_STACK_OVERFLOW:
            return "GL_STACK_OVERFLOW";
        case GL_STACK_UNDERFLOW:
            return "GL_STACK_UNDERFLOW";
        case GL_OUT_OF_MEMORY:
            return "GL_OUT_OF_MEMORY";
        // Add more error codes as needed
        default:
            return "Unknown OpenGL Error";
    }
}

static void checkGLError(const char* functionName) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        const char* errorString = glErrorToString(error);
        std::cerr << "OpenGL error after " << functionName << ": " << error << " (" << errorString << ")" << std::endl;
		exit(1);
    }
}

#endif