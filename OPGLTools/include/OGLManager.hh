#ifndef OGLManager_HPP
#define OGLManager_HPP

#include <GL/glew.h>

#include <GLFW/glfw3.h>


class OGLManager {

    public:
        OGLManager();
        ~OGLManager();

        static OGLManager *GetInstance();

        void Init(int windowX, int windowY);

        GLFWwindow* GetWindow(){ return fWindow;};

    private:

        static OGLManager *fInstance;

        GLFWwindow* fWindow;

        int fWindowX;
        int fWindowY;

};

#endif