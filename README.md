# Embedded Linux Device Health Monitor & Auto-Recovery Agent

A **C++17-based Linux system health monitoring and automatic recovery agent** that continuously monitors device health, detects abnormal conditions, monitors critical services, and automatically performs recovery actions.

The project is designed for Linux-based systems and demonstrates practical use of **Linux system interfaces, C++, systemd, process monitoring, service recovery, configuration management, logging, and automated testing**.

---

## 🚀 Features

* CPU utilization monitoring
* RAM/memory utilization monitoring
* Disk space monitoring
* CPU/system temperature monitoring
* Network availability monitoring
* Critical Linux service monitoring
* Configurable health thresholds
* JSON-based configuration
* Structured timestamped logging
* Automatic recovery of failed services
* systemd integration
* Automatic recovery of the monitoring process
* Failure simulation
* Unit testing with CTest
* CMake-based build system

---

## 🏗️ System Architecture

```text
                    Linux Operating System
                            │
          ┌─────────────────┼─────────────────┐
          │                 │                 │
        /proc              /sys           systemctl
          │                 │                 │
     ┌────┴────┐       ┌────┴────┐           │
     │         │       │         │           │
   CPU      Memory  Temperature Network    Services
     │         │       │         │           │
     └─────────┴───────┴─────────┴───────────┘
                            │
                            ▼
                 Health Monitor Agent
                            │
              ┌─────────────┴─────────────┐
              │                           │
       Health Evaluation            Service Check
              │                           │
              ▼                           ▼
           Logger                 Recovery Action
                                          │
                                          ▼
                                  systemctl restart
                                          │
                                          ▼
                                   Service Restored

                       systemd
                          │
                          ▼
                 Process Supervision
                          │
                  Restart on Failure
```

---

## 📊 Monitoring Components

| Component           | Data Source                              | Purpose                                   |
| ------------------- | ---------------------------------------- | ----------------------------------------- |
| CPU Monitor         | `/proc/stat`                             | Calculates CPU utilization                |
| Memory Monitor      | `/proc/meminfo`                          | Calculates memory utilization             |
| Disk Monitor        | `statvfs()`                              | Calculates filesystem usage               |
| Temperature Monitor | `/sys/class/thermal`, `/sys/class/hwmon` | Reads available temperature sensors       |
| Network Monitor     | `/sys/class/net`                         | Checks network availability               |
| Service Monitor     | `systemctl`                              | Checks and recovers Linux services        |
| Logger              | File + system output                     | Records system health and recovery events |
| Config Manager      | JSON configuration                       | Loads thresholds and monitoring settings  |

---

## 🔄 How It Works

The agent continuously performs the following cycle:

```text
Start
  │
  ▼
Load Configuration
  │
  ▼
Collect System Metrics
  │
  ├── CPU
  ├── Memory
  ├── Disk
  ├── Temperature
  └── Network
  │
  ▼
Check Configured Thresholds
  │
  ▼
Check Critical Services
  │
  ├── Service Active ───────► Continue Monitoring
  │
  └── Service Inactive
             │
             ▼
       Restart Service
             │
             ▼
        Log Recovery
             │
             ▼
       Wait for Interval
             │
             └──────────────► Repeat
```

---

## 🛡️ Automatic Recovery

The project provides **two levels of recovery**.

### 1. Monitored Service Recovery

When a configured Linux service becomes inactive, the application detects the failure and attempts to restart it.

```text
Service Failure
      │
      ▼
ServiceMonitor
      │
      ▼
systemctl is-active
      │
      ▼
Service Inactive
      │
      ▼
systemctl restart <service>
      │
      ▼
Service Active
```

### 2. Health Monitor Process Recovery

The health monitor itself is supervised by systemd.

The service uses:

```ini
Restart=always
RestartSec=5
```

Therefore, if the monitoring process terminates unexpectedly:

```text
Health Monitor Process
        │
        ▼
   Process Failure
        │
        ▼
      systemd
        │
        ▼
 Restart after 5 seconds
        │
        ▼
Health Monitor Running
```

