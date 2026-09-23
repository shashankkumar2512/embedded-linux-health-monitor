#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger {
public:
    static bool initialize(const std::string& filePath);

    static void info(const std::string& message);

    static void warning(const std::string& message);

    static void error(const std::string& message);

    static void recovery(const std::string& message);
};

#endif