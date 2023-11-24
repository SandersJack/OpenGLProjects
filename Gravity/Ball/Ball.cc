#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <algorithm>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
//GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;

#include "LoadShaders.hh"
#include "Controls.hh"
#include "OGLManager.hh"

// CPU representation of a particle
struct Ball{
	glm::vec3 pos, speed;
	unsigned char r,g,b,a; // Color
	float size, angle, weight;
	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

};

int numBalls = 1;

int main() {

	OGLManager *oglM = OGLManager::GetInstance();
	oglM->Init(1024, 768);

	GLFWwindow *window = oglM->GetWindow();

	GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "Shaders/Ball.vs", "Shaders/Ball.fs" );

	// Vertex shader
	GLuint CameraRight_worldspace_ID  = glGetUniformLocation(programID, "CameraRight_worldspace");
	GLuint CameraUp_worldspace_ID  = glGetUniformLocation(programID, "CameraUp_worldspace");
	GLuint ViewProjMatrixID = glGetUniformLocation(programID, "VP");

    // fragment shader
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

	static GLfloat* g_particule_position_size_data = new GLfloat[numBalls * 4];
	static GLubyte* g_particule_color_data         = new GLubyte[numBalls * 4];

	static const GLfloat g_vertex_buffer_data[] = { 
		 -0.5f, -0.5f, 0.0f,
		  0.5f, -0.5f, 0.0f,
		 -0.5f,  0.5f, 0.0f,
		  0.5f,  0.5f, 0.0f,
	};

	GLuint b_vertex_buffer;
	glGenBuffers(1, &b_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, b_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // The VBO containing the positions and sizes of the particles
	GLuint particles_position_buffer;
	glGenBuffers(1, &particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, numBalls * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// The VBO containing the colors of the particles
	GLuint particles_color_buffer;
	glGenBuffers(1, &particles_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, numBalls * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

    double lastTime = glfwGetTime();

    Controls *control = Controls::GetInstance();
	control->SetWindow(window);

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		lastTime = currentTime;
	
	} 
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );


    
}