This provides an additional layer of reliability.

---

## ⚙️ Configuration

Configuration is stored in:

```text
config/health_monitor.json
```

Example:

```json
{
    "cpu_threshold_percent": 80,
    "memory_threshold_percent": 80,
    "disk_threshold_percent": 85,
    "temperature_threshold_celsius": 75,
    "check_interval_seconds": 5,
    "monitored_services": [
        "cron.service"
    ]
}
```

### Configuration Parameters

| Parameter                       | Description                   |
| ------------------------------- | ----------------------------- |
| `cpu_threshold_percent`         | CPU warning threshold         |
| `memory_threshold_percent`      | Memory warning threshold      |
| `disk_threshold_percent`        | Disk usage warning threshold  |
| `temperature_threshold_celsius` | Temperature warning threshold |
| `check_interval_seconds`        | Monitoring interval           |
| `monitored_services`            | Linux services to monitor     |

---

## 📁 Project Structure

```text
embedded-linux-health-monitor/
│
├── CMakeLists.txt
├── README.md
├── .gitignore
│
├── config/
│   └── health_monitor.json
│
├── include/
│   ├── config_manager.h
│   ├── cpu_monitor.h
│   ├── disk_monitor.h
│   ├── logger.h
│   ├── memory_monitor.h
│   ├── network_monitor.h
│   ├── service_monitor.h
│   └── temperature_monitor.h
│
├── src/
│   ├── config_manager.cpp
│   ├── cpu_monitor.cpp
│   ├── disk_monitor.cpp
│   ├── logger.cpp
│   ├── main.cpp
│   ├── memory_monitor.cpp
│   ├── network_monitor.cpp
│   ├── service_monitor.cpp
│   └── temperature_monitor.cpp
│
├── systemd/
│   └── device-health-monitor.service
│
├── scripts/
│   ├── install.sh
│   ├── simulate_failure.sh
│   └── uninstall.sh
│
├── tests/
│   ├── test_config.cpp
│   ├── test_cpu.cpp
│   ├── test_memory.cpp
│   └── test_service_monitor.cpp
│
└── docs/
    ├── architecture.md
    └── test-report.md
```

---

## 🧰 Technologies Used

* **C++17**
* **CMake**
* **Linux**
* **systemd**
* **nlohmann/json**
* **CTest**
* Linux `/proc` filesystem
* Linux `/sys` filesystem
* Linux filesystem APIs
* Git/GitHub

---

## 📋 Requirements

### Software

* Linux-based operating system
* C++17-compatible compiler
* CMake
* nlohmann-json
* systemd

### Ubuntu/Debian

Install the required JSON library:

```bash
sudo apt update
sudo apt install nlohmann-json3-dev
```

---

## 🔨 Build

Clone the repository:

```bash
git clone https://github.com/shashankkumar2512/embedded-linux-health-monitor.git
cd embedded-linux-health-monitor
```

Configure the project:

```bash
cmake -S . -B build
```

Build:

```bash
cmake --build build -j$(nproc)
```

---

## 🧪 Run Tests

Execute the complete test suite:

```bash
ctest --test-dir build --output-on-failure
```

Current test result:

```text
100% tests passed, 0 tests failed out of 4
```

Tests included:

```text
CPU_Monitor_Test
Memory_Monitor_Test
Config_Manager_Test
Service_Monitor_Test
```

---

## ▶️ Run the Monitor

Run the application manually:

```bash
./build/device-health-monitor config/health_monitor.json
```

Example output:

```text
[INFO] Embedded Linux Health Monitor starting
[INFO] Configuration loaded successfully
[INFO] Health monitoring started
[INFO] CPU=0.0% | Memory=22.8% | Disk=5.3% | Temperature=N/A | Network=UP
[INFO] Service cron.service is active
```

The monitor checks the system every **5 seconds** according to the current configuration.

---

## 📝 Logging

Runtime logs are written to:

```text
logs/health_monitor.log
```

Example:

