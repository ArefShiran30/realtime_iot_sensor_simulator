//
// Created by Aref Shiran on 10/13/26.
//

#ifndef REALTIME_IOT_SENSOR_SIMULATOR_TEMPERATURESENSOR_H
#define REALTIME_IOT_SENSOR_SIMULATOR_TEMPERATURESENSOR_H


#pragma once

#include "Sensor.h"
#include <random>
#include <string>

// TemperatureSensor inherits from Sensor
class TemperatureSensor : public Sensor {

private:
    // Time interval between readings (milliseconds)
    int intervalMs;

    // Random number generator
    std::mt19937 generator;

    // Simulated temperature noise
    std::uniform_real_distribution<double> noise;

public:
    // Constructor
    explicit TemperatureSensor(int interval);

    // Return sensor name
    std::string getName() const override;

    // Generate a temperature reading
    double readValue() override;

    // Return sampling interval
    int getIntervalMs() const override;
};


#endif //REALTIME_IOT_SENSOR_SIMULATOR_TEMPERATURESENSOR_H
