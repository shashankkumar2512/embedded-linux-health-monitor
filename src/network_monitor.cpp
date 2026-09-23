#include "network_monitor.h"

#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

bool NetworkMonitor::isNetworkAvailable() const {

    const std::string networkPath =
        "/sys/class/net";

    if (!fs::exists(networkPath)) {
        return false;
    }

    try {

        for (const auto& entry :
             fs::directory_iterator(networkPath)) {

            const std::string interfaceName =
                entry.path().filename().string();

            if (interfaceName == "lo") {
                continue;
            }

            std::ifstream stateFile(
                entry.path() / "operstate"
            );

            if (!stateFile.is_open()) {
                continue;
            }

            std::string state;

            stateFile >> state;

            if (state == "up" ||
                state == "unknown") {

                return true;
            }
        }

    } catch (...) {

        return false;
    }

    return false;
}