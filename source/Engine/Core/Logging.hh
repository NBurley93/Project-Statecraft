#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

class Logger {
public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger>& GetLogger() { return mLogger; }

private:
    static std::shared_ptr<spdlog::logger> mLogger;
};

// Logging macros
#define GAME_TRACE(...) ::Logger::GetLogger()->trace(__VA_ARGS__)
#define GAME_INFO(...) ::Logger::GetLogger()->info(__VA_ARGS__)
#define GAME_WARN(...) ::Logger::GetLogger()->warn(__VA_ARGS__)
#define GAME_ERROR(...) ::Logger::GetLogger()->error(__VA_ARGS__)
#define GAME_CRITICAL(...) ::Logger::GetLogger()->critical(__VA_ARGS__)