#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "OPGLTools.hh"

#include "Global.hh"

#include "GameManager.hh"
#include "ResourceManager.hh"
#include "SpriteRenderer.hh"



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

OGLManager *oglM;
GameManager *gameMan;
ResourceManager *resourceMan;

#include "MatrixTransform.hh"

int main(){

    uint SCR_WIDTH = 800;
    uint SCR_HEIGHT = 600;

    /// OPENGl Window Init
    /*
	oglM = OGLManager::GetInstance();
    oglM->SetWindowColour(Vector4(0.0f, 0.0f, 0.4f, 0.0f));
	oglM->Init(SCR_WIDTH, SCR_HEIGHT, "Breakout");
	GLFWwindow *window = oglM->GetWindow();
    */
    if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		exit(1);
	}

    glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE,false);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Breakout", NULL, NULL);
    if ( window == NULL ) {
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		exit(2);
    }

	glfwMakeContextCurrent(window);
    // Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		exit(3);
	}

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // OpenGL configuration
    // --------------------
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /// Game Init
    gameMan = GameManager::GetInstance();
    gameMan->SetWidthHeight(SCR_WIDTH, SCR_HEIGHT);
    gameMan->Init();

    /// Resourse manager init
    resourceMan = ResourceManager::GetInstance();

    /// Setting up camera
    Matrix4 projection = Ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);  


    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        gameMan->ProcessInput(deltaTime);

        gameMan->Update(deltaTime);

        glClear(GL_COLOR_BUFFER_BIT);
        gameMan->Render();

        glfwSwapBuffers(window);

        checkGLError("Control");
    }

    resourceMan->Clear();

    glfwTerminate();

    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            gameMan->fKeys[key] = true;
        else if (action == GLFW_RELEASE)
            gameMan->fKeys[key] = false;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}