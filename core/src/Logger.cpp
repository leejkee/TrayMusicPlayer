#include <Logger.h>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

namespace Core::Service {
Logger::Logger(const std::string& module)
    : m_moduleName(module)
{
}

void Logger::logout(const LogLevel level, const std::string& message)
{
    outputToConsole(log(level, message));
}

std::string Logger::log(const LogLevel level, const std::string& message)
{
    const std::string logLevel = logLevelToString(level);
    std::string timestamp = getCurrentTimestamp();

    auto str = "[" + logLevel + "] [" + timestamp + "] "
        + m_moduleName + ": " + message + "\n";
    return str;
}

std::string Logger::logLevelToString(LogLevel level)
{
    switch (level) {
    case LogLevel::Debug:
        return "DEBUG";
    case LogLevel::Info:
        return "INFO";
    case LogLevel::Warning:
        return "WARNING";
    case LogLevel::Error:
        return "ERROR";
    default:
        return "UNKNOWN";
    }
}

std::string Logger::getCurrentTimestamp()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
void Logger::outputToConsole(const std::string& logMessage) const
{
    std::cerr << logMessage;
}

}