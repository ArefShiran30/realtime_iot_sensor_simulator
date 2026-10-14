//
// Created by Aref Shiran on 10/13/26.
//

#ifndef REALTIME_IOT_SENSOR_SIMULATOR_SENSORREADING_H
#define REALTIME_IOT_SENSOR_SIMULATOR_SENSORREADING_H




#pragma once

#include <string>
#include <chrono>

// Represents one reading produced by a sensor
struct SensorReading {

    // Name of the sensor
    std::string sensorName;

    // Measured sensor value
    double value;

    // Measurement unit, e.g. C or kPa
    std::string unit;

    // Time when the reading was generated
    std::chrono::steady_clock::time_point timestamp;

    // Sampling interval jitter in milliseconds
    double jitterMs;
};



#endif //REALTIME_IOT_SENSOR_SIMULATOR_SENSORREADING_H
