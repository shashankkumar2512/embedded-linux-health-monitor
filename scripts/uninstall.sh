#!/bin/bash

set -e

INSTALL_DIR="/opt/device-health-monitor"

echo "Removing Device Health Monitor..."

if systemctl is-system-running >/dev/null 2>&1; then

    sudo systemctl stop \
        device-health-monitor \
        2>/dev/null || true

    sudo systemctl disable \
        device-health-monitor \
        2>/dev/null || true
fi

sudo rm -f \
    /etc/systemd/system/device-health-monitor.service

if systemctl is-system-running >/dev/null 2>&1; then
    sudo systemctl daemon-reload
fi

sudo rm -rf "$INSTALL_DIR"

echo "Device Health Monitor removed."