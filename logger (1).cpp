#include "logger.hpp"
#include <cstdarg>
#include <iostream>

using namespace std;

#define CONFIGURE_FILE_DIR "oru_logger.conf"

#define  SUCCESS 0
#define  FAILED -1
#define SHELL_PRINT(AAA) (std::cout << AAA)

std::vector<std::pair<TraceType, log4cpp::Category*>> Logger::loggerFileList{};

std::unordered_set<std::string> Logger::componentSet;
std::unordered_map<std::string, uint32_t> Logger::componentPriorityMap;

std::recursive_mutex Logger::loggerMutex;
uint32_t Logger::defaultLogLevel = 0;

void split(vector<string>& paramList, const string& str, char separator = ' ')
{
string::size_type start = 0;
string::size_type end = start;

while (start < str.length())
{
    if (str.at(start) == separator)
    {
        start++;
        continue;
    }
    if (str.at(start) == '\"')
    {
        end = str.find_first_of('\"', start + 1);
        if (end == string::npos)
        {
            return;
        }
        paramList.emplace_back(str.substr(start + 1, end - start - 1));
        start = end + 1;
        continue;
    }
    end = str.find_first_of(separator, start + 1);
    if (end == string::npos)
    {
        paramList.emplace_back(str.substr(start));
        return;
    }
    paramList.emplace_back(str.substr(start, end - start));
    start = end + 1;
}
}

Logger::Logger()
{
    try
    {
        std::lock_guard<std::recursive_mutex> lock(loggerMutex);
        log4cpp::PropertyConfigurator::configure(CONFIGURE_FILE_DIR);

        // REGIST_SHELL_CMD("logger", cmdHandler);

        // later on will consider use below API from log4Cpp 
        // log4cpp::Category::getCurrentCategories();
        loggerFileList.reserve(10);
        loggerFileList = {
            {TraceType::TRACE_RADIO, log4cpp::Category::exists("radioTrace")},
            {TraceType::TRACE_HW, log4cpp::Category::exists("hardwareLog")},
            {TraceType::TRACE_CMD, log4cpp::Category::exists("cmdTrace")},
            {TraceType::TRACE_FPGA, log4cpp::Category::exists("fpgaDebugLog")},
            {TraceType::TRACE_CAL, log4cpp::Category::exists("calTrace")}};

        defaultLogLevel = LogType::ERROR | LogType::WARN | LogType::INFO;
    }
    catch (log4cpp::ConfigureFailure& e)
    {
        cout << "load log4cpp configure file fail! error reason: " << e.what() << endl;
    }    
}

Logger& Logger::instance()
{
    static Logger instance;
    return instance;
}

void Logger::debug(const std::string& trace)
{
    if (trace.empty())
    {
        return;
    }
    log4cpp::Category& radioTrace = log4cpp::Category::getInstance("radioTrace");
    radioTrace.debug(trace);
}

void Logger::info(const std::string& trace)
{
    if (trace.empty())
    {
        return;
    }
    log4cpp::Category& radioTrace = log4cpp::Category::getInstance("radioTrace");
    radioTrace.info(trace);
}

Logger::~Logger()
{
    std::lock_guard<std::recursive_mutex> lock(loggerMutex);
    componentSet.clear();
    componentPriorityMap.clear();
    loggerFileList.clear();
    log4cpp::Category::shutdown();
}

void Logger::warning(const std::string& trace)
{
    if (trace.empty())
    {
        return;
    }
    log4cpp::Category& radioTrace = log4cpp::Category::getInstance("radioTrace");
    radioTrace.warn(trace);
}

void Logger::error(const std::string& trace)
{
    log4cpp::Category& radioTrace = log4cpp::Category::getInstance("radioTrace");
    log4cpp::Category& hardwareLog = log4cpp::Category::getInstance("hardwareLog");
    radioTrace.error(trace);
    hardwareLog.error(trace);
}


bool Logger::isLogEnable(const std::string& component, const uint32_t& logLevel)
{
    bool logEnabled = false;

    std::lock_guard<std::recursive_mutex> lock(loggerMutex);

    registerComponent(component); // this part is temp

    if(componentSet.find(component) != componentSet.end())
    {
        auto it = componentPriorityMap.find(component);
        uint32_t compareWithLogLevel = (it != componentPriorityMap.end()) ? it->second : defaultLogLevel;
        logEnabled = (logLevel & compareWithLogLevel) ? true : false;
    }

    return logEnabled;
}

