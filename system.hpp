#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#define SPLICING_LOG(trace) std::string(trace)

#include "logger.hpp"

#define LOG_DBUG(CONTENT)                        LOGGER_DEBUG(CONTENT)
#define LOG_INFOR(CONTENT)                       LOGGER_INFO(CONTENT)
#define LOG_CMDINFO(CONTENT)                     LOGGER_CMDINFO(CONTENT)
#define LOG_WARN(CONTENT)                        LOGGER_WARNING(CONTENT)
#define LOG_ERROR(CONTENT)                       LOGGER_ERROR(CONTENT)
#define LOG_FPGA_DEBUG(CONTENT)                  LOGGER_FPGA_DEBUG(CONTENT)

#define SPLICING_LOG(trace) std::string(trace)

#define TRACE_LOG(TRACE_TYPE_BITMASK, COMPONENT, PRIORITY, FORMAT, ...)  LOGGER_LOG(TRACE_TYPE_BITMASK, COMPONENT, PRIORITY, FORMAT, ##__VA_ARGS__)
#define TRACE_LOG_REGISTER_COMPONENT(COMPONENT) LOGGER_COMPONENT_REGISTER(COMPONENT)

#define TRACE_ERROR(TRACE_TYPE_BITMASK, COMPONENT, format, ...) \
    TRACE_LOG(TRACE_TYPE_BITMASK, COMPONENT, 0x01, format, ##__VA_ARGS__)
// log4cpp::Priority::ERROR

#define TRACE_INFO(TRACE_TYPE_BITMASK, COMPONENT, format, ...) \
    TRACE_LOG(TRACE_TYPE_BITMASK, COMPONENT, 0x04, format, ##__VA_ARGS__)
// log4cpp::Priority::INFO

#define TRACE_DEBUG(TRACE_TYPE_BITMASK, COMPONENT, format, ...) \
    TRACE_LOG(TRACE_TYPE_BITMASK, COMPONENT, 0x08 , format, ##__VA_ARGS__)
// log4cpp::Priority::DEBUG

#define TRACE_WARN(TRACE_TYPE_BITMASK, COMPONENT, format, ...) \
    TRACE_LOG(TRACE_TYPE_BITMASK, COMPONENT, 0x02, format, ##__VA_ARGS__)
// log4cpp::Priority::WARN

// #define RADIO_DEBUG(format, ...) TRACE_DEBUG(TraceType::TRACE_RADIO, "", format, ##__VA_ARGS__)
// #define RADIO_INFO(format, ...)  TRACE_INFO(TraceType::TRACE_RADIO, "", format, ##__VA_ARGS__)
// #define RADIO_CMDINFO(format, ...) TRACE_INFO(TraceType::TRACE_RADIO|TraceType::TRACE_CMD, "", format, ##__VA_ARGS__)
// #define RADIO_WARNING(format, ...) TRACE_INFO(TraceType::TRACE_RADIO, "", format, ##__VA_ARGS__)
// #define RADIO_ERROR(format, ...) TRACE_ERROR(TraceType::TRACE_RADIO|TraceType::TRACE_HW, "", format, ##__VA_ARGS__)
// #define CALIB_LOG(format, ...) TRACE_INFO(TraceType::TRACE_CAL, "", format, ##__VA_ARGS__)
// #define FPGA_DEBUG_LOG(format, ...) TRACE_INFO(TraceType::TRACE_FPGA, "", format, ##__VA_ARGS__)

#define TRACE_REGISTER_COMPONENT(COMPONENT) TRACE_LOG_REGISTER_COMPONENT(COMPONENT)
    

#define RADIO_DEBUG(format, ...) \
do\
{\
    char buf[LOG_BUF_SIZE] = { 0 };\
    snprintf(buf, LOG_BUF_SIZE, format, ##__VA_ARGS__);\
    LOG_DBUG(SPLICING_LOG(buf));\
} while (0)

#define RADIO_INFO(format, ...) \
do\
{\
    char buf[LOG_BUF_SIZE] = { 0 };\
    snprintf(buf, LOG_BUF_SIZE, format, ##__VA_ARGS__);\
    LOG_INFOR(SPLICING_LOG(buf));\
} while (0)

#define RADIO_WARNING(format, ...) \
do\
{\
    char buf[LOG_BUF_SIZE] = { 0 };\
    snprintf(buf, LOG_BUF_SIZE, format, ##__VA_ARGS__);\
    LOG_WARN(SPLICING_LOG(buf));\
} while (0)

#define RADIO_ERROR(format, ...) \
do\
{\
    char buf[LOG_BUF_SIZE] = { 0 };\
    snprintf(buf, LOG_BUF_SIZE, format, ##__VA_ARGS__);\
    LOG_ERROR(SPLICING_LOG(buf));\
} while (0)

#endif