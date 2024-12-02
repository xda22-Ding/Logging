#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include <log4cpp/Category.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <vector>
#include <mutex>
#include <string>
#include <cstdint>

#define LOGGER_LOG(TRACE_TYPE_BITMASK, COMPONENT, PRIORITY, FORMAT, ...) \
    Logger::instance().log(TRACE_TYPE_BITMASK, COMPONENT, PRIORITY, FORMAT, ##__VA_ARGS__)

#define LOG_BUF_SIZE 1024

enum TraceType : uint32_t {
    TRACE_RADIO = 0x1,
    TRACE_HW = 0x2,
    TRACE_CMD = 0x4,
    TRACE_FPGA = 0x8,
    TRACE_CAL = 0x10,
};

class Logger {
public:
    static Logger& instance();

    void log(uint32_t traceTypeBitmask, const std::string& component,
             log4cpp::Priority::Value priority, const std::string& messageFormat, ...);

private:
    Logger();

    std::vector<std::pair<TraceType, log4cpp::Category*>> loggerList;
    std::mutex traceMutex;
    char buf[LOG_BUF_SIZE];  // Global buffer for message formatting
};

#endif // __LOGGER_HPP__
