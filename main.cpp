
#include <iostream>
#include <chrono>
#include <thread>
#include <memory>

#include "SensorManager.h"
#include "TemperatureSensor.h"
#include "PressureSensor.h"

int main() {
    std::cout << "Starting Sensor Monitoring System...\n";

    SensorManager manager;

    manager.addSensor(
        std::make_unique<TemperatureSensor>(1000)
    );

    manager.addSensor(
        std::make_unique<PressureSensor>(500)
    );

    // Start producers and consumer
    manager.start();

    // Run for 10 seconds
    std::this_thread::sleep_for(
        std::chrono::seconds(10)
    );

    // Stop producers, drain queue, join consumer
    manager.stop();

    std::cout << "\nSimulation ended.\n";

    manager.printStatistics();

    std::cout << "\nProgram finished cleanly.\n";

    return 0;
}
