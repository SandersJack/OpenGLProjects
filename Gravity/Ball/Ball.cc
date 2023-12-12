#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <vector>
#include <random>
#include <algorithm>
#include <map>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
//GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

#include "LoadShaders.hh"
#include "Controls.hh"
#include "OGLManager.hh"
#include "Shape3D.hh"
#include "Text2D.hh"



GLuint ShapeshaderProgram;
GLuint SliderBoxProgram;
GLuint VAO_slider_container;
Shape3D *shapeTools;

// CPU representation of a particle
struct Ball{
	glm::vec3 pos, speed;
	unsigned char r,g,b,a; // Color
	float size, angle, weight;
	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f
	float radius;
};



bool checkBallCollision(const Ball& ball1, const Ball& ball2) {
    // Calculate the distance between the centers of the circles
    float distance = std::sqrt((ball1.pos.x - ball2.pos.x) * (ball1.pos.x - ball2.pos.x) +
                               (ball1.pos.y - ball2.pos.y) * (ball1.pos.y - ball2.pos.y));

    // Check if the distance is less than the sum of the radii
    return distance < (ball1.radius + ball2.radius);
}

// Function to handle elastic collision between two circles
void handleElasticCollision(Ball& ball1, Ball& ball2) {
    float dx = ball2.pos.x - ball1.pos.x;
    float dy = ball2.pos.y - ball1.pos.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    // Normalized collision vector
    float nx = dx / distance;
    float ny = dy / distance;

    // Relative velocity components
    float relativeVelocityX = ball2.speed.x - ball1.speed.x;
    float relativeVelocityY = ball2.speed.x - ball1.speed.y;

    // Calculate relative velocity in the direction of the collision
    float relativeSpeed = relativeVelocityX * nx + relativeVelocityY * ny;

    // If circles are moving towards each other
    if (relativeSpeed < 0) {
        // Apply elastic collision formula
        float impulse = (2.0f * relativeSpeed) / (1.0f / ball1.radius + 1.0f / ball2.radius);

        // Update velocities
        ball1.speed.x += impulse / ball1.radius * nx;
        ball1.speed.y += impulse / ball1.radius * ny;
        ball2.speed.x -= impulse / ball2.radius * nx;
        ball2.speed.y  -= impulse / ball2.radius * ny;
    }
}

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

float damping = 0.9f;
float gravity = -9.81f;
bool isDragging = false;
double lastMouseX = 0.0;

bool simulationPause = false;
bool buttonPress = false;
bool buttonreleased = true;

const float lowerLimit_damp = 0.0f;  // Set your lower limit
const float upperLimit_damp = 1.0f;   // Set your upper limit

const float lowerLimit_grav = -10.0f;  // Set your lower limit
const float upperLimit_grav = 0.0f;   // Set your upper limit

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            isDragging = true;
            glfwGetCursorPos(window, &lastMouseX, NULL);
        } else if (action == GLFW_RELEASE) {
            isDragging = false;
        }
    }

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		if (xpos > 30 && xpos < 170 && ypos > 120 && ypos < 180){
			simulationPause = !simulationPause;
		}
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (isDragging) {
		float lowerLimit, upperLimit;
		//std::cout << xpos << " " << ypos << std::endl;
		if(ypos > 90 && ypos < 110) {
			lowerLimit = lowerLimit_damp;
			upperLimit = upperLimit_damp;
			double deltaX = xpos - lastMouseX;
			damping += static_cast<float>(deltaX) * 0.01f; // Adjust the sensitivity as needed
			if (damping < lowerLimit) {
				damping = lowerLimit;
			} else if (damping > upperLimit) {
				damping = upperLimit;
			}
			lastMouseX = xpos;
		} else if (ypos > 30 && ypos < 50){
			lowerLimit = lowerLimit_grav;
			upperLimit = upperLimit_grav;
			double deltaX = xpos - lastMouseX;
			gravity += static_cast<float>(deltaX) * 0.01f; // Adjust the sensitivity as needed
			if (gravity < lowerLimit) {
				gravity = lowerLimit;
			} else if (gravity > upperLimit) {
				gravity = upperLimit;
			}
			lastMouseX = xpos;
		}
    }
	if(buttonPress){
		
	}
	
}

