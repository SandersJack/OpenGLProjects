#include "Shape3D.hh"

Shape3D *Shape3D::fInstance = nullptr;

Shape3D *Shape3D::GetInstance() {
    if(!fInstance)
        fInstance = new Shape3D();
    return fInstance;
}

Shape3D::Shape3D(){

}

Shape3D::~Shape3D(){

}

// Function to render the 3D shape
void Shape3D::render3DShape(GLuint shaderProgram, GLuint VAO, const Matrix4& modelViewProjection, const Vector3& shapePosition, 
	GLuint vertexSize, const Vector4 &color_alpha) {
    
	glUseProgram(shaderProgram);

    // Set the shape position using a uniform variable
    GLuint shapePositionLocation = glGetUniformLocation(shaderProgram, "shapePosition");
    glUniform3fv(shapePositionLocation, 1, &shapePosition[0]);

    // Set the model-view-projection matrix
    GLuint mvpLocation = glGetUniformLocation(shaderProgram, "modelViewProjection");
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &modelViewProjection[0][0]);

    // Set the shape color and alpha
    GLuint shapeColorLocation = glGetUniformLocation(shaderProgram, "color");
    glUniform4fv(shapeColorLocation, 1, &color_alpha[0]);

    glBindVertexArray(VAO);

    // Draw the 3D shape
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexSize/3);  // Assuming a simple quad for this example

    glBindVertexArray(0);

    // Check for OpenGL errors after drawing
}

void Shape3D::generateCircleVertices(GLfloat* vertices, float radius, int numPoints, float height) {
    for (int i = 0; i < numPoints; ++i) {
        float theta = 2.0f * PI * i / numPoints;
        float x = radius * std::cos(theta);
        float y = radius * std::sin(theta);
        float z = height;

        // Add the vertex coordinates to the array
        vertices[i * 3] = x;
        vertices[i * 3 + 1] = y;
		vertices[i * 3 + 2] = z;
    }
}

void Shape3D::generateQuadVertices(GLfloat* vertices, float width, float height, float depth) {
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;

    for (int i = 0; i < 4; ++i) {
        float x = ((i % 2 == 0) ? -1 : 1) * halfWidth;
        float y = ((i / 2 == 0) ? -1 : 1) * halfHeight;

        vertices[i * 3] = x;
        vertices[i * 3 + 1] = y;
        vertices[i * 3 + 2] = depth / 2.0f;  // Adjust for depth
    }
}

