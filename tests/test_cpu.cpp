#include "cpu_monitor.h"

#include <cassert>
#include <iostream>

int main() {

    CpuMonitor monitor;

    const double usage =
        monitor.getUsagePercent();

    std::cout
        << "CPU usage: "
        << usage
        << "%"
        << std::endl;

    assert(usage >= 0.0);
    assert(usage <= 100.0);

    std::cout
        << "CPU test PASSED"
        << std::endl;

    return 0;
}