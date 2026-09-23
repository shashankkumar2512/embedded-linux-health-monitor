#include "disk_monitor.h"

#include <sys/statvfs.h>

double DiskMonitor::getUsagePercent(
    const char* path
) const {

    struct statvfs filesystem{};

    if (statvfs(path, &filesystem) != 0) {
        return -1.0;
    }

    const unsigned long long total =
        static_cast<unsigned long long>(
            filesystem.f_blocks
        ) *
        filesystem.f_frsize;

    const unsigned long long available =
        static_cast<unsigned long long>(
            filesystem.f_bavail
        ) *
        filesystem.f_frsize;

    if (total == 0) {
        return -1.0;
    }

    const unsigned long long used =
        total - available;

    return (
        static_cast<double>(used) /
        static_cast<double>(total)
    ) * 100.0;
}