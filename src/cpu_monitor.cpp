#include "cpu_monitor.h"

#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>

namespace {

struct CpuStats {
    long long user = 0;
    long long nice = 0;
    long long system = 0;
    long long idle = 0;
    long long iowait = 0;
    long long irq = 0;
    long long softirq = 0;
    long long steal = 0;

    long long total() const {
        return user +
               nice +
               system +
               idle +
               iowait +
               irq +
               softirq +
               steal;
    }

    long long idleTime() const {
        return idle + iowait;
    }
};

bool readCpuStats(CpuStats& stats) {

    std::ifstream file("/proc/stat");

    if (!file.is_open()) {
        return false;
    }

    std::string line;

    if (!std::getline(file, line)) {
        return false;
    }

    std::istringstream stream(line);

    std::string cpu;

    stream >> cpu
           >> stats.user
           >> stats.nice
           >> stats.system
           >> stats.idle
           >> stats.iowait
           >> stats.irq
           >> stats.softirq
           >> stats.steal;

    return cpu == "cpu";
}

}

double CpuMonitor::getUsagePercent() const {

    CpuStats first;
    CpuStats second;

    if (!readCpuStats(first)) {
        return -1.0;
    }

    std::this_thread::sleep_for(
        std::chrono::milliseconds(100)
    );

    if (!readCpuStats(second)) {
        return -1.0;
    }

    const long long totalDelta =
        second.total() - first.total();

    const long long idleDelta =
        second.idleTime() - first.idleTime();

    if (totalDelta <= 0) {
        return 0.0;
    }

    double usage =
        100.0 *
        static_cast<double>(
            totalDelta - idleDelta
        ) /
        static_cast<double>(totalDelta);

    if (usage < 0.0) {
        usage = 0.0;
    }

    if (usage > 100.0) {
        usage = 100.0;
    }

    return usage;
}