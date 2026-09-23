# Embedded Linux Device Health Monitor & Auto-Recovery Agent

> **Monitor. Detect. Recover. Keep Linux running.**

A **C++17-based Linux system health monitoring and automatic recovery agent** built to continuously observe system health, detect abnormal conditions, monitor critical services, and automatically recover from failures.

Designed for Linux-based systems, the project combines **Linux system interfaces, C++, systemd, process supervision, service recovery, configuration management, structured logging, and automated testing** into a single lightweight monitoring agent.

---

## 🚀 What It Does

The agent continuously watches the health of a Linux system and reacts when something goes wrong.

| Monitor         | What It Checks            |
| --------------- | ------------------------- |
| 🖥️ CPU         | CPU utilization           |
| 🧠 Memory       | RAM utilization           |
| 💾 Disk         | Filesystem usage          |
| 🌡️ Temperature | Available thermal sensors |
| 🌐 Network      | Network availability      |
| ⚙️ Services     | Critical Linux services   |

When a monitored service fails, the agent can detect the failure, attempt recovery, and record the event.

---

## 🧠 System Overview

```text
                    ┌──────────────────────────┐
                    │     Linux Operating      │
                    │         System           │
                    └────────────┬─────────────┘
                                 │
             ┌───────────────────┼───────────────────┐
             ▼                   ▼                   ▼
          /proc                 /sys             systemctl
             │                   │                   │
       ┌─────┴─────┐       ┌─────┴─────┐       ┌───┴────┐
       │ CPU       │       │Temperature│       │Services│
       │ Memory    │       │ Network   │       └───┬────┘
       └─────┬─────┘       └─────┬─────┘           │
             └─────────────┬─────┴─────────────────┘
                           ▼
              ┌─────────────────────────┐
              │    Health Monitor Agent │
              └────────────┬────────────┘
                           │
                ┌──────────┴──────────┐
                ▼                     ▼
        Health Evaluation       Service Check
                │                     │
                ▼                     ▼
             Logging             Recovery
                                      │
                                      ▼
                              systemctl restart
```

---

## 🔄 Monitoring Cycle

The agent follows a continuous monitoring loop:

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
Evaluate Thresholds
  │
  ▼
Check Critical Services
  │
  ├── Active ───────────────► Continue
  │
  └── Inactive
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

The current configuration checks the system every **5 seconds**.

---

## 🛡️ Two-Layer Recovery

### Service Recovery

When a monitored Linux service becomes inactive:

```text
Service Failure
      ↓
ServiceMonitor
      ↓
systemctl is-active
      ↓
Service Inactive
      ↓
systemctl restart
      ↓
Service Active
```

### Monitor Process Recovery

The monitor itself is supervised by **systemd**:

```ini
Restart=always
RestartSec=5
```

If the monitoring process terminates unexpectedly, systemd automatically starts it again.

This creates a simple recovery chain:

```text
System Problem
      ↓
Detection
      ↓
Recovery Action
      ↓
Logging
      ↓
Continued Monitoring
```

---

## ⚙️ Configuration

Monitoring thresholds are kept outside the application code:

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

This keeps system-specific settings separate from the monitoring logic.

---

## 📁 Project Structure

```text
embedded-linux-health-monitor/
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

## 🧰 Built With

**C++17** · **CMake** · **Linux** · **systemd** · **nlohmann/json** · **CTest**

Also uses native Linux interfaces including:

* `/proc`
* `/sys`
* Linux filesystem APIs

---

## 🔨 Build

```bash
git clone https://github.com/shashankkumar2512/embedded-linux-health-monitor.git
cd embedded-linux-health-monitor

cmake -S . -B build
cmake --build build -j$(nproc)
```

---

## 🧪 Testing

Run the complete test suite:

```bash
ctest --test-dir build --output-on-failure
```

### Current Result

```text
4/4 tests passed
0 failures
```

Tests cover:

```text
CPU Monitor
Memory Monitor
Configuration Manager
Service Monitor
```

---

## ▶️ Run

```bash
./build/device-health-monitor config/health_monitor.json
```

Example:

```text
[INFO] Embedded Linux Health Monitor starting
[INFO] Configuration loaded successfully
[INFO] Health monitoring started
[INFO] CPU=0.0% | Memory=22.8% | Disk=5.3% | Temperature=N/A | Network=UP
[INFO] Service cron.service is active
```

---

## 📝 Logging

Runtime events are written to:

```text
logs/health_monitor.log
```

Example:

```text
[2026-09-23 15:43:33] [INFO] Health monitoring started
[2026-09-23 15:43:33] [INFO] CPU=0.0% | Memory=22.8% | Disk=5.3% | Temperature=N/A | Network=UP
[2026-09-23 15:43:33] [INFO] Service cron.service is active
```

---

## ⚡ systemd Deployment

Install the service:

```bash
sudo ./scripts/install.sh
```

Check its status:

```bash
sudo systemctl status device-health-monitor
```

Enable automatic startup:

```bash
sudo systemctl enable device-health-monitor
```

Expected state:

```text
Active: active (running)
```

---

## 🧯 Recovery Testing

A dedicated test service is provided for controlled recovery testing:

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

> **Testing note:** Use the dedicated test service rather than intentionally stopping a production-critical service.

---

## ✅ Verification

The project has verified both recovery layers.

### Monitor Process

```text
MainPID=18787
NRestarts=1
```

This confirms that systemd restarted the monitoring process after termination.

### Monitored Service

```text
health-monitor-test.service
        ↓
      stopped
        ↓
   detected
        ↓
     restarted
        ↓
      active
```

---

## 🌡️ WSL Note

When running under WSL2, physical thermal sensor interfaces may not be available.

In that environment:

```text
Temperature=N/A
```

This is handled gracefully by the application. On Linux systems exposing thermal or hwmon interfaces, available temperature values can be read normally.

---

## 📊 Test Summary

| Area                 | Result |
| -------------------- | ------ |
| Compilation          | ✅ PASS |
| CPU Monitoring       | ✅ PASS |
| Memory Monitoring    | ✅ PASS |
| Disk Monitoring      | ✅ PASS |
| Temperature Handling | ✅ PASS |
| Network Monitoring   | ✅ PASS |
| Configuration        | ✅ PASS |
| Service Monitoring   | ✅ PASS |
| Unit Tests           | ✅ PASS |
| systemd Deployment   | ✅ PASS |
| Process Recovery     | ✅ PASS |
| Service Recovery     | ✅ PASS |

**4/4 unit tests passed — 0 failures.**

---

## 🔐 Design Principles

The project keeps the monitoring system simple and maintainable:

* Configuration is separated from application logic.
* Monitored service names receive basic validation.
* Runtime logs and generated build files stay outside version control.
* systemd provides process-level supervision.
* Recovery actions are explicitly logged.

---

## 🚀 Future Direction

The project can later evolve toward:

```text
Local Monitoring
      ↓
Remote Monitoring
      ↓
Metrics
      ↓
Alerts
      ↓
Historical Analysis
```

Potential extensions include REST monitoring, a web dashboard, Prometheus metrics, notifications, process-level monitoring, historical metrics, Docker monitoring, and broader embedded Linux deployment.

---

## 👨‍💻 Project

### Embedded Linux Device Health Monitor & Auto-Recovery Agent

**C++17 · Linux · CMake · systemd · nlohmann/json**

A practical Linux systems project focused on one core idea:

> **Detect problems early. Recover automatically. Keep monitoring.**
