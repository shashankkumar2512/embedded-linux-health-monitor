#include "service_monitor.h"

#include <cassert>
#include <iostream>

int main() {

    ServiceMonitor monitor;

    // Invalid service name should be rejected.
    const bool result =
        monitor.isServiceActive(
            "invalid;service"
        );

    assert(result == false);

    std::cout
        << "Service monitor validation test PASSED"
        << std::endl;

    return 0;
}