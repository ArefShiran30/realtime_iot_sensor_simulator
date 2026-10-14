//
// Created by Aref Shiran on 10/13/26.
//

#ifndef REALTIME_IOT_SENSOR_SIMULATOR_PRESSURESENSOR_H
#define REALTIME_IOT_SENSOR_SIMULATOR_PRESSURESENSOR_H



#pragma once

#include "Sensor.h"
#include <random>
#include <string>

// PressureSensor inherits from Sensor
class PressureSensor : public Sensor {

private:
    // Sampling interval in milliseconds
    int intervalMs;

    // Random number generator
    std::mt19937 generator;

    // Simulated pressure variation
    std::uniform_real_distribution<double> noise;

public:
    // Constructor
    explicit PressureSensor(int interval);

    // Return the sensor name
    std::string getName() const override;

    // Generate a simulated pressure reading
    double readValue() override;

    // Return the sampling interval
    int getIntervalMs() const override;
};




#endif //REALTIME_IOT_SENSOR_SIMULATOR_PRESSURESENSOR_H
