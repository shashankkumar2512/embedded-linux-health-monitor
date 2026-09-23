#include "temperature_monitor.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

namespace {

double readTemperature(const fs::path& filePath) {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        return -1.0;
    }

    long long rawValue = 0;
    file >> rawValue;

    if (!file) {
        return -1.0;
    }

    double temperature;

    if (std::llabs(rawValue) >= 1000) {
        temperature =
            static_cast<double>(rawValue) / 1000.0;
    } else {
        temperature =
            static_cast<double>(rawValue);
    }

    if (temperature <= -50.0 ||
        temperature >= 150.0) {
        return -1.0;
    }

    return temperature;
}

} // namespace

double TemperatureMonitor::getTemperatureCelsius() const {

    double maximumTemperature = -1.0;

    // Check Linux thermal zones.
    const fs::path thermalPath = "/sys/class/thermal";

    try {
        if (fs::exists(thermalPath) &&
            fs::is_directory(thermalPath)) {

            for (const auto& entry :
                 fs::directory_iterator(thermalPath)) {

                const std::string name =
                    entry.path().filename().string();

                if (name.rfind("thermal_zone", 0) != 0) {
                    continue;
                }

                const double temperature =
                    readTemperature(
                        entry.path() / "temp"
                    );

                if (temperature > maximumTemperature) {
                    maximumTemperature = temperature;
                }
            }
        }
    }
    catch (...) {
    }

    // Check Linux hwmon devices.
    const fs::path hwmonPath = "/sys/class/hwmon";

    try {
        if (fs::exists(hwmonPath) &&
            fs::is_directory(hwmonPath)) {

            for (const auto& hwmon :
                 fs::directory_iterator(hwmonPath)) {

                if (!fs::is_directory(hwmon.path())) {
                    continue;
                }

                for (const auto& entry :
                     fs::directory_iterator(hwmon.path())) {

                    const std::string name =
                        entry.path().filename().string();

                    if (name.rfind("temp", 0) != 0) {
                        continue;
                    }

                    if (name.find("_input") ==
                        std::string::npos) {
                        continue;
                    }

                    const double temperature =
                        readTemperature(entry.path());

                    if (temperature > maximumTemperature) {
                        maximumTemperature = temperature;
                    }
                }
            }
        }
    }
    catch (...) {
    }

    return maximumTemperature;
}
