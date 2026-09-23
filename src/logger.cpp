#include "logger.h"

#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>

namespace {

std::ofstream logFile;
std::mutex logMutex;

std::string getTimestamp() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t currentTime =
        std::chrono::system_clock::to_time_t(now);

    std::tm timeInfo{};

    localtime_r(&currentTime, &timeInfo);

    std::ostringstream timestamp;

    timestamp << std::put_time(
        &timeInfo,
        "%Y-%m-%d %H:%M:%S"
    );

    return timestamp.str();
}

void writeLog(
    const std::string& level,
    const std::string& message
) {
    std::lock_guard<std::mutex> lock(logMutex);

    const std::string line =
        "[" + getTimestamp() + "] [" +
        level + "] " + message;

    std::cout << line << std::endl;

    if (logFile.is_open()) {
        logFile << line << std::endl;
        logFile.flush();
    }
}

}

bool Logger::initialize(const std::string& filePath) {

    try {
        const std::filesystem::path path(filePath);

        if (!path.parent_path().empty()) {
            std::filesystem::create_directories(
                path.parent_path()
            );
        }

        logFile.open(
            filePath,
            std::ios::app
        );

        return logFile.is_open();

    } catch (...) {
        return false;
    }
}

void Logger::info(const std::string& message) {
    writeLog("INFO", message);
}

void Logger::warning(const std::string& message) {
    writeLog("WARNING", message);
}

void Logger::error(const std::string& message) {
    writeLog("ERROR", message);
}

void Logger::recovery(const std::string& message) {
    writeLog("RECOVERY", message);
}