#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include "config_manager.h"
#include "cpu_monitor.h"
#include "disk_monitor.h"
#include "logger.h"
#include "memory_monitor.h"
#include "network_monitor.h"
#include "service_monitor.h"
#include "temperature_monitor.h"

namespace {

std::string formatValue(
    double value,
    const std::string& unit
) {
    if (value < 0.0) {
        return "N/A";
    }

    std::ostringstream stream;

    stream << std::fixed
           << std::setprecision(1)
           << value
           << unit;

    return stream.str();
}

}

int main(int argc, char* argv[]) {

    std::string configPath =
        "../config/health_monitor.json";

    if (argc > 1) {
        configPath = argv[1];
    }

    if (!Logger::initialize(
            "logs/health_monitor.log")) {

        std::cerr
            << "Warning: Could not open log file."
            << std::endl;
    }

    Logger::info(
        "Embedded Linux Health Monitor starting"
    );

    ConfigManager config(configPath);

    if (!config.load()) {

        Logger::error(
            "Failed to load configuration: " +
            configPath
        );

        return EXIT_FAILURE;
    }

    Logger::info(
        "Configuration loaded successfully"
    );

    CpuMonitor cpuMonitor;
    MemoryMonitor memoryMonitor;
    DiskMonitor diskMonitor;
    TemperatureMonitor temperatureMonitor;
    NetworkMonitor networkMonitor;
    ServiceMonitor serviceMonitor;

    const double cpuThreshold =
        config.getCpuThreshold();

    const double memoryThreshold =
        config.getMemoryThreshold();

    const double diskThreshold =
        config.getDiskThreshold();

    const double temperatureThreshold =
        config.getTemperatureThreshold();

    const int interval =
        config.getCheckInterval();

    Logger::info(
        "Health monitoring started"
    );

    while (true) {

        const double cpu =
            cpuMonitor.getUsagePercent();

        const double memory =
            memoryMonitor.getUsagePercent();

        const double disk =
            diskMonitor.getUsagePercent("/");

        const double temperature =
            temperatureMonitor.getTemperatureCelsius();

        const bool network =
            networkMonitor.isNetworkAvailable();

        std::ostringstream status;

        status
            << "CPU="
            << formatValue(cpu, "%")
            << " | Memory="
            << formatValue(memory, "%")
            << " | Disk="
            << formatValue(disk, "%")
            << " | Temperature="
            << formatValue(temperature, "C")
            << " | Network="
            << (network ? "UP" : "DOWN");

        Logger::info(status.str());

        // CPU threshold
        if (cpu >= 0.0 &&
            cpu > cpuThreshold) {

            Logger::warning(
                "CPU usage exceeded threshold: " +
                formatValue(cpu, "%")
            );
        }

        // Memory threshold
        if (memory >= 0.0 &&
            memory > memoryThreshold) {

            Logger::warning(
                "Memory usage exceeded threshold: " +
                formatValue(memory, "%")
            );
        }

        // Disk threshold
        if (disk >= 0.0 &&
            disk > diskThreshold) {

            Logger::warning(
                "Disk usage exceeded threshold: " +
                formatValue(disk, "%")
            );
        }

        // Temperature threshold
        if (temperature >= 0.0 &&
            temperature > temperatureThreshold) {

            Logger::warning(
                "Temperature exceeded threshold: " +
                formatValue(
                    temperature,
                    "C"
                )
            );
        }

        // Network
        if (!network) {

            Logger::warning(
                "No active network interface detected"
            );
        }

        // Service monitoring + recovery
        for (const std::string& service :
             config.getMonitoredServices()) {

            if (serviceMonitor.isServiceActive(
                    service)) {

                Logger::info(
                    "Service " +
                    service +
                    " is active"
                );

            } else {

                Logger::error(
                    "Service " +
                    service +
                    " is NOT active"
                );

                Logger::recovery(
                    "Attempting to restart service: " +
                    service
                );

                if (serviceMonitor.restartService(
                        service)) {

                    Logger::recovery(
                        "Successfully restarted service: " +
                        service
                    );

                } else {

                    Logger::error(
                        "Failed to restart service: " +
                        service
                    );
                }
            }
        }

        std::this_thread::sleep_for(
            std::chrono::seconds(interval)
        );
    }

    return EXIT_SUCCESS;
}