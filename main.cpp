#include "logger.hpp"
#include "system.hpp"
#include <thread>
#include <vector>

void testNewAPIs() {
    // Log different levels of messages using the TRACE_* macros
    TRACE_INFO(TraceType::TRACE_RADIO, "Main", "Radio logging test: %d", 42);
    TRACE_ERROR(TraceType::TRACE_HW, "Main", "Hardware error encountered.");
    TRACE_DEBUG(TraceType::TRACE_CMD, "Main", "Debugging command: %s", "test_cmd");
    TRACE_WARN(TraceType::TRACE_FPGA, "Main", "FPGA warning: value=%f", 3.14159);

    // Log an example message to ensure proper threshold behavior
    TRACE_INFO(TraceType::TRACE_CAL, "Main", "Calibration info: %s", "Test completed");

    // Additional logs to test thread safety
    TRACE_DEBUG(TraceType::TRACE_HW, "Main", "This is a DEBUG message");
    TRACE_INFO(TraceType::TRACE_HW, "Main", "This is an INFO message");
    TRACE_ERROR(TraceType::TRACE_HW, "Main", "This is an ERROR message");
    TRACE_ERROR(TraceType::TRACE_HW | TraceType::TRACE_RADIO | TraceType::TRACE_FPGA | TraceType::TRACE_CMD,
                "Main", "This is an ERROR message that exists in 4 types");
}

void testOldAPIs() {
    // Test old RADIO_* macros for thread safety
    RADIO_INFO("Old RADIO INFO: %s", "Testing old API info");
    RADIO_WARNING("Old RADIO WARNING: %s", "Testing old API warning");
    RADIO_ERROR("Old RADIO ERROR: %s", "Testing old API error");
}

void runMultithreadedTest() {
    std::vector<std::thread> threads;

    // Create multiple threads to simulate concurrent logging
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([]() {
            Logger::instance();
            testNewAPIs();
            testOldAPIs();
        });
    }

    // Join all threads
    for (auto& t : threads) {
        t.join();
    }
}

int main() {
    // Initialize the logger instance
    Logger::instance();

    // Run multithreaded test
    runMultithreadedTest();

    return 0;
}