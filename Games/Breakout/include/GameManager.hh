#ifndef GameManager_HPP
#define GameManager_HPP

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class GameManager {

    public:
        GameState fState;
        bool fKeys[1024];
        unsigned int fWidth, fHeight;
        GameManager(unsigned int width, unsigned int height);
        ~GameManager();

        void Init();

        void ProcessInput(float dt);
        void Update(float dt);
        void Render();

};

#endif