```text
[2026-09-23 15:43:33] [INFO] Health monitoring started
[2026-09-23 15:43:33] [INFO] CPU=0.0% | Memory=22.8% | Disk=5.3% | Temperature=N/A | Network=UP
[2026-09-23 15:43:33] [INFO] Service cron.service is active
```

The `logs/` directory is excluded from Git using `.gitignore`.

---

## ⚡ systemd Deployment

The project includes a systemd service:

```text
systemd/device-health-monitor.service
```

Install it using:

```bash
sudo ./scripts/install.sh
```

Check the service:

```bash
sudo systemctl status device-health-monitor
```

Expected state:

```text
Active: active (running)
```

Enable it to start automatically:

```bash
sudo systemctl enable device-health-monitor
```

---

## 🧯 Failure Simulation

The project contains a failure simulation script:

```bash
./scripts/simulate_failure.sh
```

For controlled service recovery testing, the project uses:

```text
health-monitor-test.service
```

The test service can safely be stopped:

```bash
sudo systemctl stop health-monitor-test.service
```

The health monitor detects the inactive service and attempts to restart it.

Verify:

```bash
systemctl is-active health-monitor-test.service
```

Expected:

```text
active
```

> For testing, use the dedicated test service instead of intentionally stopping a production-critical Linux service.

---

## 🔍 Verified Recovery Tests

### Health Monitor Process Recovery

The monitor process was intentionally terminated.

systemd automatically restarted it.

Verified using:

```bash
systemctl show device-health-monitor -p MainPID -p NRestarts
```

Observed:

```text
MainPID=18787
NRestarts=1
```

This confirms that systemd successfully restarted the monitoring process.

### Service Recovery

The test service was intentionally stopped:

```bash
sudo systemctl stop health-monitor-test.service
```

It was subsequently detected and restarted by the health monitor.

Verification:

```bash
systemctl is-active health-monitor-test.service
```

Result:

```text
active
```

---

## 🌡️ Temperature on WSL

When running inside WSL2, physical thermal sensor interfaces may not be exposed.

Therefore, the monitor may display:

```text
Temperature=N/A
```

This is handled gracefully by the application.

On a Linux machine or embedded device exposing thermal or hwmon sensors, the monitor can read the available temperature values.

---

## 🧪 Test Summary

| Test                      | Result |
| ------------------------- | ------ |
| Project compilation       | ✅ PASS |
| CPU monitoring            | ✅ PASS |
| Memory monitoring         | ✅ PASS |
| Disk monitoring           | ✅ PASS |
| Temperature handling      | ✅ PASS |
| Network monitoring        | ✅ PASS |
| Configuration loading     | ✅ PASS |
| Service monitoring        | ✅ PASS |
| CPU unit test             | ✅ PASS |
| Memory unit test          | ✅ PASS |
| Configuration unit test   | ✅ PASS |
| Service monitor unit test | ✅ PASS |
| systemd deployment        | ✅ PASS |
| Process auto-recovery     | ✅ PASS |
| Service auto-recovery     | ✅ PASS |

### Unit Test Result

**4/4 tests passed — 0 failures**

---

## 📚 Documentation

Additional documentation is available in:

```text
docs/architecture.md
docs/test-report.md
```

---

## 🔐 Design Considerations

The project includes basic validation for monitored service names before constructing `systemctl` commands.

Configuration is kept separate from the application code so thresholds and monitored services can be changed without modifying the monitoring logic.

Runtime-generated build files and logs are excluded from version control.

---

## 🚀 Future Improvements

Potential extensions include:

* REST API for remote monitoring
* Web dashboard
* Prometheus metrics export
* Alert notifications
* Email/Telegram notifications
* More detailed network monitoring
* Process-level resource monitoring
* Hardware-specific temperature sensors
* Configurable recovery policies
* Historical health metrics
* Container/Docker monitoring
* Cross-platform embedded Linux deployment

---

## 👨‍💻 Project

**Embedded Linux Device Health Monitor & Auto-Recovery Agent**

Built using **C++17, Linux, CMake, systemd, and nlohmann/json**.

The project demonstrates continuous system monitoring, fault detection, logging, service recovery, process supervision, and automated testing on Linux.