GLuint modelLocation;
GLuint colorLocation;
GLuint vao_slider;

GLuint sliderShaderProgram;

void init_slider() {

	sliderShaderProgram = LoadShaders( "Shaders/slider.vs", "Shaders/slider.fs" );

	// Use the shader program
    glUseProgram(sliderShaderProgram);

    // Set uniform locations
    modelLocation = glGetUniformLocation(sliderShaderProgram, "model");
    colorLocation = glGetUniformLocation(sliderShaderProgram, "color");

    // Set initial model matrix and color uniforms
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &modelMatrix[0][0]);

    glm::vec3 sliderColor = glm::vec3(0.2f, 0.2f, 0.8f);
    glUniform3fv(colorLocation, 1, &sliderColor[0]);

    // Set up vertex array and buffer
    glGenVertexArrays(1, &vao_slider);
    glBindVertexArray(vao_slider);

    GLfloat vertices[] = {
        0.0f, -0.1f, -25.f,
        1.0f, -0.1f, -25.f,
        1.0f,  0.1f, -25.f,
        0.0f,  0.1f, -25.f
    };

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void drawSlider(float varible, glm::vec3 startpos, float min, float max) {
    // Set the shader program
    glUseProgram(sliderShaderProgram);

	if(max > 1)
		varible /= max/2;

	glm::vec3 pos = startpos;
	pos.x = varible - pos.x - min;

    // Set the model matrix (for transformations)
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), pos);
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &modelMatrix[0][0]);

    // Set the color (you can update this based on your requirements)
    glm::vec3 sliderColor = glm::vec3(0.2f, 0.2f, 0.8f);
    glUniform3fv(colorLocation, 1, &sliderColor[0]);

    // Draw the slider using two triangles to form a quad
    glBindVertexArray(vao_slider);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // Change to GL_TRIANGLE_FAN
    glBindVertexArray(0);

    // Unset the shader program after rendering
    glUseProgram(0);
}

GLFWwindow* sliderWindow;

void initSliderWindow() {

    // Create a GLFW window for the slider
    sliderWindow = glfwCreateWindow(400, 200, "Slider Window", NULL, NULL);
    if (!sliderWindow) {
        std::cerr << "Failed to create GLFW slider window" << std::endl;
        glfwTerminate();
        return;
    }

	glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW for slider window" << std::endl;
        glfwTerminate();
        return;
    }

	// Set the position of the slider window
    glfwSetWindowPos(sliderWindow, 1200, 200);

    // Make the window's context current
    glfwMakeContextCurrent(sliderWindow);

    // Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Set GLFW mouse button callback
    glfwSetMouseButtonCallback(sliderWindow, mouse_button_callback);
    // Set GLFW cursor position callback
    glfwSetCursorPosCallback(sliderWindow, cursor_position_callback);
}

void cleanupSliderWindow() {
    glfwDestroyWindow(sliderWindow);
    glfwTerminate();
}

GLuint textProgram2D;
Text2D *text = Text2D::GetInstance();


