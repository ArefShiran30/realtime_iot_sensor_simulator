//
// Created by Aref Shiran on 10/13/26.
//

#include "TemperatureSensor.h"

// Constructor
TemperatureSensor::TemperatureSensor(int interval)
    : intervalMs(interval),
      generator(std::random_device{}()),
      noise(-0.5, 0.5)
{
}

// Return sensor name
std::string TemperatureSensor::getName() const {
    return "Temperature Sensor";
}

// Generate a simulated temperature reading
double TemperatureSensor::readValue() {
    return 25.0 + noise(generator);
}

// Return sampling interval in milliseconds
int TemperatureSensor::getIntervalMs() const {
    return intervalMs;
}
