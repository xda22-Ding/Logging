#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <vector>
#include <string>
#include <cstdint>
#include <log4cpp/Category.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <mutex>
#include <unordered_set>
#include <unordered_map>

// Define TraceType as a bitmask-friendly enum
enum TraceType : uint32_t {
    TRACE_RADIO = 0x01,   // for radio trace log file
    TRACE_HW    = 0x02,   // for HW trace log file
    TRACE_CMD   = 0x04,   // for CMD trace log file
    TRACE_FPGA  = 0x08,   // for FPGA trace log file
    TRACE_CAL   = 0x10    // for Calibration trace log file
};

// Define log type enum
enum LogType : uint32_t {
    ERROR = 0x01,
    WARN  = 0x02,
    INFO  = 0x04,
    DEBUG = 0x08
};

#define LOGGER_DEBUG(trace)      Logger::instance().debug(trace)
#define LOGGER_INFO(trace)       Logger::instance().info(trace)
#define LOGGER_CMDINFO(trace)    Logger::instance().cmdinfo(trace)
#define LOGGER_WARNING(trace)    Logger::instance().warning(trace)
#define LOGGER_ERROR(trace)      Logger::instance().error(trace)
#define SCREEN_PRINT(trace)      Logger::instance().terminalPrint(trace)
#define LOGGER_FPGA_DEBUG(trace)      Logger::instance().fpgaDebugInfo(trace)

#define LOGGER_LOG(TRACE_TYPE_BITMASK, COMPONENT, PRIORITY, FORMAT, ...) \
     	Logger::instance().log(TRACE_TYPE_BITMASK, COMPONENT, PRIORITY, FORMAT, ##__VA_ARGS__)

#define LOGGER_COMPONENT_REGISTER(COMPONENT) Logger::instance().registerLogComponent(COMPONENT)

class Logger
{
    public:
        static Logger& instance();
        static int cmdHandler(std::string cmd);

        void registerComponent(const std::string& component);
        void log(const uint32_t traceTypeBitmask, const std::string& component,
                 const uint32_t level, const std::string& messageFormat, ...);

        ~Logger();

        void debug(const std::string& trace);
        void info(const std::string& trace);
        void warning(const std::string& trace);
        void error(const std::string& trace);

private:

        Logger();

        bool isLogEnable(const std::string& component, const uint32_t& logLevel);
        log4cpp::Priority::Value getLog4CppPriorityVal(const uint32_t level);

        // data member
        static std::vector<std::pair<TraceType, log4cpp::Category*>> loggerFileList;
        static std::unordered_set<std::string> componentSet;

        static std::unordered_map<std::string, uint32_t> componentPriorityMap;
        static std::recursive_mutex loggerMutex;

        static uint32_t defaultLogLevel;

        #define LOG_BUF_SIZE 1024
        char buf[LOG_BUF_SIZE];
};

#endif // __LOGGER_H__
