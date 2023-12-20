#ifndef Shape3D_HPP
#define Shape3D_HPP

#include <GL/glew.h>

#include <GLFW/glfw3.h>
//GLFWwindow* window;

#include "MathGL.hh"


class Shape3D {

    public:
        Shape3D();
        ~Shape3D();

        static Shape3D *GetInstance();

        void render3DShape(GLuint shaderProgram, GLuint VAO, const Matrix4& modelViewProjection, 
            const Vector3& shapePosition, GLuint vertexSize, const Vector4& color_alpha);

        void generateCircleVertices(GLfloat* vertices, float radius, int numPoints, float height);

        void generateQuadVertices(GLfloat* vertices, float width, float height, float depth);

    private:

        static Shape3D *fInstance;

};

#endif