#include "memory_monitor.h"

#include <cassert>
#include <iostream>

int main() {

    MemoryMonitor monitor;

    const double usage =
        monitor.getUsagePercent();

    std::cout
        << "Memory usage: "
        << usage
        << "%"
        << std::endl;

    assert(usage >= 0.0);
    assert(usage <= 100.0);

    std::cout
        << "Memory test PASSED"
        << std::endl;

    return 0;
}