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
    Matrix4 test;
    Matrix4 test2 = Translate(test, Vector3(0.,0.,0.));


    uint SCR_WIDTH = 800;
    uint SCR_HEIGHT = 600;
    /// OPENGl Window Init
	oglM = OGLManager::GetInstance();
    oglM->SetWindowColour(Vector4(0.0, 0.0, 0.0, 0.0));
	oglM->Init(SCR_WIDTH, SCR_HEIGHT, "Breakout");
	GLFWwindow *window = oglM->GetWindow();

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
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        gameMan->ProcessInput(deltaTime);

        gameMan->Update(deltaTime);

        glClear(GL_COLOR_BUFFER_BIT);
        gameMan->Render();

        glfwSwapBuffers(window);
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