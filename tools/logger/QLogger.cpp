//
// Created by cww on 25-5-3.
//
#include "QLogger.h"
namespace Tray::Log {
    void QLogger::log(const LogLevel level, const QString &message) const {
        QString levelStr = logLevelToString(level);
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        const QString logMessage = QString("[%1] [%2] %3: %4")
                .arg(levelStr, timestamp, m_moduleName, message);
        outputToConsole(logMessage);
    }

    QString QLogger::logLevelToString(const LogLevel level) {
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
}
