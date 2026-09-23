cmake_minimum_required(VERSION 3.16)

project(EmbeddedLinuxHealthMonitor
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# Find JSON library
find_package(nlohmann_json CONFIG REQUIRED)

# Include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Main source files
set(SOURCES
    src/main.cpp
    src/cpu_monitor.cpp
    src/memory_monitor.cpp
    src/disk_monitor.cpp
    src/temperature_monitor.cpp
    src/network_monitor.cpp
    src/service_monitor.cpp
    src/logger.cpp
    src/config_manager.cpp
)

# Main executable
add_executable(device-health-monitor ${SOURCES})

target_link_libraries(
    device-health-monitor
    PRIVATE nlohmann_json::nlohmann_json
)

# Enable testing
enable_testing()

# -------------------------
# CPU Test
# -------------------------

add_executable(test_cpu
    tests/test_cpu.cpp
    src/cpu_monitor.cpp
)

target_include_directories(
    test_cpu
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME CPU_Monitor_Test
    COMMAND test_cpu
)

# -------------------------
# Memory Test
# -------------------------

add_executable(test_memory
    tests/test_memory.cpp
    src/memory_monitor.cpp
)

target_include_directories(
    test_memory
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME Memory_Monitor_Test
    COMMAND test_memory
)

# -------------------------
# Config Test
# -------------------------

add_executable(test_config
    tests/test_config.cpp
    src/config_manager.cpp
)

target_include_directories(
    test_config
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(
    test_config
    PRIVATE nlohmann_json::nlohmann_json
)

add_test(
    NAME Config_Manager_Test
    COMMAND test_config ${PROJECT_SOURCE_DIR}/config/health_monitor.json
)

# -------------------------
# Service Monitor Test
# -------------------------

add_executable(test_service_monitor
    tests/test_service_monitor.cpp
    src/service_monitor.cpp
)

target_include_directories(
    test_service_monitor
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

add_test(
    NAME Service_Monitor_Test
    COMMAND test_service_monitor
)