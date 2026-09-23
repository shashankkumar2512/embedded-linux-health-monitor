#!/bin/bash

set -e

MODE="${1:-help}"

show_help() {

    echo
    echo "Embedded Linux Health Monitor Failure Simulator"
    echo
    echo "Usage:"
    echo
    echo "  ./simulate_failure.sh cpu"
    echo "  ./simulate_failure.sh service <service-name>"
    echo
}

simulate_cpu() {

    echo "Starting CPU load..."

    CPU_COUNT="$(nproc)"

    PIDS=()

    for ((i=0; i<CPU_COUNT; i++)); do
        yes > /dev/null &
        PIDS+=($!)
    done

    echo "CPU load started."
    echo "Press Ctrl+C to stop."

    cleanup() {

        echo
        echo "Stopping CPU load..."

        for PID in "${PIDS[@]}"; do
            kill "$PID" 2>/dev/null || true
        done

        echo "CPU simulation stopped."
    }

    trap cleanup EXIT INT TERM

    while true; do
        sleep 1
    done
}

simulate_service() {

    SERVICE="$1"

    if [ -z "$SERVICE" ]; then
        echo "Please provide a service name."
        exit 1
    fi

    echo "Checking service: $SERVICE"

    if ! systemctl list-unit-files \
        "${SERVICE}.service" \
        --no-legend >/dev/null 2>&1; then

        echo "Service may not exist."
    fi

    echo "Stopping service..."

    sudo systemctl stop "$SERVICE"

    echo
    echo "Service stopped."
    echo
    echo "The health monitor should detect the failure."
    echo
    echo "Restart the service manually with:"
    echo
    echo "sudo systemctl start $SERVICE"
}

case "$MODE" in

    cpu)
        simulate_cpu
        ;;

    service)
        simulate_service "$2"
        ;;

    *)
        show_help
        ;;

esac