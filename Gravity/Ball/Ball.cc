#include <stdio.h>
#include <stdlib.h>
#include <iostream>

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

const int maxnumBalls = 1;
Ball BallContainer[maxnumBalls];

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

	static GLfloat* g_balls_position_size_data = new GLfloat[maxnumBalls * 4];
	static GLubyte* g_balls_color_data         = new GLubyte[maxnumBalls * 4];

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
	GLuint balls_position_buffer;
	glGenBuffers(1, &balls_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, balls_position_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, maxnumBalls * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// The VBO containing the colors of the particles
	GLuint balls_color_buffer;
	glGenBuffers(1, &balls_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, balls_color_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, maxnumBalls * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

    double lastTime = glfwGetTime();

    Controls *control = Controls::GetInstance();
	control->SetWindow(window);

	int spawnedBalls = 0;
	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		lastTime = currentTime;
		
		control->computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = control->getProjectionMatrix();
		glm::mat4 ViewMatrix = control->getViewMatrix();

		glm::vec3 CameraPosition(glm::inverse(ViewMatrix)[3]);
		glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

		int newBalls = 1;
		if(spawnedBalls < maxnumBalls) {
			for(int i=0; i<newBalls; i++){

				glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f);

				
				BallContainer[i].pos = glm::vec3(0,0,-20.0f);

				BallContainer[i].speed = maindir;

				BallContainer[i].r = rand() % 256;
				BallContainer[i].g = rand() % 256;
				BallContainer[i].b = rand() % 256;
				BallContainer[i].a = 255;

				BallContainer[i].size = 5.f;

				spawnedBalls++;

				std::cout << "Spawned" << std::endl;
			}
		}


		// Simulate all particles
		int BallCount = 0;
		for(int i=0; i<maxnumBalls; i++){
			Ball& p = BallContainer[i];

			// Simulate simple physics : gravity only, no collisions
			p.speed += glm::vec3(0.0f,-9.81f, 0.0f) * (float)delta * 0.5f;
			p.pos += p.speed * (float)delta;
			p.cameradistance = glm::length2( p.pos - CameraPosition );

			//std::cout << glm::length(p.pos) << std::endl;
			// Fill the GPU buffer
			g_balls_position_size_data[4*BallCount+0] = p.pos.x;
			g_balls_position_size_data[4*BallCount+1] = p.pos.y;
			g_balls_position_size_data[4*BallCount+2] = p.pos.z;
											
			g_balls_position_size_data[4*BallCount+3] = p.size;
											
			g_balls_color_data[4*BallCount+0] = p.r;
			g_balls_color_data[4*BallCount+1] = p.g;
			g_balls_color_data[4*BallCount+2] = p.b;
			g_balls_color_data[4*BallCount+3] = p.a;

			BallCount++;
		}


		glBindBuffer(GL_ARRAY_BUFFER, balls_position_buffer);
		glBufferData(GL_ARRAY_BUFFER, maxnumBalls * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); 
		glBufferSubData(GL_ARRAY_BUFFER, 0, BallCount * sizeof(GLfloat) * 4, g_balls_position_size_data);

		glBindBuffer(GL_ARRAY_BUFFER, balls_color_buffer);
		glBufferData(GL_ARRAY_BUFFER, maxnumBalls * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, BallCount * sizeof(GLubyte) * 4, g_balls_color_data);


        glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Use our shader
		glUseProgram(programID);

		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID, 0);

		glUniform3f(CameraRight_worldspace_ID, ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
		glUniform3f(CameraUp_worldspace_ID   , ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);

        glUniformMatrix4fv(ViewProjMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);


		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, b_vertex_buffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

        // 2nd attribute buffer : positions of particles' centers
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, balls_position_buffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			4,                                // size : x + y + z + size => 4
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : particles' colors
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, balls_color_buffer);
		glVertexAttribPointer(
			2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			4,                                // size : r + g + b + a => 4
			GL_UNSIGNED_BYTE,                 // type
			GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
		glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
		glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

		// Draw the particules !
		// This draws many times a small triangle_strip (which looks like a quad).
		// This is equivalent to :
		// for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4), 
		// but faster.
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, BallCount);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	
	} 
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	delete[] g_balls_position_size_data;

	// Cleanup VBO and shader
	glDeleteBuffers(1, &balls_color_buffer);
	glDeleteBuffers(1, &balls_position_buffer);
	glDeleteBuffers(1, &b_vertex_buffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);
	

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
    
	return -1;
}