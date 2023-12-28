#include "OGLManager.hh"

#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <algorithm>

#include <GL/glew.h>

#include <GLFW/glfw3.h>


OGLManager *OGLManager::fInstance = nullptr;

OGLManager *OGLManager::GetInstance() {
    if(!fInstance)
        fInstance = new OGLManager();
    return fInstance;
}

OGLManager::OGLManager() : fWindowResize(false), fWindowColor(Vector4(0.0f, 0.0f, 0.4f, 0.0f)){

}

OGLManager::~OGLManager(){

}

void OGLManager::Init(int windowX, int windowY, const char* name){

    fWindowX = windowX;
    fWindowY = windowY;
    // Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		exit(1);
	}

    glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE,fWindowResize);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
	fWindow = glfwCreateWindow( fWindowX, fWindowY, name, NULL, NULL);

    if( fWindow == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		exit(2);
	}
	glfwMakeContextCurrent(fWindow);
    // Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		exit(3);
	}

    // Ensure we can capture the escape key being pressed below
	glfwSetInputMode(fWindow, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    //glfwSetInputMode(fWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Set the mouse at the center of the screen
    glfwPollEvents();
    //glfwSetCursorPos(fWindow, 1024/2, 768/2);

    // Dark blue background
	glClearColor(fWindowColor.x, fWindowColor.y, fWindowColor.z, fWindowColor.w);

    // Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
    
}