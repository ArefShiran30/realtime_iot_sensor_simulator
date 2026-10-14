
#pragma once

#include "Sensor.h"
#include "SensorReading.h"
#include "ThreadSafeQueue.h"

#include <atomic>
#include <memory>
#include <thread>
#include <vector>

class SensorManager {

private:
    // Store sensor objects
    std::vector<std::unique_ptr<Sensor>> sensors;

    // Producer threads
    std::vector<std::thread> threads;

    // Consumer thread
    std::thread consumerThread;

    // Shared thread-safe queue
    ThreadSafeQueue<SensorReading> readingQueue;

    // Jitter data for each sensor
    std::vector<std::vector<double>> jitterData;

    // Atomic stop signal
    std::atomic<bool> stopFlag{false};

    // Tracks whether start() has been called
    bool started = false;

    // Producer function
    void sensorTask(
        Sensor& sensor,
        std::vector<double>& jitters
    );

    // Consumer function
    void consumerTask();

public:
    SensorManager() = default;

    // Disable copying
    SensorManager(const SensorManager&) = delete;
    SensorManager& operator=(const SensorManager&) = delete;

    // Destructor
    ~SensorManager();

    // Register a sensor
    void addSensor(std::unique_ptr<Sensor> sensor);

    // Start all threads
    void start();

    // Stop all threads
    void stop();

    // Print performance statistics
    void printStatistics() const;
};
