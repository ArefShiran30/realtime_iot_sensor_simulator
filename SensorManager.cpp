
#include "SensorManager.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <utility>

// ========================================
// DESTRUCTOR
// ========================================

SensorManager::~SensorManager() {
    stop();
}

// ========================================
// ADD SENSOR
// ========================================

void SensorManager::addSensor(
    std::unique_ptr<Sensor> sensor
) {
    if (!sensor) {
        throw std::invalid_argument(
            "Cannot add a null sensor"
        );
    }

    if (started) {
        throw std::logic_error(
            "Cannot add sensors after starting"
        );
    }

    if (sensor->getIntervalMs() <= 0) {
        throw std::invalid_argument(
            "Sensor interval must be positive"
        );
    }

    sensors.push_back(std::move(sensor));
}

// ========================================
// START THREADS
// ========================================

void SensorManager::start() {
    if (started) {
        throw std::logic_error(
            "SensorManager already started"
        );
    }

    if (sensors.empty()) {
        throw std::logic_error(
            "No sensors registered"
        );
    }

    // This manager supports one simulation run.
    started = true;
    stopFlag.store(false);

    jitterData.clear();
    jitterData.resize(sensors.size());

    threads.reserve(sensors.size());

    try {
        // Start consumer thread first
        consumerThread = std::thread(
            &SensorManager::consumerTask,
            this
        );

        // Start one producer thread per sensor
        for (std::size_t i = 0; i < sensors.size(); ++i) {

            threads.emplace_back(
                &SensorManager::sensorTask,
                this,
                std::ref(*sensors[i]),
                std::ref(jitterData[i])
            );
        }
    }
    catch (...) {
        stop();
        throw;
    }
}

// ========================================
// STOP THREADS SAFELY
// ========================================

void SensorManager::stop() {
    // Tell all producer threads to stop
    stopFlag.store(true);

    // Wait for all producers to finish
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    threads.clear();

    // No producers remain, so close queue
    readingQueue.close();

    // Consumer processes remaining queue items
    if (consumerThread.joinable()) {
        consumerThread.join();
    }
}

// ========================================
// PRODUCER THREAD
// ========================================

void SensorManager::sensorTask(
    Sensor& sensor,
    std::vector<double>& jitters
) {
    using Clock = std::chrono::steady_clock;

    const int intervalMs = sensor.getIntervalMs();

    const auto interval =
        std::chrono::milliseconds(intervalMs);

    auto nextTime = Clock::now();
    auto lastTime = nextTime;

    bool firstReading = true;

    while (!stopFlag.load()) {

        const auto now = Clock::now();

        double jitter = 0.0;

        // Calculate sampling interval jitter
        if (!firstReading) {

            const double elapsed =
                std::chrono::duration<double, std::milli>(
                    now - lastTime
                ).count();

            jitter = elapsed - intervalMs;

            jitters.push_back(jitter);
        }

        firstReading = false;
        lastTime = now;

        // Read the sensor using polymorphism
        const double value = sensor.readValue();

        // Determine sensor measurement unit
        std::string unit;

        if (sensor.getName() == "Temperature Sensor") {
            unit = "C";
        }
        else if (sensor.getName() == "Pressure Sensor") {
            unit = "kPa";
        }

        // Create sensor reading record
        SensorReading reading{
            sensor.getName(),
            value,
            unit,
            now,
            jitter
        };

        // Send reading to thread-safe queue
        readingQueue.push(std::move(reading));

        // Schedule the next reading
        nextTime += interval;

        std::this_thread::sleep_until(nextTime);
    }
}

// ========================================
// CONSUMER THREAD
// ========================================

void SensorManager::consumerTask() {

    SensorReading reading;

    // Wait for producer data
    while (readingQueue.waitAndPop(reading)) {

        std::cout
            << "[Consumer] "
            << reading.sensorName
            << " | Value: "
            << std::fixed
            << std::setprecision(2)
            << reading.value
            << " " << reading.unit
            << " | Jitter: "
            << std::showpos
            << std::setprecision(3)
            << reading.jitterMs
            << std::noshowpos
            << " ms\n";
    }

    std::cout
        << "[Consumer] Finished processing.\n";
}

// ========================================
// PERFORMANCE STATISTICS
// ========================================

void SensorManager::printStatistics() const {

    if (!threads.empty() || consumerThread.joinable()) {
        throw std::logic_error(
            "Stop threads before printing statistics"
        );
    }

    for (std::size_t i = 0; i < sensors.size(); ++i) {

        const auto& jitters = jitterData[i];

        std::cout
            << "\n=== "
            << sensors[i]->getName()
            << " Statistics ===\n";

        if (jitters.empty()) {
            std::cout << "No jitter samples available.\n";
            continue;
        }

        // Average jitter
        const double average =
            std::accumulate(
                jitters.begin(),
                jitters.end(),
                0.0
            ) / jitters.size();

        // Minimum and maximum jitter
        const auto [minIt, maxIt] =
            std::minmax_element(
                jitters.begin(),
                jitters.end()
            );

        double totalAbsolute = 0.0;
        double maxAbsolute = 0.0;

        for (double jitter : jitters) {

            const double absolute = std::abs(jitter);

            totalAbsolute += absolute;

            maxAbsolute =
                std::max(maxAbsolute, absolute);
        }

        const double meanAbsolute =
            totalAbsolute / jitters.size();

        std::cout
            << std::fixed
            << std::setprecision(3);

        std::cout
            << "Samples: "
            << jitters.size() << "\n";

        std::cout
            << "Average jitter: "
            << average << " ms\n";

        std::cout
            << "Mean absolute jitter: "
            << meanAbsolute << " ms\n";

        std::cout
            << "Minimum jitter: "
            << *minIt << " ms\n";

        std::cout
            << "Maximum jitter: "
            << *maxIt << " ms\n";

        std::cout
            << "Maximum absolute jitter: "
            << maxAbsolute << " ms\n";
    }
}
