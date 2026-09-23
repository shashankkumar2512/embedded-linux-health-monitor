# ⚡ Embedded Linux Health Monitor

### **Detect. Diagnose. Recover. Automatically.**

A lightweight **C++17 Linux system health monitor** designed to continuously observe system resources, detect unhealthy conditions, monitor critical services, and automatically recover from service failures.

Built around native Linux interfaces, **systemd**, configurable thresholds, structured logging, and automated testing.

---

<div align="center">

### 🖥️ SYSTEM HEALTH · ⚙️ SERVICE WATCH · 🔄 AUTO RECOVERY

**C++17**  •  **Linux**  •  **CMake**  •  **systemd**  •  **CTest**

</div>

---

## 🎯 The Idea

Most monitoring tools tell you that something is wrong.

This project goes one step further:

```text
                    ┌─────────────────┐
                    │   Linux Device  │
                    └────────┬────────┘
                             │
                     Observe the system
                             │
                             ▼
                    ┌─────────────────┐
                    │  Health Monitor  │
                    └────────┬────────┘
                             │
                  ┌──────────┴──────────┐
                  ▼                     ▼
             System Health        Service Health
                  │                     │
                  ▼                     ▼
              Thresholds          Active / Failed
                  │                     │
                  └──────────┬──────────┘
                             ▼
                       Take Action
                             │
                   ┌─────────┴─────────┐
                   ▼                   ▼
                 Log                Recover
                                      │
                                      ▼
                                Keep Running
```

The agent continuously monitors the machine and can restart configured services when they become inactive.

---

# 🧩 What It Monitors

|     | Component       | Source                                   | Purpose                       |
| --- | --------------- | ---------------------------------------- | ----------------------------- |
| 🖥️ | **CPU**         | `/proc/stat`                             | CPU utilization               |
| 🧠  | **Memory**      | `/proc/meminfo`                          | Memory utilization            |
| 💾  | **Disk**        | `statvfs()`                              | Filesystem usage              |
| 🌡️ | **Temperature** | `/sys/class/thermal`, `/sys/class/hwmon` | Thermal readings              |
| 🌐  | **Network**     | `/sys/class/net`                         | Network availability          |
| ⚙️  | **Services**    | `systemctl`                              | Service monitoring & recovery |
| 📝  | **Logger**      | File + system output                     | Health & recovery events      |
| ⚙️  | **Config**      | JSON                                     | Monitoring configuration      |

---

# 🔄 Inside the Monitoring Loop

Every monitoring cycle follows the same path:

```text
                    START
                      │
                      ▼
              Load Configuration
                      │
                      ▼
             ┌──────────────────┐
             │ Collect Metrics  │
             └────────┬─────────┘
                      │
          ┌───────────┼───────────┐
          ▼           ▼           ▼
        CPU         Memory       Disk
          │           │           │
          └───────────┼───────────┘
                      │
                 Temperature
                      │
                    Network
                      │
                      ▼
              Evaluate Thresholds
                      │
                      ▼
               Check Services
                      │
               ┌──────┴──────┐
               ▼             ▼
             ACTIVE        INACTIVE
               │             │
               │             ▼
               │        Restart Service
               │             │
               │             ▼
               │         Log Recovery
               │             │
               └──────┬──────┘
                      ▼
                Wait 5 Seconds
                      │
                      └──────► REPEAT
```

The monitoring interval is configurable and is currently set to **5 seconds**.

---

# 🛡️ Two Layers of Recovery

The project doesn't only watch other services.

It also protects **itself**.

### Layer 01 — Service Recovery

```text
Critical Service
      │
      ▼
   Failure
      │
      ▼
systemctl is-active
      │
      ▼
  INACTIVE
      │
      ▼
systemctl restart
      │
      ▼
   ACTIVE ✓
```

The configured service is detected and a restart is attempted automatically.

### Layer 02 — Monitor Recovery

