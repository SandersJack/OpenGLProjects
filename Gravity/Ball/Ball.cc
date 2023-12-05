#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <vector>
#include <random>
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

const int maxnumBalls = 10;
Ball BallContainer[maxnumBalls];

const char* glErrorToString(GLenum error) {
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

void checkGLError(const char* functionName) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        const char* errorString = glErrorToString(error);
        std::cerr << "OpenGL error after " << functionName << ": " << error << " (" << errorString << ")" << std::endl;
		exit(1);
    }
}


// Function to render the 3D shape
void render3DShape(GLuint shaderProgram, GLuint VAO, const glm::mat4& modelViewProjection, const glm::vec3& shapePosition, 
	GLuint vertexSize) {
    
	glUseProgram(shaderProgram);
	checkGLError("glUseProgram");

    // Set the shape position using a uniform variable
    GLuint shapePositionLocation = glGetUniformLocation(shaderProgram, "shapePosition");
    glUniform3fv(shapePositionLocation, 1, &shapePosition[0]);
	checkGLError("glUniform3fv");

    // Set the model-view-projection matrix
    GLuint mvpLocation = glGetUniformLocation(shaderProgram, "modelViewProjection");
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &modelViewProjection[0][0]);
	checkGLError("glUniformMatrix4fv");

    glBindVertexArray(VAO);
	checkGLError("glBindVertexArray");

    // Draw the 3D shape
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexSize/3);  // Assuming a simple quad for this example
	checkGLError("glDrawArrays");

    glBindVertexArray(0);
	checkGLError("glBindVertexArray");

    // Check for OpenGL errors after drawing
    //checkGLError("render3DShape");
}

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}


int main() {

	// Use a random device to seed the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> Distribution_11(-1.0f, 1.0f);

	OGLManager *oglM = OGLManager::GetInstance();
	oglM->Init(1024, 768);

	GLFWwindow *window = oglM->GetWindow();

	// Create shader program
    GLuint ShapeshaderProgram = LoadShaders( "Shaders/border.vs", "Shaders/border.fs" );

	/// Vertex Array Object (VAO)
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertex Buffer Object (VBO)
	/* square
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
    };
	*/ 
	GLfloat vertices[] = {
        -.65f, -0.5f, 0.0f,
		.65f, -0.5f, 0.0f,
		-.65f,  0.5f, 0.0f,
		.65f,  0.5f, 0.0f,
    };
	// Scale factor
	float scaleFactor = 28.0f;

	// Scale the rectangle by multiplying each coordinate by the scaleFactor
	for (int i = 0; i < sizeof(vertices) / sizeof(vertices[0]); ++i) {
		vertices[i] *= scaleFactor;
	}

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	

    // Vertex Attribute Pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
	checkGLError("glVertexAttribPointer");

	
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
	
	control->SetEnableCursor(false);
	control->SetEnableUpDown(false);
	control->SetEnableLeftRight(false);
	
	double damping = 0.9;
	double halfboundsize = 0.5 * 28 - 0.5 * 2;

	checkGLError("control");
	int spawnedBalls = 0;
	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glEnable(GL_DEPTH_TEST);
		

		control->computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = control->getProjectionMatrix();
		glm::mat4 ViewMatrix = control->getViewMatrix();

		glm::vec3 CameraPosition(glm::inverse(ViewMatrix)[3]);
		glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

		glm::mat4 ViewProjectionMatrix2 = ProjectionMatrix * ViewMatrix;
		checkGLError("Before");

		render3DShape(ShapeshaderProgram, VAO, ViewProjectionMatrix, glm::vec3(0.0f, 0.0f, -30.0f), sizeof(vertices));
		checkGLError("Before2");
		
		
		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		lastTime = currentTime;
		
		

		
		int newBalls = maxnumBalls;
		if(spawnedBalls < maxnumBalls) {
			for(int i=0; i<newBalls; i++){

				glm::vec3 maindir = glm::vec3(0.0f, 5.0f, 0.0f);

				//double rand_01 = ((double) rand() / (RAND_MAX));
				BallContainer[i].pos = glm::vec3(Distribution_11(gen) * halfboundsize, Distribution_11(gen) * halfboundsize,-29.9f);

				BallContainer[i].speed = maindir;

				BallContainer[i].r = rand() % 256;
				BallContainer[i].g = rand() % 256;
				BallContainer[i].b = rand() % 256;
				BallContainer[i].a = 255;

				BallContainer[i].size = 2.f;

				spawnedBalls++;

				std::cout << "Spawned" << std::endl;
			}
		}

		// Simulate all particles
		int BallCount = 0;
		for(int i=0; i<maxnumBalls; i++){
			Ball& p = BallContainer[i];
			
			
			
			// Check bounding box in y
			if(abs(p.pos.y) > halfboundsize){
				p.pos.y = halfboundsize * sgn(p.pos.y);
				p.speed *= -1 * damping;
			}

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

		glBindVertexArray(VertexArrayID);

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

	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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