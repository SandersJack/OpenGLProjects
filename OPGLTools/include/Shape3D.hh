#ifndef Shape3D_HPP
#define Shape3D_HPP

#include <GL/glew.h>

#include <GLFW/glfw3.h>
//GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;


class Shape3D {

    public:
        Shape3D();
        ~Shape3D();

        static Shape3D *GetInstance();

        void render3DShape(GLuint shaderProgram, GLuint VAO, const glm::mat4& modelViewProjection, 
            const glm::vec3& shapePosition, GLuint vertexSize);

        void generateCircleVertices(GLfloat* vertices, float radius, int numPoints, float height);

        void generateQuadVertices(GLfloat* vertices, float width, float height, float depth);

    private:

        static Shape3D *fInstance;

};

#endif