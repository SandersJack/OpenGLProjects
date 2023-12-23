#include "Shape2D.hh"

Shape2D *Shape2D::fInstance = nullptr;

Shape2D *Shape2D::GetInstance() {
    if(!fInstance)
        fInstance = new Shape2D();
    return fInstance;
}

Shape2D::Shape2D(){

}

Shape2D::~Shape2D(){

}

// Function to render the 2D shape
void Shape2D::render2DShape(Shader shaderProgram, GLuint VAO, const Matrix4& modelViewProjection, const Vector2& shapePosition, 
	GLuint vertexSize, const Vector4 &color_alpha) {
    
	shaderProgram.Use();

    // Set the shape position using a uniform variable
    GLuint shapePositionLocation = glGetUniformLocation(shaderProgram.GetID(), "shapePosition");
    glUniform2fv(shapePositionLocation, 1, &shapePosition[0]);

    // Set the model-view-projection matrix
    GLuint mvpLocation = glGetUniformLocation(shaderProgram.GetID(), "modelViewProjection");
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, modelViewProjection.value_ptr());

    // Set the shape color and alpha
    GLuint shapeColorLocation = glGetUniformLocation(shaderProgram.GetID(), "color");
    glUniform4fv(shapeColorLocation, 1, color_alpha.value_ptr());

    glBindVertexArray(VAO);

    // Draw the 3D shape
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexSize/2);  // Assuming a simple quad for this example

    glBindVertexArray(0);

    // Check for OpenGL errors after drawing
}

// Function to render the 2D shape with texture
void Shape2D::render2DShape(Shader shaderProgram, const Texture2D textureProgram, GLuint VAO, const Matrix4& modelViewProjection, const Vector2& shapePosition, 
	GLuint vertexSize, const Vector4 &color_alpha) {
    
	shaderProgram.Use();

    // Set the shape position using a uniform variable
    GLuint shapePositionLocation = glGetUniformLocation(shaderProgram.GetID(), "shapePosition");
    glUniform2fv(shapePositionLocation, 1, &shapePosition[0]);

    // Set the model-view-projection matrix
    GLuint mvpLocation = glGetUniformLocation(shaderProgram.GetID(), "modelViewProjection");
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, modelViewProjection.value_ptr());

    // Set the shape color and alpha
    GLuint shapeColorLocation = glGetUniformLocation(shaderProgram.GetID(), "color");
    glUniform4fv(shapeColorLocation, 1, color_alpha.value_ptr());

    glActiveTexture(GL_TEXTURE0);
    textureProgram.Bind();

    glBindVertexArray(VAO);

    // Draw the 3D shape
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexSize/4);  // Assuming a simple quad for this example

    glBindVertexArray(0);

    // Check for OpenGL errors after drawing
}


void Shape2D::generateQuadVertices(GLfloat* vertices, float width, float height) {
    // Calculate half-width and half-height for convenience
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;

    // Define vertices in counterclockwise order
    vertices[0] = -halfWidth;  // Bottom-left x
    vertices[1] = -halfHeight; // Bottom-left y
    vertices[2] = 0.0f;        // Bottom-left z

    vertices[3] = halfWidth;   // Bottom-right x
    vertices[4] = -halfHeight; // Bottom-right y
    vertices[5] = 0.0f;        // Bottom-right z

    vertices[6] = halfWidth;   // Top-right x
    vertices[7] = halfHeight;  // Top-right y
    vertices[8] = 0.0f;        // Top-right z

    vertices[9] = -halfWidth;  // Top-left x
    vertices[10] = halfHeight; // Top-left y
    vertices[11] = 0.0f;       // Top-left z
}

