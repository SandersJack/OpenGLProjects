#include "GameManager.hh"

GameManager *GameManager::fInstance = nullptr;

GameManager *GameManager::GetInstance() {
    if(!fInstance)
        fInstance = new GameManager();
    return fInstance;
}

GameManager::GameManager():
    fState(GAME_ACTIVE), fWidth(800), fHeight(600) {

}

GameManager::~GameManager(){}

void GameManager::Init(){

}

void GameManager::Update(float dt){

}

void GameManager::ProcessInput(float dt){

}

void GameManager::Render(){
    
}