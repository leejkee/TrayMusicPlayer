//
// Created by 31305 on 2025/8/24.
//
#pragma once

#include <QString>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QDateTime>

namespace Tray::Log
{
enum class LogLevel
{
    Debug, Info, Warning, Error
};

inline QString logLevelToString(const LogLevel level)
{
    switch (level)
    {
    case LogLevel::Debug:
        return "DEBUG";
    case LogLevel::Info:
        return "INFO";
    case LogLevel::Warning:
        return "WARNING";
    case LogLevel::Error:
        return "ERROR";
    }
    return "UNKNOWN";
}

inline void log(const LogLevel level
                , const QString& moduleName
                , const QString& message)
{
    const QString timestamp = QDateTime::currentDateTime().
            toString("yyyy-MM-dd hh:mm:ss.zzz");
    const QString logMessage = QString("[%1] [%2] [%3] %4")
                              .arg(timestamp)
                              .arg(logLevelToString(level))
                              .arg(moduleName)
                              .arg(message);

    qDebug() << logMessage;
}
} // namespace Tray::Log

#define GET_MODULE_NAME() QFileInfo(__FILE__).fileName()

#define LOG_MESSAGE(level, ...) \
do { \
const QString message {__VA_ARGS__}; \
Tray::Log::log(level, GET_MODULE_NAME(), message); \
} while (0)

#define LOG_DEBUG(...)   LOG_MESSAGE(Tray::Log::LogLevel::Debug, __VA_ARGS__)
#define LOG_INFO(...)    LOG_MESSAGE(Tray::Log::LogLevel::Info, __VA_ARGS__)
#define LOG_WARNING(...) LOG_MESSAGE(Tray::Log::LogLevel::Warning, __VA_ARGS__)
#define LOG_ERROR(...)   LOG_MESSAGE(Tray::Log::LogLevel::Error, __VA_ARGS__)
