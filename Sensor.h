//
// Created by Aref Shiran on 10/13/26.
//

#ifndef REALTIME_IOT_SENSOR_SIMULATOR_SENSOR_H
#define REALTIME_IOT_SENSOR_SIMULATOR_SENSOR_H

#pragma once

#include <string>

// Abstract base class for all sensors
class Sensor {
public:
    virtual ~Sensor() = default;

    // Return the name of the sensor
    virtual std::string getName() const = 0;

    // Generate a sensor reading
    virtual double readValue() = 0;

    // Return sampling interval in milliseconds
    virtual int getIntervalMs() const = 0;
};
#endif //REALTIME_IOT_SENSOR_SIMULATOR_SENSOR_H
