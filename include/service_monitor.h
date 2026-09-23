#ifndef SERVICE_MONITOR_H
#define SERVICE_MONITOR_H

#include <string>

class ServiceMonitor {
public:

    bool isServiceActive(
        const std::string& service
    ) const;

    bool restartService(
        const std::string& service
    ) const;

private:

    bool isValidServiceName(
        const std::string& service
    ) const;
};

#endif