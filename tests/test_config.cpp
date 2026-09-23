#include "config_manager.h"

#include <cassert>
#include <iostream>

int main(int argc, char* argv[]) {

    if (argc < 2) {

        std::cerr
            << "Usage: test_config <config-file>"
            << std::endl;

        return 1;
    }

    ConfigManager config(argv[1]);

    const bool loaded =
        config.load();

    assert(loaded);

    assert(
        config.getCpuThreshold() > 0.0
    );

    assert(
        config.getMemoryThreshold() > 0.0
    );

    assert(
        config.getDiskThreshold() > 0.0
    );

    assert(
        config.getTemperatureThreshold() > 0.0
    );

    assert(
        config.getCheckInterval() > 0
    );

    std::cout
        << "Configuration test PASSED"
        << std::endl;

    return 0;
}