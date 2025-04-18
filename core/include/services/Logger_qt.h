#pragma once
#include <QDateTime>
#include <QString>


namespace Core::Service {
    class Logger_QT {
    public:
        Logger_QT() = default;

        explicit Logger_QT(const QString &str)
            : m_moduleName(str) {
        }

        enum class LogLevel {
            Debug,
            Info,
            Warning,
            Error
        };

        void log(const LogLevel level, const QString &message) const {
            QString levelStr = logLevelToString(level);
            QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
            const QString logMessage = QString("[%1] [%2] %3: %4")
                    .arg(levelStr, timestamp, m_moduleName, message);
            outputToConsole(logMessage);
        }

    private:
        QString m_moduleName;

        static QString logLevelToString(const LogLevel level) {
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

        static void outputToConsole(const QString &logMessage) {
            qDebug() << logMessage;
        }
    };
}
