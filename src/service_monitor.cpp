#include "service_monitor.h"

#include <cstdlib>
#include <cctype>
#include <regex>

bool ServiceMonitor::isValidServiceName(
    const std::string& service
) const {

    if (service.empty() ||
        service.size() > 255) {
        return false;
    }

    for (char c : service) {

        if (!(std::isalnum(
                  static_cast<unsigned char>(c)
              ) ||
              c == '.' ||
              c == '_' ||
              c == '-' ||
              c == '@')) {

            return false;
        }
    }

    return true;
}

bool ServiceMonitor::isServiceActive(
    const std::string& service
) const {

    if (!isValidServiceName(service)) {
        return false;
    }

    const std::string command =
        "systemctl is-active --quiet " +
        service;

    const int result =
        std::system(command.c_str());

    return result == 0;
}

bool ServiceMonitor::restartService(
    const std::string& service
) const {

    if (!isValidServiceName(service)) {
        return false;
    }

    const std::string command =
        "systemctl restart " +
        service;

    const int result =
        std::system(command.c_str());

    return result == 0;
}