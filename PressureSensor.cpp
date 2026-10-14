//
// Created by Aref Shiran on 10/13/26.
//

#include "PressureSensor.h"

// Constructor
PressureSensor::PressureSensor(int interval)
    : intervalMs(interval),
      generator(std::random_device{}()),
      noise(-2.0, 2.0)
{
}

// Return the sensor name
std::string PressureSensor::getName() const {
    return "Pressure Sensor";
}

// Generate a simulated pressure reading
double PressureSensor::readValue() {
    return 101.3 + noise(generator);
}

// Return the sampling interval in milliseconds
int PressureSensor::getIntervalMs() const {
    return intervalMs;
}
