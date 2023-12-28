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

    modelLocation = glGetUniformLocation(fResourceManager->GetShader("sprite").GetID(), "model");
    colorLocation = glGetUniformLocation(fResourceManager->GetShader("sprite").GetID(), "color");

    // Set initial model matrix and color uniforms
    Matrix4 modelMatrix = Matrix4(1.0f);
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &modelMatrix[0][0]);

    Vector3 sliderColor = Vector3(0.2f, 0.2f, 0.8f);
    glUniform3fv(colorLocation, 1, &sliderColor[0]);
    

}

void GameManager::Update(float dt){
    
}

void GameManager::ProcessInput(float dt){

}

void GameManager::Render(){
    const Texture2D &textureRef = fResourceManager->GetTexture("face");
    Vector3 Color = Vector3(0.2f, 0.2f, 0.8f);
    fSpriteRenderer->DrawSprite(textureRef, Vector2(0.,0.), Vector2(0.0,0.0), 45.0, Color);
}