#ifndef Controls_HPP
#define Controls_HPP

#include <GLFW/glfw3.h> 

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//using namespace glm;

class Controls
{
    private:
        static Controls *fInstance;

        GLFWwindow* fWindow;

        glm::vec3 fPosition; 
        float fHorizontalAngle;
        float fVerticalAngle;
        float fInitialFoV;

        float fSpeed;
        float fMouseSpeed;

        glm::mat4 fViewMatrix;
        glm::mat4 fProjectionMatrix;

        bool fEnabledCursor = true;
        bool fEnabledUpDown = true;
        bool fEnabledLeftRight = true;

    public:
        Controls(/* args */);
        ~Controls();

        static Controls *GetInstance();

        void computeMatricesFromInputs();

        glm::mat4 getViewMatrix(){
            return fViewMatrix;
        }
        glm::mat4 getProjectionMatrix(){
            return fProjectionMatrix;
        }

        void SetWindow(GLFWwindow* wind){ fWindow = wind;}

        void SetEnableCursor(bool val){fEnabledCursor = val;}
        void SetEnableUpDown(bool val){fEnabledUpDown = val;}
        void SetEnableLeftRight(bool val){fEnabledLeftRight = val;}

        
};

#endif