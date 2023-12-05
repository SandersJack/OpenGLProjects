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
void Shape3D::render3DShape(GLuint shaderProgram, GLuint VAO, const glm::mat4& modelViewProjection, const glm::vec3& shapePosition, 
	GLuint vertexSize) {
    
	glUseProgram(shaderProgram);

    // Set the shape position using a uniform variable
    GLuint shapePositionLocation = glGetUniformLocation(shaderProgram, "shapePosition");
    glUniform3fv(shapePositionLocation, 1, &shapePosition[0]);

    // Set the model-view-projection matrix
    GLuint mvpLocation = glGetUniformLocation(shaderProgram, "modelViewProjection");
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &modelViewProjection[0][0]);

    glBindVertexArray(VAO);

    // Draw the 3D shape
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexSize/3);  // Assuming a simple quad for this example

    glBindVertexArray(0);

    // Check for OpenGL errors after drawing
}
