#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <string>
#include <vector>

class ConfigManager {
public:

    explicit ConfigManager(
        const std::string& configPath
    );

    bool load();

    double getCpuThreshold() const;

    double getMemoryThreshold() const;

    double getDiskThreshold() const;

    double getTemperatureThreshold() const;

    int getCheckInterval() const;

    const std::vector<std::string>&
    getMonitoredServices() const;

private:

    std::string configPath;

    double cpuThreshold = 80.0;

    double memoryThreshold = 80.0;

    double diskThreshold = 85.0;

    double temperatureThreshold = 75.0;

    int checkInterval = 5;

    std::vector<std::string> monitoredServices;
};

#endif