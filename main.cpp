#include "logger.hpp"
#include "system.hpp"

int main() {
    // Initialize the logger instance
    Logger::instance();

    // Log different levels of messages using the TRACE_* macros
    TRACE_INFO(TraceType::TRACE_RADIO, "Main", "Radio logging test: %d", 42);
    TRACE_ERROR(TraceType::TRACE_HW, "Main", "Hardware error encountered.");
    TRACE_DEBUG(TraceType::TRACE_CMD, "Main", "Debugging command: %s", "test_cmd");
    TRACE_WARN(TraceType::TRACE_FPGA, "Main", "FPGA warning: value=%f", 3.14159);

    // Log an example message to ensure proper threshold behavior
    TRACE_INFO(TraceType::TRACE_CAL, "Main", "Calibration info: %s", "Test completed");

    TRACE_DEBUG(TraceType::TRACE_HW, "Main", "This is a DEBUG message");
    TRACE_INFO(TraceType::TRACE_HW, "Main", "This is a INFO message");
    TRACE_ERROR(TraceType::TRACE_HW, "Main", "This is a ERROR message");
    TRACE_ERROR(TraceType::TRACE_HW | TraceType::TRACE_RADIO | TraceType::TRACE_FPGA | TraceType::TRACE_CMD , "Main", "This is a ERROR message that exists in 4 types");

    


    return 0;
}