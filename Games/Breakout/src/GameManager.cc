#include "GameManager.hh"

#include "Shape2D.hh"

static GLuint VAO, modelLocation, colorLocation;

GameManager *GameManager::fInstance = nullptr;

GameManager *GameManager::GetInstance() {
    if(!fInstance)
        fInstance = new GameManager();
    return fInstance;
}

GameManager::GameManager():
    fState(GAME_ACTIVE), fWidth(800), fHeight(600) {
    fResourceManager = ResourceManager::GetInstance();
    fSpriteRenderer = SpriteRenderer::GetInstance();
}

GameManager::~GameManager(){}

void GameManager::Init(){
    fResourceManager->LoadShader("shaders/sprite.vs", "shaders/sprite.fs", "sprite");

    Matrix4 modelMatrix1 = Matrix4(1.0f);
    fResourceManager->GetShader("sprite").Use().SetInteger("image", 0);
    fResourceManager->GetShader("sprite").SetMatrix4("model", modelMatrix1);

    fSpriteRenderer->SetShader(fResourceManager->GetShader("sprite"));
    fSpriteRenderer->initRenderData();
    // load textures
    fResourceManager->LoadTexture("textures/awesomeface.png", true, "face");

    //Testing

    //fResourceManager->LoadShader("shaders/shape2d.vs", "shaders/shape2d.fs", "shape2d");
    //fResourceManager->GetShader("sprite").Use();

    modelLocation = glGetUniformLocation(fResourceManager->GetShader("sprite").GetID(), "model");
    colorLocation = glGetUniformLocation(fResourceManager->GetShader("sprite").GetID(), "color");

    // Set initial model matrix and color uniforms
    Matrix4 modelMatrix = Matrix4(1.0f);
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &modelMatrix[0][0]);

    Vector3 sliderColor = Vector3(0.2f, 0.2f, 0.8f);
    glUniform3fv(colorLocation, 1, &sliderColor[0]);
    


    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    
	GLfloat vertices[] = {
        /// Vertex ////   Tex ///
        -0.5f, -0.5f, 0.0f, 0.0f,   // Bottom-left
        0.5f, -0.5f, 1.0f, 0.0f,    // Bottom-right
        0.5f, 0.5f, 1.0f, 1.0f,     // Top-right
        -0.5f, 0.5f, 0.0f, 1.0f
    };
    
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void GameManager::Update(float dt){
    
}

void GameManager::ProcessInput(float dt){

}

void GameManager::Render(){ /*
    const Texture2D &textureRef = fResourceManager->GetTexture("face");
    //fSpriteRenderer->DrawSprite(textureRef, 
    //    Vector2(200.0f, 200.0f), Vector2(300.0f, 400.0f), 45.0f, Vector3(0.0f, 1.0f, 0.0f));  

    fSpriteRenderer->DrawSprite(textureRef, 
        Vector2(0.0f, 0.0f), Vector2(10.0f, 10.0f), 45.0f, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
        */
    // Set the model matrix (for transformations)

    /// Ok this it the same as in the ball and somthing is very broken :( i would have a look at the shaders as i did govery quick on them.

    fResourceManager->GetShader("sprite").Use();
    Matrix4 modelMatrix = Matrix4(1.0f);//.translate(pos);
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, modelMatrix.value_ptr());

    // Set the color (you can update this based on your requirements)
    Vector3 sliderColor = Vector3(0.2f, 0.2f, 0.8f);
    glUniform3fv(colorLocation, 1, sliderColor.value_ptr());
    // Draw the slider using two triangles to form a quad

    glActiveTexture(GL_TEXTURE0);
    fResourceManager->GetTexture("face").Bind();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // Change to GL_TRIANGLE_FAN
    glBindVertexArray(0);

    // Unset the shader program after rendering
    glUseProgram(0);
    checkGLError("Control10");
}