void drawSliderWindow() {
	glfwMakeContextCurrent(sliderWindow);
	glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	drawSlider(damping, glm::vec3(1.0,0.0,0.0), 0, 1);
    glUseProgram(textProgram2D);
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(400), 0.0f, static_cast<float>(200));
    glUniformMatrix4fv(glGetUniformLocation(textProgram2D, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glm::mat4 projection2 = glm::ortho(0.0f, static_cast<float>(400), 0.0f, static_cast<float>(200));

	std::string str = "Damping: " + std::to_string(damping);
	str = str.substr(0, str.find(".")+3);
    text->RenderText(str, -.99f, 0.2f, 0.004f, glm::vec3(0.5, 0.8f, 0.2f));
	std::string str2 = "Gravity: " + std::to_string(gravity);

	str2 = str2.substr(0, str.find(".")+3);
	text->RenderText(str2, -.99f, 0.8f, 0.004f, glm::vec3(0.5, 0.8f, 0.2f));
	drawSlider(gravity, glm::vec3(0.0,0.6,0.0), 0, 20);

	glm::vec4 button_colour;
	if (simulationPause)
		button_colour = glm::vec4(1.0, .0, .0, 1.0);
	else 
		button_colour = glm::vec4(1.0, 1.0, 1.0, 1.0);

	shapeTools->render3DShape(SliderBoxProgram, VAO_slider_container, projection2, glm::vec3(100.0f, 50.0f, 0.003f), 12, button_colour);
	text->RenderText("Pause", -.77f, -0.58f, 0.004f, glm::vec3(0.5, 0.8f, 0.2f));
	checkGLError("SliderBox");

	glfwSwapBuffers(sliderWindow);
	glfwPollEvents();   
}



template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

int main() {

	// Use a random device to seed the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> Distribution_11(-1.0f, 1.0f);
	std::uniform_real_distribution<float> Distribution_1010(-10.f, 10.f);

    int SCR_WIDTH = 1024;
    int SCR_HEIGHT = 768;

	OGLManager *oglM = OGLManager::GetInstance();
	oglM->Init(SCR_WIDTH, SCR_HEIGHT);

	GLFWwindow *window = oglM->GetWindow();

	shapeTools = Shape3D::GetInstance();

	// Create shader program
    ShapeshaderProgram = LoadShaders( "Shaders/border.vs", "Shaders/border.fs" );

	/// Vertex Array Object (VAO)
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
	GLfloat *quadvertices = new GLfloat[4 * 3];

	shapeTools->generateQuadVertices(quadvertices, 1.30f, 1.0f, 0.0f);

	GLfloat vertices[12];
	for(int i; i<12; i++){
		vertices[i] = quadvertices[i];
	}

	// Scale factor
	float scaleFactor = 28.0f;

	// Scale the rectangle by multiplying each coordinate by the scaleFactor
	for (int i = 0; i < 12; ++i) {
		vertices[i] *= scaleFactor;
	}

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	

    // Vertex Attribute Pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

	
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
	
	// Set the radius and the number of points
    float radius = 1.f;
    int numPoints = 100;  // You can adjust the number of points

    // Allocate memory for the circle vertices
    GLfloat* g_vertex_buffer_data = new GLfloat[numPoints * 3];

    // Generate circle vertices
    shapeTools->generateCircleVertices(g_vertex_buffer_data, radius, numPoints, 0.0);

	GLuint b_vertex_buffer;
	glGenBuffers(1, &b_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, b_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, (numPoints) * 3 * sizeof(GLfloat), g_vertex_buffer_data, GL_STATIC_DRAW);

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

    initSliderWindow();
	init_slider();

    textProgram2D = LoadShaders( "Shaders/text.vs", "Shaders/text.fs" );
    text->initFreeType("fonts/Roboto-Regular.ttf",48);
    text->SetShaderProgram(textProgram2D);


    SliderBoxProgram = LoadShaders( "Shaders/slider_box.vs", "Shaders/slider_box.fs" );

    ////// Slider container
    /// Vertex Array Object (VAO)
    glGenVertexArrays(1, &VAO_slider_container);
    glBindVertexArray(VAO_slider_container);
	GLfloat *slide_cont_vertices = new GLfloat[4 * 3];

	shapeTools->generateQuadVertices(slide_cont_vertices, 5.f, 2.f, 0.0f);

	GLfloat slide_vertices[12];
	for(int i; i<12; i++){
		slide_vertices[i] = slide_cont_vertices[i];
	}

	// Scale the rectangle by multiplying each coordinate by the scaleFactor
	for (int i = 0; i < 12; ++i) {
		slide_vertices[i] *= scaleFactor;
	}

    GLuint VBO_slider_box;
    glGenBuffers(1, &VBO_slider_box);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_slider_box);
    glBufferData(GL_ARRAY_BUFFER, sizeof(slide_vertices), slide_vertices, GL_STATIC_DRAW);
	
	// Vertex Attribute Pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

	
	glm::vec3 halfboundsize = glm::vec3(0.65 * 28 - 2*radius, 0.5 * 28 - 2*radius, 0.0f); 

	checkGLError("control");
	int spawnedBalls = 0;
    int q = 0;
	do {
		glfwMakeContextCurrent(window);
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

    
		control->computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = control->getProjectionMatrix();
		glm::mat4 ViewMatrix = control->getViewMatrix();

		glm::vec3 CameraPosition(glm::inverse(ViewMatrix)[3]);
		glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

		glm::mat4 ViewProjectionMatrix2 = ProjectionMatrix * ViewMatrix;
        
		shapeTools->render3DShape(ShapeshaderProgram, VAO, ViewProjectionMatrix, glm::vec3(0.0f, 0.0f, -30.0f), 12, glm::vec4(1.0, 1.0, 1.0, 1.0)); // White color;
		
		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		lastTime = currentTime;
		
		int newBalls = maxnumBalls;
		if(spawnedBalls < maxnumBalls) {
			for(int i=0; i<newBalls; i++){

				glm::vec3 maindir = glm::vec3(Distribution_1010(gen), 5.0f, 0.0f);

				//double rand_01 = ((double) rand() / (RAND_MAX));
				BallContainer[i].pos = glm::vec3(Distribution_11(gen) * halfboundsize.x, Distribution_11(gen) * halfboundsize.y,-29.9f);

				BallContainer[i].speed = maindir;

				BallContainer[i].r = rand() % 256;
				BallContainer[i].g = rand() % 256;
				BallContainer[i].b = rand() % 256;
				BallContainer[i].a = 255;

				BallContainer[i].radius = radius*2;

				BallContainer[i].size = 2.f;

				spawnedBalls++;
			}
		}

		// Simulate all particles
		int BallCount = 0;
		for(int i=0; i<maxnumBalls; i++){
			Ball& p = BallContainer[i];
			
			if(!simulationPause) {
				// Check bounding box in y
				if(abs(p.pos.y) > halfboundsize.y){
					p.pos.y = halfboundsize.y * sgn(p.pos.y);
					p.speed.y *= -1 * damping;
				}

				// Check bounding box in x
				if(abs(p.pos.x) > (halfboundsize.x)){
					p.pos.x = (halfboundsize.x) * sgn(p.pos.x);
					p.speed.x *= -1 * damping;
				}

				/// Check collsions on all other balls
				if(true){
					for(int t=0; t<maxnumBalls; t++){
						if(i == t)
							continue;
						Ball& p2 = BallContainer[t];
						if(checkBallCollision(p, p2)){
							handleElasticCollision(p,p2);
						}
					}
				}

				// Simulate simple physics : gravity only, no collisions
				p.speed += glm::vec3(0.0f,gravity, 0.0f) * (float)delta * 0.5f;
				p.pos += p.speed * (float)delta;
				p.cameradistance = glm::length2( p.pos - CameraPosition );
			}

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
		glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, numPoints, BallCount);
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		
		
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
		
		glClear(GL_COLOR_BUFFER_BIT);
		drawSliderWindow();
		checkGLError("slide");
		
	} 
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   (glfwWindowShouldClose(window) == 0  && !glfwWindowShouldClose(sliderWindow)));
	
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

	cleanupSliderWindow();
    
	return -1;
}