#include <Logger.h>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

namespace Core::Service {
Logger::Logger(const std::string& module)
{
    if (module.empty()) {
        outputToConsole("[ERROR]: init error, null string");
    }
}

void Logger::logout(const LogLevel level, const std::string& message) const
{
    outputToConsole(log(level, message));
}

std::string Logger::log(const LogLevel level, const std::string& message) const {
    const std::string logLevel = logLevelToString(level);
    const std::string timestamp = getCurrentTimestamp();

    auto str = "[" + logLevel + "] [" + timestamp + "] "
        + m_moduleName + ": " + message + "\n";
    return str;
}

std::string Logger::logLevelToString(const LogLevel level)
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
void Logger::outputToConsole(const std::string& logMessage)
{
    std::cerr << logMessage;
}

}