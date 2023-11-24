#ifndef Controls_HPP
#define Controls_HPP


// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//using namespace glm;

class Controls
{
    private:
        static Controls *fInstance;

        glm::vec3 fPosition; 
        float fHorizontalAngle;
        float fVerticalAngle;
        float fInitialFoV;

        float fSpeed;
        float fMouseSpeed;

        glm::mat4 fViewMatrix;
        glm::mat4 fProjectionMatrix;

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

        
};

#endif