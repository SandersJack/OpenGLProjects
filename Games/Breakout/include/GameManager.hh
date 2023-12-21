#ifndef GameManager_HPP
#define GameManager_HPP

#include "Global.hh"

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class GameManager {

    public:
        GameState fState;
        bool fKeys[1024];
        
        GameManager();
        ~GameManager();

        void Init();

        void ProcessInput(float dt);
        void Update(float dt);
        void Render();

        static GameManager *GetInstance();

        void SetWidthHeight(uint val_w, uint val_h){fWidth = val_w; fHeight = val_h;}
        void SetWidth(uint val){ fWidth = val;}
        void SetHeight(uint val){ fHeight = val;}

    private:
        static GameManager *fInstance;

        uint fWidth, fHeight;

};

#endif