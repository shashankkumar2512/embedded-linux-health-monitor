# Embedded Linux Device Health Monitor & Auto-Recovery Agent

A C++17-based Linux system health monitoring and automatic recovery agent designed to continuously monitor device health and automatically recover failed services.

## Features

- CPU usage monitoring
- Memory usage monitoring
- Disk usage monitoring
- CPU/system temperature monitoring
- Network availability monitoring
- Critical Linux service monitoring
- Configurable health thresholds using JSON
- Structured timestamped logging
- Automatic service restart
- systemd integration
- systemd process auto-recovery
- Failure simulation
- Unit tests using CTest
- CMake build system

## Architecture

The monitor runs continuously and periodically collects system health information from Linux interfaces.

### Monitoring Sources

| Metric | Linux Source |
|---|---|
| CPU | `/proc/stat` |
| Memory | `/proc/meminfo` |
| Disk | `statvfs()` |
| Temperature | `/sys/class/thermal` and `/sys/class/hwmon` |
| Network | `/sys/class/net` |
| Service status | `systemctl` |

The collected metrics are compared with configurable thresholds.

If a monitored service becomes inactive, the agent attempts to restart it automatically.

The health-monitor process itself is managed by systemd with:

```text
Restart=always
RestartSec=5