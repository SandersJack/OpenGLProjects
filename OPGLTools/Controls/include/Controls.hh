#ifndef Controls_HPP
#define Controls_HPP

#include <GLFW/glfw3.h> 

#include "MathGL.hh"
//using namespace glm;

class Controls
{
    private:
        static Controls *fInstance;

        GLFWwindow* fWindow;

        Vector3 fPosition; 
        float fHorizontalAngle;
        float fVerticalAngle;
        float fInitialFoV;

        float fSpeed;
        float fMouseSpeed;

        Matrix4 fViewMatrix;
        Matrix4 fProjectionMatrix;

        bool fEnabledCursor = true;
        bool fEnabledUpDown = true;
        bool fEnabledLeftRight = true;

    public:
        Controls(/* args */);
        ~Controls();

        static Controls *GetInstance();

        void computeMatricesFromInputs();

        Matrix4 getViewMatrix(){
            return fViewMatrix;
        }
        Matrix4 getProjectionMatrix(){
            return fProjectionMatrix;
        }

        void SetWindow(GLFWwindow* wind){ fWindow = wind;}

        void SetEnableCursor(bool val){fEnabledCursor = val;}
        void SetEnableUpDown(bool val){fEnabledUpDown = val;}
        void SetEnableLeftRight(bool val){fEnabledLeftRight = val;}

        
};

#endif