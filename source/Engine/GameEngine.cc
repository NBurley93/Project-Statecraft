#include "GameEngine.hh"
#include <tinyxml2.h>
#include <pch.hh>

using namespace tinyxml2;

GameEngine* GameEngine::CreateEngine(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        throw new std::exception("SDL failed to initialize");
    }

    SDL_version v;
    SDL_GetVersion(&v);
    GAME_INFO("SDL {}.{}.{}", v.major, v.minor, v.patch);

    // Load config
    GAME_INFO("Loading config.xml...");
    XMLDocument doc;
    doc.LoadFile("game/config.xml");

    int width, height;
    bool fullscreen, vsync;

    auto windowCfg = doc.FirstChildElement("window");
    auto cfgWndWidth = windowCfg->FirstChildElement("width");
    auto cfgWndHeight = windowCfg->FirstChildElement("height");
    auto cfgWndFulls = windowCfg->FirstChildElement("fullscreen");
    auto cfgWndVsync = windowCfg->FirstChildElement("vsync");

    cfgWndWidth->QueryIntText(&width);
    cfgWndHeight->QueryIntText(&height);
    cfgWndFulls->QueryBoolText(&fullscreen);
    cfgWndVsync->QueryBoolText(&vsync);

    GAME_INFO("Creating engine...");
    GameEngine *engine = new GameEngine();
    GAME_INFO("Creating Window and Renderer with params.");
    GAME_INFO("Width: {}, Height: {}, Fullscreen: {}, VSync: {}", width, height, fullscreen, vsync);
    engine->mWindow = new Window(Window::WindowProperties(width, height, fullscreen, vsync));
    return engine;
}


GameEngine::~GameEngine() {
    delete tex;
    delete mWindow;
    SDL_Quit();
}


bool GameEngine::Init() {
    if (mWindow->IsInitialized()) {
        tex = new Texture();
        tex->LoadImage(*mWindow->GetContext(), "game/textures/test.png");

        mRunning = true;
        return true;
    } else {
        return false;
    }
}


void GameEngine::Run() {
    SDL_SetRenderDrawBlendMode(mWindow->GetContext(), SDL_BLENDMODE_BLEND);

    while (mRunning) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            switch(e.type) {
                case SDL_QUIT:
                mRunning = false;
                break;
            }
        }

        SDL_SetRenderDrawColor(mWindow->GetContext(), 255, 0, 255, 255);
        SDL_RenderClear(mWindow->GetContext());
        SDL_SetRenderDrawColor(mWindow->GetContext(), 255, 255, 255, 255);

        // Draw stuff
        SDL_Rect src;
        SDL_Rect dst;

        src.x = 0;
        src.y = 0;
        src.w = tex->Width();
        src.h = tex->Height();

        dst.x = 0;
        dst.y = 0;
        dst.w = mWindow->GetWidth();
        dst.h = mWindow->GetHeight();

        SDL_RenderCopy(
            mWindow->GetContext(),
            tex->GetID(),
            &src,
            &dst
        );

        SDL_RenderPresent(mWindow->GetContext());
    }
}

