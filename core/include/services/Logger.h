#pragma once

#define QT
#if defined(QT)
#include <QString>
#include <QDateTime>

namespace Core::Service {
class Logger {
public:
    Logger();
    Logger(const std::string);
    Logger(const Logger&) = delete;

    enum class LogLevel {
        Debug,
        Info,
        Warning,
        Error
    };

    void log(LogLevel level, const QString& message) const
    {
        QString levelStr = logLevelToString(level);
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        QString logMessage = QString("[%1] [%2] %3: %4")
                                 .arg(levelStr, timestamp, m_moduleName, message);
        outputToConsole(logMessage);
    }

private:
    QString m_moduleName;
    static QString logLevelToString(LogLevel level)
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

    void outputToConsole(const QString& logMessage) const
    {
        qDebug() << logMessage;
    }
};
#else

#include <string>

#endif
}