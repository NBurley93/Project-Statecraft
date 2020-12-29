#pragma once
#include "Display/Window.hh"
#include "Display/Texture.hh"
#include "Audio/AudioManager.hh"


class GameEngine {
public:
    ~GameEngine();

    bool Init();
    void Run();

    static GameEngine* CreateEngine(int argc, char* argv[]);
private:
    void Update();

    Texture* tex;

    Window* mWindow;
    bool mRunning = false;
};