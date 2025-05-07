#pragma once
#include <QDateTime>
#include <QString>

namespace Tray::Log {
    class QLogger {
    public:
        QLogger() = default;

        explicit QLogger(const QString &str)
            : m_moduleName(str) {
        }

        enum class LogLevel {
            Debug,
            Info,
            Warning,
            Error
        };

        void log(LogLevel level, const QString &message) const;

        static QString boolToString(const bool b) {
            return b ? "true" : "false";
        }

    private:
        QString m_moduleName;

        static QString logLevelToString(LogLevel level);

        static void outputToConsole(const QString &logMessage) {
            qDebug() << logMessage;
        }

    };
}
