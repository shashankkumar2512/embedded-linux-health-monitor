#!/bin/bash

set -e

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
INSTALL_DIR="/opt/device-health-monitor"

echo "=========================================="
echo " Device Health Monitor Installation"
echo "=========================================="

echo "[1/5] Building project..."

cmake -S "$PROJECT_DIR" \
      -B "$PROJECT_DIR/build"

cmake --build "$PROJECT_DIR/build" -j"$(nproc)"

echo "[2/5] Creating installation directories..."

sudo mkdir -p "$INSTALL_DIR/bin"
sudo mkdir -p "$INSTALL_DIR/config"
sudo mkdir -p "$INSTALL_DIR/logs"

echo "[3/5] Installing application..."

sudo cp \
    "$PROJECT_DIR/build/device-health-monitor" \
    "$INSTALL_DIR/bin/"

sudo cp \
    "$PROJECT_DIR/config/health_monitor.json" \
    "$INSTALL_DIR/config/"

echo "[4/5] Installing systemd service..."

sudo cp \
    "$PROJECT_DIR/systemd/device-health-monitor.service" \
    /etc/systemd/system/

sudo systemctl daemon-reload

echo "[5/5] Enabling service..."

if systemctl is-system-running >/dev/null 2>&1; then

    sudo systemctl enable device-health-monitor

    sudo systemctl restart device-health-monitor

    echo
    echo "Service installed and started."

    sudo systemctl status \
        device-health-monitor \
        --no-pager

else

    echo
    echo "WARNING:"
    echo "systemd is not currently available."
    echo "The application was installed, but the service"
    echo "was not started."
fi

echo
echo "Installation completed."