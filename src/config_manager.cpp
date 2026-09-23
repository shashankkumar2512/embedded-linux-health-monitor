#include "config_manager.h"

#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

ConfigManager::ConfigManager(
    const std::string& configPath
)
    : configPath(configPath) {
}

bool ConfigManager::load() {

    try {

        std::ifstream file(configPath);

        if (!file.is_open()) {
            return false;
        }

        json config;

        file >> config;

        cpuThreshold =
            config.value(
                "cpu_threshold_percent",
                80.0
            );

        memoryThreshold =
            config.value(
                "memory_threshold_percent",
                80.0
            );

        diskThreshold =
            config.value(
                "disk_threshold_percent",
                85.0
            );

        temperatureThreshold =
            config.value(
                "temperature_threshold_celsius",
                75.0
            );

        checkInterval =
            config.value(
                "check_interval_seconds",
                5
            );

        monitoredServices.clear();

        if (config.contains("monitored_services") &&
            config["monitored_services"].is_array()) {

            for (const auto& service :
                 config["monitored_services"]) {

                if (service.is_string()) {
                    monitoredServices.push_back(
                        service.get<std::string>()
                    );
                }
            }
        }

        return true;

    } catch (const std::exception&) {

        return false;
    }
}

double ConfigManager::getCpuThreshold() const {
    return cpuThreshold;
}

double ConfigManager::getMemoryThreshold() const {
    return memoryThreshold;
}

double ConfigManager::getDiskThreshold() const {
    return diskThreshold;
}

double ConfigManager::getTemperatureThreshold() const {
    return temperatureThreshold;
}

int ConfigManager::getCheckInterval() const {
    return checkInterval;
}

const std::vector<std::string>&
ConfigManager::getMonitoredServices() const {
    return monitoredServices;
}