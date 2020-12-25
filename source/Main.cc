#include "Engine/GameEngine.hh"
#include <pch.hh>

int main(int argc, char* argv[]) {
    Logger::Init();
    GAME_INFO("Initialized logging");
    GAME_INFO("Starting Ritual Game Engine...");
    GameEngine *engine = GameEngine::CreateEngine(argc, argv);
    if (engine->Init()) {
        engine->Run();
    }
    GAME_INFO("Cleaning up...");
    delete engine;
    GAME_INFO("Shutdown complete.");
    return 0;
}