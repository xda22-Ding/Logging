#include "logger.hpp"
#include <cstdarg>
#include <iostream>

#define CONFIGURE_FILE_DIR "oru_logger.conf"

Logger::Logger() {
    try {
        log4cpp::PropertyConfigurator::configure(CONFIGURE_FILE_DIR);

        loggerList.reserve(10);
        loggerList = {
            {TraceType::TRACE_RADIO, log4cpp::Category::exists("radioTrace")},
            {TraceType::TRACE_HW, log4cpp::Category::exists("hardwareLog")},
            {TraceType::TRACE_CMD, log4cpp::Category::exists("cmdTrace")},
            {TraceType::TRACE_FPGA, log4cpp::Category::exists("fpgaDebugLog")},
            {TraceType::TRACE_CAL, log4cpp::Category::exists("calTrace")},
        };
    } catch (log4cpp::ConfigureFailure& e) {
        std::cout << "Load log4cpp configure file failed! Error: " << e.what() << std::endl;
    }
}

Logger& Logger::instance() {
    static Logger loggerInstance;
    return loggerInstance;
}

void Logger::log(uint32_t traceTypeBitmask, const std::string& component,
                 log4cpp::Priority::Value priority, const std::string& messageFormat, ...) {
    std::lock_guard<std::mutex> lock(traceMutex);

    for (const auto& [traceType, logger] : loggerList) {
        if ((traceTypeBitmask & static_cast<uint32_t>(traceType)) && logger != nullptr &&
            priority <= logger->getPriority()) {

            va_list args;
            va_start(args, messageFormat);
            vsnprintf(buf, LOG_BUF_SIZE, messageFormat.c_str(), args);
            va_end(args);

            std::string formattedMessage = "[" + component + "] " + buf;
            logger->log(priority, formattedMessage);
        }
    }
}