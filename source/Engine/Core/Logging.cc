#include <pch.hh>
#include "Logging.hh"
#include <spdlog/sinks/stdout_color_sinks.h>

std::shared_ptr<spdlog::logger> Logger::mLogger;

void Logger::Init() {
    spdlog::set_pattern("%^[%n][%T][%l] - %v%$");
    mLogger = spdlog::stdout_color_mt("GAME");
    mLogger->set_level(spdlog::level::trace);
}