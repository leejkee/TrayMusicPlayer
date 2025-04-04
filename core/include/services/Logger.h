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
    void logout(LogLevel, const std::string& message) const;
    [[nodiscard]] std::string log(LogLevel, const std::string& message) const;
    static std::string logLevelToString(LogLevel level);

private:
    std::string m_moduleName;
    static std::string getCurrentTimestamp(); 
    static void outputToConsole(const std::string& logMessage) ;

};
}