The monitoring agent itself runs under systemd:

```ini
Restart=always
RestartSec=5
```

So if the monitor process terminates:

```text
Monitor
  │
  ✕
Crash / Termination
  │
  ▼
 systemd
  │
  ▼
Wait 5 seconds
  │
  ▼
Restart
  │
  ▼
Monitor Running ✓
```

---

# ⚙️ Configuration-Driven

No need to modify C++ code just to change monitoring thresholds.

```text
config/
└── health_monitor.json
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

### Configuration Controls

```text
CPU Threshold          → 80%
Memory Threshold       → 80%
Disk Threshold         → 85%
Temperature Threshold  → 75°C
Check Interval         → 5 sec
Monitored Services     → configurable
```

---

# 🏗️ Architecture

```text
                         LINUX KERNEL
                              │
             ┌────────────────┼────────────────┐
             │                │                │
             ▼                ▼                ▼
           /proc             /sys          systemctl
             │                │                │
       ┌─────┴─────┐    ┌─────┴─────┐    ┌───┴────┐
       │ CPU       │    │ Thermal   │    │Services│
       │ Memory    │    │ Network   │    └───┬────┘
       └─────┬─────┘    └─────┬─────┘        │
             │                │               │
             └────────┬───────┴───────────────┘
                      ▼
             ┌────────────────────┐
             │  HEALTH MONITOR    │
             └─────────┬──────────┘
                       │
              ┌────────┴────────┐
              ▼                 ▼
         Health Engine     Service Engine
              │                 │
              ▼                 ▼
           Logger          Recovery Logic
                                │
                                ▼
                           systemctl
                                │
                                ▼
                         Service Restored
```

---

# 📁 Project Structure

```text
embedded-linux-health-monitor/
│
├── ⚙️ config/
│   └── health_monitor.json
│
├── 📦 include/
│   ├── config_manager.h
│   ├── cpu_monitor.h
│   ├── disk_monitor.h
│   ├── logger.h
│   ├── memory_monitor.h
│   ├── network_monitor.h
│   ├── service_monitor.h
│   └── temperature_monitor.h
│
├── 🔧 src/
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
├── ⚡ systemd/
│   └── device-health-monitor.service
│
├── 🧪 scripts/
│   ├── install.sh
│   ├── simulate_failure.sh
│   └── uninstall.sh
│
├── ✅ tests/
│   ├── test_config.cpp
│   ├── test_cpu.cpp
│   ├── test_memory.cpp
│   └── test_service_monitor.cpp
│
└── 📚 docs/
    ├── architecture.md
    └── test-report.md
