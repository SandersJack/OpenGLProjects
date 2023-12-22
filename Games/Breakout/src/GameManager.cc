#include "GameManager.hh"

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

    Matrix4 projection = Ortho(0.0f, static_cast<float>(fWidth), 
        static_cast<float>(fHeight), 0.0f, -1.0f, 1.0f);
    fResourceManager->GetShader("sprite").Use().SetInteger("image", 0);
    fResourceManager->GetShader("sprite").SetMatrix4("projection", projection);

    fSpriteRenderer->SetShader(fResourceManager->GetShader("sprite"));
    fSpriteRenderer->initRenderData();
    // load textures
    fResourceManager->LoadTexture("textures/awesomeface.png", true, "face");
}

void GameManager::Update(float dt){
    
}

void GameManager::ProcessInput(float dt){

}

void GameManager::Render(){
    const Texture2D &textureRef = fResourceManager->GetTexture("face");
    fSpriteRenderer->DrawSprite(textureRef, 
        Vector2(200.0f, 200.0f), Vector2(300.0f, 400.0f), 45.0f, Vector3(0.0f, 1.0f, 0.0f));  
}