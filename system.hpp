#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#define SPLICING_LOG(trace) std::string(trace)

#include "logger.hpp"


#define LOG_DBUG(CONTENT)                        LOGGER_DEBUG(CONTENT)
#define LOG_INFOR(CONTENT)                       LOGGER_INFO(CONTENT)
#define LOG_WARN(CONTENT)                        LOGGER_WARNING(CONTENT)
#define LOG_ERROR(CONTENT)                       LOGGER_ERROR(CONTENT)
// TRACE_LOG macro using LOGGER_LOG and passing varadic arguments
#define TRACE_ERROR(TRACE_TYPE_BITMASK, COMPONENT, FORMAT, ...) \
    TRACE_LOG(TRACE_TYPE_BITMASK, COMPONENT, log4cpp::Priority::ERROR, FORMAT, ##__VA_ARGS__)

#define TRACE_INFO(TRACE_TYPE_BITMASK, COMPONENT, FORMAT, ...) \
    TRACE_LOG(TRACE_TYPE_BITMASK, COMPONENT, log4cpp::Priority::INFO, FORMAT, ##__VA_ARGS__)

#define TRACE_DEBUG(TRACE_TYPE_BITMASK, COMPONENT, FORMAT, ...) \
    TRACE_LOG(TRACE_TYPE_BITMASK, COMPONENT, log4cpp::Priority::DEBUG, FORMAT, ##__VA_ARGS__)

#define TRACE_WARN(TRACE_TYPE_BITMASK, COMPONENT, FORMAT, ...) \
    TRACE_LOG(TRACE_TYPE_BITMASK, COMPONENT, log4cpp::Priority::WARN, FORMAT, ##__VA_ARGS__)

// TRACE_LOG macro definition
#define TRACE_LOG(TRACE_TYPE_BITMASK, COMPONENT, PRIORITY, FORMAT, ...) \
    LOGGER_LOG(TRACE_TYPE_BITMASK, COMPONENT, PRIORITY, FORMAT, ##__VA_ARGS__)

    

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