#ifndef Shape2D_HPP
#define Shape2D_HPP

#include <GL/glew.h>

#include <GLFW/glfw3.h>
//GLFWwindow* window;

#include "MathGL.hh"

#include "Shader.hh"
#include "Texture2D.hh"


class Shape2D {

    public:
        Shape2D();
        ~Shape2D();

        static Shape2D *GetInstance();

        void render2DShape(Shader shaderProgram, GLuint VAO, const Matrix4& modelViewProjection, 
            const Vector2& shapePosition, GLuint vertexSize, const Vector4& color_alpha);

        void render2DShape(Shader shaderProgram, const Texture2D textureProgram, GLuint VAO, const Matrix4& modelViewProjection, 
            const Vector2& shapePosition, GLuint vertexSize, const Vector4& color_alpha);

        void generateQuadVertices(GLfloat* vertices, float width, float height);

    private:

        static Shape2D *fInstance;

};

#endif