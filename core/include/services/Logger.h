#pragma once

#include <string>
namespace Core::Service {

class Logger {
public:
    explicit Logger(const std::string&);
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    enum class LogLevel {
        Debug,
        Info,
        Warning,
        Error
    };
    void logout(const LogLevel, const std::string& message);
    std::string log(const LogLevel, const std::string& message);
    static std::string logLevelToString(LogLevel level);

private:
    std::string m_moduleName;
    static std::string getCurrentTimestamp(); 
    void outputToConsole(const std::string& logMessage) const;

};
}
