#ifndef OGLManager_HPP
#define OGLManager_HPP

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "Vector4.hh"

#include <string>


class OGLManager {

    public:
        OGLManager();
        ~OGLManager();

        static OGLManager *GetInstance();

        void Init(int windowX, int windowY, const char* name="window_name");

        GLFWwindow* GetWindow(){ return fWindow;};

        void SetWindowResize(bool val){fWindowResize = false;}
        void SetWindowColour(Vector4 val){fWindowColor = val;}

    private:

        static OGLManager *fInstance;

        GLFWwindow* fWindow;

        int fWindowX;
        int fWindowY;

        bool fWindowResize;
        Vector4 fWindowColor;   

};

#endif