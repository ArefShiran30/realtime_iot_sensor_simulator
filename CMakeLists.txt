
cmake_minimum_required(VERSION 3.16)

project(realtime_iot_sensor_simulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(realtime_iot_sensor_simulator
        main.cpp
        TemperatureSensor.cpp
        PressureSensor.cpp
        SensorManager.cpp
)
