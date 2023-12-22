#include "SpriteRenderer.hh"


#include "MathGL.hh"
#include "MatrixTransform.hh"


SpriteRenderer *SpriteRenderer::fInstance = nullptr;

SpriteRenderer *SpriteRenderer::GetInstance() {
    if(!fInstance)
        fInstance = new SpriteRenderer();
    return fInstance;
}

SpriteRenderer::SpriteRenderer() {

}

void SpriteRenderer::DrawSprite(const Texture2D& texture, Vector2 position, Vector2 size, float rotate, Vector3 color){
    // prepare transformations
    fShader.Use();
    Matrix4 model(1.0f);
    std::cout << " --------- Draw --------- " << std::endl;
    model = Translate(model, Vector3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    model = Translate(model, Vector3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
    model = Rotate(model, ToRadians(rotate), Vector3(0.0f, 0.0f, 1.0f)); // then rotate
    model = Translate(model, Vector3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back
    //model.Print();
    model = Scale(model, Vector3(size, 1.0f)); // last scale

    fShader.SetMatrix4("model", model);

    // render textured quad
    fShader.SetVector3f("spriteColor", color);
    glActiveTexture(GL_TEXTURE0);
    texture.Bind();
    glBindVertexArray(fQuadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::initRenderData(){
    // configure VAO/VBO
    uint VBO;
    float vertices[] = { 
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &fQuadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(fQuadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}