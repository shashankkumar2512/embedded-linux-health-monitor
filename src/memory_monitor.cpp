#include "memory_monitor.h"

#include <fstream>
#include <string>

double MemoryMonitor::getUsagePercent() const {

    std::ifstream file("/proc/meminfo");

    if (!file.is_open()) {
        return -1.0;
    }

    long long memTotal = 0;
    long long memAvailable = 0;

    std::string key;
    long long value;
    std::string unit;

    while (file >> key >> value >> unit) {

        if (key == "MemTotal:") {
            memTotal = value;
        }

        else if (key == "MemAvailable:") {
            memAvailable = value;
        }

        if (memTotal > 0 &&
            memAvailable > 0) {
            break;
        }
    }

    if (memTotal <= 0) {
        return -1.0;
    }

    double used =
        static_cast<double>(
            memTotal - memAvailable
        );

    double usage =
        (used /
         static_cast<double>(memTotal))
        * 100.0;

    if (usage < 0.0) {
        usage = 0.0;
    }

    if (usage > 100.0) {
        usage = 100.0;
    }

    return usage;
}