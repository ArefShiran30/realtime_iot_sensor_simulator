//
// Created by Aref Shiran on 10/13/26.
//

#ifndef REALTIME_IOT_SENSOR_SIMULATOR_THREADSAFEQUEUE_H
#define REALTIME_IOT_SENSOR_SIMULATOR_THREADSAFEQUEUE_H

#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <utility>

// Generic thread-safe queue
// T can be SensorReading or another data type.
template <typename T>
class ThreadSafeQueue {

private:
    // Stores the queued data
    std::queue<T> dataQueue;

    // Protects shared queue access
    mutable std::mutex queueMutex;

    // Wakes consumers when data becomes available
    std::condition_variable condition;

    // Indicates that no more items will be added
    bool closed = false;

public:
    ThreadSafeQueue() = default;

    // Queue cannot be copied
    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

    // Add a new item to the queue
    // Returns false if the queue has been closed.
    bool push(T item) {

        {
            std::lock_guard<std::mutex> lock(queueMutex);

            if (closed) {
                return false;
            }

            dataQueue.push(std::move(item));
        }

        // Notify one waiting consumer
        condition.notify_one();

        return true;
    }

    // Wait until data is available or queue is closed
    // Returns false when closed and empty.
    bool waitAndPop(T& result) {

        std::unique_lock<std::mutex> lock(queueMutex);

        // Wait without continuously using CPU
        condition.wait(lock, [this] {
            return closed || !dataQueue.empty();
        });

        if (dataQueue.empty()) {
            return false;
        }

        result = std::move(dataQueue.front());
        dataQueue.pop();

        return true;
    }

    // Stop accepting new items and wake consumers
    void close() {

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            closed = true;
        }

        condition.notify_all();
    }

    // Check whether the queue is empty
    bool empty() const {

        std::lock_guard<std::mutex> lock(queueMutex);

        return dataQueue.empty();
    }

    // Get number of pending items
    std::size_t size() const {

        std::lock_guard<std::mutex> lock(queueMutex);

        return dataQueue.size();
    }
};

#endif //REALTIME_IOT_SENSOR_SIMULATOR_THREADSAFEQUEUE_H