```

---

# 🧰 Technology Stack

```text
┌─────────────────────────────────────────────┐
│                  APPLICATION                │
│                    C++17                    │
├─────────────────────────────────────────────┤
│                  BUILD SYSTEM               │
│                    CMake                    │
├─────────────────────────────────────────────┤
│                 SYSTEM LAYER                │
│             Linux / systemd                 │
├─────────────────────────────────────────────┤
│              CONFIGURATION                  │
│               nlohmann/json                 │
├─────────────────────────────────────────────┤
│                   TESTING                   │
│                    CTest                    │
└─────────────────────────────────────────────┘
```

The monitor directly uses Linux `/proc`, `/sys`, and filesystem interfaces.

---

# 🔨 Build

### 1. Clone

```bash
git clone https://github.com/shashankkumar2512/embedded-linux-health-monitor.git
cd embedded-linux-health-monitor
```

### 2. Configure

```bash
cmake -S . -B build
```

### 3. Build

```bash
cmake --build build -j$(nproc)
```

---

# 🧪 Test

Run:

```bash
ctest --test-dir build --output-on-failure
```

### Result

```text
╭──────────────────────────────╮
│       TEST RESULT            │
├──────────────────────────────┤
│  Tests        : 4            │
│  Passed       : 4            │
│  Failed       : 0            │
│  Status       : PASS ✓       │
╰──────────────────────────────╯
```

Covered tests:

* CPU Monitor
* Memory Monitor
* Configuration Manager
* Service Monitor

---

# ▶️ Run It

```bash
./build/device-health-monitor config/health_monitor.json
```

Example runtime:

```text
[INFO] Embedded Linux Health Monitor starting
[INFO] Configuration loaded successfully
[INFO] Health monitoring started
[INFO] CPU=0.0% | Memory=22.8% | Disk=5.3% | Temperature=N/A | Network=UP
[INFO] Service cron.service is active
```

---

# 📝 Observability

Logs are written to:

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

# ⚡ systemd Deployment

Install:

```bash
sudo ./scripts/install.sh
```

Check:

```bash
sudo systemctl status device-health-monitor
```

Enable at boot:

```bash
sudo systemctl enable device-health-monitor
```

Expected:

```text
Active: active (running)
```

---

# 🧯 Failure Recovery Test

A dedicated test service is used for safe recovery testing.

```bash
sudo systemctl stop health-monitor-test.service
```

The monitor detects the failure and attempts recovery.

Verify:

```bash
systemctl is-active health-monitor-test.service
```

Result:

```text
active
```

This provides a controlled way to demonstrate the service-recovery mechanism.

---

# 🔬 Verified Recovery

### Monitor Process

The monitoring process was intentionally terminated.

systemd restarted it automatically.

```text
MainPID=18787
NRestarts=1
```

### Monitored Service

```text
health-monitor-test.service
        │
        ▼
     stopped
        │
        ▼
     detected
        │
        ▼
     restarted
        │
        ▼
      active ✓
```

---

# 📊 Verification Snapshot

| Test Area               | Status |
| ----------------------- | :----: |
| Compilation             |    ✅   |
| CPU Monitoring          |    ✅   |
| Memory Monitoring       |    ✅   |
| Disk Monitoring         |    ✅   |
| Temperature Handling    |    ✅   |
| Network Monitoring      |    ✅   |
| Configuration           |    ✅   |
| Service Monitoring      |    ✅   |
| CPU Unit Test           |    ✅   |
| Memory Unit Test        |    ✅   |
| Configuration Unit Test |    ✅   |
| Service Unit Test       |    ✅   |
| systemd Deployment      |    ✅   |
| Process Auto-Recovery   |    ✅   |
| Service Auto-Recovery   |    ✅   |

**15/15 listed verification areas passed.**

---

# 🌡️ Running on WSL2?

One limitation is environment-dependent:

```text
WSL2
 │
 └── Physical thermal sensors may not be exposed
                         │
                         ▼
                  Temperature=N/A
```

The application handles this gracefully. On Linux systems exposing thermal or hwmon sensors, available readings can be collected.

---

# 🔐 Design Philosophy

The project follows a simple systems-engineering approach:

```text
         OBSERVE
            ↓
         MEASURE
            ↓
         EVALUATE
            ↓
          ACT
            ↓
          LOG
            ↓
         RECOVER
            ↓
         REPEAT
```

Configuration remains separate from the application logic, monitored service names receive basic validation, and generated logs/build files remain outside version control.

---

# 🚀 Possible Evolution

The current project establishes the local monitoring and recovery foundation.

Its documented future direction includes:

```text
Local Monitor
     │
     ├── Remote Monitoring
     ├── Web Dashboard
     ├── Prometheus Metrics
     ├── Alert Notifications
     ├── Historical Metrics
     ├── Process Monitoring
     ├── Container Monitoring
     └── Embedded Linux Deployment
```

These are potential extensions rather than current features.

---

<div align="center">

## ⚡ Embedded Linux Health Monitor

**A small agent with a simple mission:**

### **Watch the system. Detect failure. Recover automatically.**

Built with **C++17 · Linux · CMake · systemd**

</div>