log4cpp::Priority::Value Logger::getLog4CppPriorityVal(const uint32_t level)
{
    log4cpp::Priority::Value val;

    switch(level)
    {
        case 0x01:
            val = 300;
            break;
        case 0x02:
            val = 400;
            break;
        case 0x04:
            val = 600;
            break;
        case 0x08:
            val = 700;
            break;
    }            
    return val;
}

void Logger::registerComponent(const std::string& component)
{
    std::lock_guard<std::recursive_mutex> lock(loggerMutex);
    componentSet.insert(component);
}

void Logger::log(const uint32_t traceTypeBitmask, const std::string& component,
    const uint32_t level, const std::string& format, ...)
{
    if(isLogEnable(component, level))
    {
        auto priority = getLog4CppPriorityVal(level);
        for (const auto& [traceType, logger] : loggerFileList)
        {
            if ((traceTypeBitmask & static_cast<uint32_t>(traceType)) &&
                (logger != nullptr) &&
                (priority <= static_cast<uint32_t>(logger->getPriority())))
            {
                va_list args;
                va_start(args, format);
                vsnprintf(buf, LOG_BUF_SIZE, format.c_str(), args);
                va_end(args);

                const std::string msg = "[" + component + "] " + buf;
                logger->log(priority, msg);
            }
        }
    }
}

int Logger::cmdHandler(std::string cmd)
{
    int returnVale = FAILED; 

    ostringstream oss;
    vector<string> arguments;
    split(arguments, cmd);

    oss << "usage:\n\t"
        << "logger help\n\t"
        << "logger list_components\n\t"
        << "logger set_component_level <componentName> <level>\n\t"
        << "logger set_component_level_default <componentName>\n\t"
        << "logger set_all_components_level_default\n\t"
        << "logger set_default_level_value <level>\n\t"
        << "  level : uint16_t type, Default level is set to Error / Warning / Info \n\t"
        << "  bit 0 : Error\n\t"
        << "  bit 1 : Warning\n\t"
        << "  bit 2 : Info\n\t"
        << "  bit 3 : Debug\n";

    std::string usage = oss.str();

    std::lock_guard<std::recursive_mutex> lock(loggerMutex);

    try
    {
        if (arguments.size() < 2)
        {
            SHELL_PRINT("Invalid argument. Use 'logger help' for usage.\n" + usage);
        }
        else if (arguments.at(1) == "help")
        {
            SHELL_PRINT(usage);
            returnVale = SUCCESS;
        }
        else if (arguments.at(1) == "list_components")
        {
            std::ostringstream componentStream("List of logging components are :\n{\n");
            for (const auto& component : componentSet)
            {
                componentStream << " " << component;

                auto it = componentPriorityMap.find(component);
                if(it != componentPriorityMap.end())
                {
                    componentStream << " Level : 0x" << std::hex << it->second;
                }

                componentStream  << "\n";
            }

            componentStream << "}\n";
            SHELL_PRINT(componentStream.str());
            returnVale = SUCCESS;
        }
        else if (arguments.at(1) == "set_component_level")
        {
            if(componentSet.find(arguments.at(2)) != componentSet.end())
            {
                unsigned long level = strtoul(arguments.at(3).c_str(), NULL, 16);
                componentPriorityMap[arguments.at(2)] = static_cast<uint32_t>(level);
                returnVale = SUCCESS;
            }
            else
            {
                SHELL_PRINT("Invalid component name. Use 'logger list_components'\n");
            }
        }
        else if (arguments.at(1) == "set_component_level_default")
        {
            auto it = componentPriorityMap.find(arguments.at(2));
            if(it != componentPriorityMap.end())
            {
                componentPriorityMap.erase(it);
            }

            returnVale = SUCCESS;
        }
        else if (arguments.at(1) == "set_all_components_level_default")
        {
            componentPriorityMap.clear();
            returnVale = SUCCESS;
        }
        else if (arguments.at(1) == "set_default_level_value")
        {
            unsigned long level = strtoul(arguments.at(3).c_str(), NULL, 16);
            defaultLogLevel = static_cast<uint32_t>(level);
            returnVale = SUCCESS;
        }
        else
        {
            SHELL_PRINT("Unknown command or incorrect arguments. Use 'logger help' for usage.\n" + usage);
        }
    } 
    catch (...)
    {
        SHELL_PRINT("Invalid arguments. Use 'logger help' for usage.\n" + usage);
    }

    return returnVale;
}
