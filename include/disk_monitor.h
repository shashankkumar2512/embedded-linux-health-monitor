#ifndef DISK_MONITOR_H
#define DISK_MONITOR_H

class DiskMonitor {
public:
    double getUsagePercent(
        const char* path = "/"
    ) const;
};

#endif