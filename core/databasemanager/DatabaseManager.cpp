//
// Created by cww on 25-4-16.
//
#include "DatabaseManager.h"
#include <TrayConfig.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QRegularExpression>
#include <QUuid>


namespace Tray::Core {
    DatabaseManager::DatabaseManager(const QString &connectionName, QObject *parent)
        : QObject(parent) {
        setObjectName("DatabaseManager");
        Log = Log::QLogger(this->objectName());

        m_connectionName = connectionName.isEmpty()
                               ? ("TrayDB_" + QUuid::createUuid().toString(QUuid::WithoutBraces))
                               : connectionName;

        if (QSqlDatabase::contains(m_connectionName)) {
            m_databaseConnection = QSqlDatabase::database(m_connectionName);
        } else {
            m_databaseConnection = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);
            m_databaseConnection.setDatabaseName(PROJECT_PATH + DB_PATH);
        }

        if (!m_databaseConnection.open()) {
            Log.log(Log::QLogger::LogLevel::Error,
                    QString("Cannot open Database [%1]: Init failed, %2").arg(
                        PROJECT_PATH + DB_PATH, m_databaseConnection.lastError().text()));
        } else {
            Log.log(Log::QLogger::LogLevel::Info,
                    QString("Open Database [%1] successfully: connectionName: %2").arg(PROJECT_PATH + DB_PATH,
                    m_connectionName));
        }
    }

    DatabaseManager::~DatabaseManager() {
        if (m_databaseConnection.isOpen()) {
            m_databaseConnection.close();
        }
        m_databaseConnection = QSqlDatabase(); // Detach current object from connection
        QSqlDatabase::removeDatabase(m_connectionName);
    }

    bool DatabaseManager::isOpen() const {
        return m_databaseConnection.isOpen();
    }

    bool DatabaseManager::isValidTableName(const QString &tableName) {
        // ^          - 匹配字符串的开始
        // [          - 字符集合开始
        // a-zA-Z     - 匹配任意英文字母（大写或小写）
        // \          - 匹配一个空格字符
        // \\p{Han}   - 匹配任意中文汉字 (使用 Unicode 属性 Han)
        // ]          - 字符集合结束
        // +          - 匹配前面的字符集合一次或多次 (确保表名不为空)
        // $          - 匹配字符串的结束
        // 这个表达式确保整个字符串只包含英文字母、空格或中文汉字，并且至少有一个字符。
        static QRegularExpression re("^[a-zA-Z \\p{Han}]+$");

        // QRegularExpression::match() 尝试从字符串开头进行匹配。
        // hasMatch() 检查是否找到了一个匹配项。
        // 由于使用了 ^ 和 $，hasMatch() 只有在整个字符串都符合规则时才返回 true。
        return re.match(tableName).hasMatch();
    }

    bool DatabaseManager::createTable(const QString &tableName) {
        if (!isValidTableName(tableName)) {
            Log.log(Log::QLogger::LogLevel::Error, "Invalid table name: " + tableName);
            return false;
        }

        QSqlQuery query(m_databaseConnection);
        const QString sql = QString("CREATE TABLE IF NOT EXISTS %1 ("
                    "%2 %3 %4,"
                    "%5 %6 %7,"
                    "%8 %6 %7,"
                    "%9 %6 %7,"
                    "%10 %6 %7,"
                    "%11 %3 %7)")
                .arg(tableName,
                     FIELD_ID, TYPE_INTEGER, CONSTRAINT_PK_AI,
                     FIELD_FULL_NAME, TYPE_TEXT, CONSTRAINT_NOTNULL,
                     FIELD_ABSOLUTE_PATH,
                     FIELD_MUSIC_NAME,
                     FIELD_ARTIST,
                     FIELD_DURATION);

        if (!query.exec(sql)) {
            Log.log(Log::QLogger::LogLevel::Error,
                    "Create table failed: " + tableName + ": " + query.lastError().text());
            return false;
        }
        return true;
    }

    bool DatabaseManager::insertSong(const QString &tableName, const Song &song) {
        if (!isValidTableName(tableName)) {
            Log.log(Log::QLogger::LogLevel::Error, "Invalid table name: " + tableName);
            return false;
        }

        QSqlQuery query(m_databaseConnection);
        query.prepare(QString("INSERT INTO %1 (%2, %3, %4, %5, %6) VALUES (?, ?, ?, ?, ?)")
            .arg(tableName,
                 FIELD_FULL_NAME,
                 FIELD_ABSOLUTE_PATH,
                 FIELD_MUSIC_NAME,
                 FIELD_ARTIST,
                 FIELD_DURATION));
        query.addBindValue(song.m_title);
        query.addBindValue(song.m_path);
        query.addBindValue(song.m_name);
        query.addBindValue(song.m_artist);
        query.addBindValue(song.m_duration);

        if (!query.exec()) {
            Log.log(Log::QLogger::LogLevel::Error, "Insert song failed: " + query.lastError().text());
            return false;
        }
        return true;
    }

    bool DatabaseManager::deleteSongWithTitle(const QString &tableName, const QString &title) {
        if (!isValidTableName(tableName)) {
            Log.log(Log::QLogger::LogLevel::Error, "Invalid table name: " + tableName);
            return false;
        }

        QSqlQuery query(m_databaseConnection);
        query.prepare(QString("DELETE FROM %1 WHERE %2 = ?")
            .arg(tableName, FIELD_FULL_NAME));
        query.addBindValue(title);

        if (!query.exec()) {
            Log.log(Log::QLogger::LogLevel::Error, "Delete song failed: " + query.lastError().text());
            return false;
        }
        return true;
    }

    QVector<Song> DatabaseManager::readAllSongsFromTable(const QString &tableName) {
        QVector<Song> result;
        if (!isValidTableName(tableName)) {
            Log.log(Log::QLogger::LogLevel::Error, "Invalid table name: " + tableName);
            return result;
        }

        QSqlQuery query(m_databaseConnection);
        const QString sql = QString("SELECT %1, %2, %3, %4, %5 FROM %6")
                .arg(FIELD_FULL_NAME,
                     FIELD_ABSOLUTE_PATH,
                     FIELD_MUSIC_NAME,
                     FIELD_ARTIST,
                     FIELD_DURATION,
                     tableName);

        if (!query.exec(sql)) {
            Log.log(Log::QLogger::LogLevel::Error, "Query failed: " + query.lastError().text());
            return result;
        }

        while (query.next()) {
            Song song;
            QSqlRecord rec = query.record();
            song.m_title = query.value(rec.indexOf(FIELD_FULL_NAME)).toString();
            song.m_path = query.value(rec.indexOf(FIELD_ABSOLUTE_PATH)).toString();
            song.m_name = query.value(rec.indexOf(FIELD_MUSIC_NAME)).toString();
            song.m_artist = query.value(rec.indexOf(FIELD_ARTIST)).toString();
            song.m_duration = query.value(rec.indexOf(FIELD_DURATION)).toInt();
            result.append(song);
        }

        return result;
    }
}
