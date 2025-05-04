//
// Created by cww on 25-4-16.
//
#include "DatabaseManager.h"
#include <QSqlError>
#include <QSqlQuery>

namespace Tray::Core {
    DatabaseManager::DatabaseManager(const QString &dbPath, const QString &connectionName, QObject *parent) : QObject(
        parent) {
        setObjectName("DatabaseManager");
        Log = Log::QLogger(this->objectName());
        if (QSqlDatabase::contains(connectionName)) {
            m_database = QSqlDatabase::database(connectionName);
        } else {
            m_database = QSqlDatabase::addDatabase("QSQLITE", connectionName);
            m_database.setDatabaseName(dbPath);
        }

        if (!m_database.open()) {
            Log.log(Log::QLogger::LogLevel::Error, QString("Cannot open Database [%1]: Init failed, %2").arg(dbPath,
                        m_database.lastError().text()));
        } else {
            Log.log(Log::QLogger::LogLevel::Info, QString("Open Database [%1] successfully: connectionName: %2").arg
                    (dbPath, connectionName));
        }
    }

    bool DatabaseManager::isOpen() const {
        return m_database.isOpen();
    }

    bool DatabaseManager::createTable(const QString &tableName) {
        QSqlQuery query(m_database);
        const QString sql = QString(
            "CREATE TABLE IF NOT EXISTS %1 ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "fullName TEXT NOT NULL,"
            "absolutePath TEXT NOT NULL,"
            "songName TEXT NOT NULL,"
            "artist TEXT NOT NULL,"
            "duration INTEGER NOT NULL)").arg(tableName);

        if (!query.exec(sql)) {
            Log.log(Log::QLogger::LogLevel::Error, "create table failed: " + tableName + ": " + query.lastError().text());
            return false;
        }
        return true;
    }


    bool DatabaseManager::deleteTable(const QString &tableName) {
        QSqlQuery query(m_database);
        const QString sql = QString("DROP TABLE IF EXISTS %1").arg(tableName);

        if (!query.exec(sql)) {
            Log.log(Log::QLogger::LogLevel::Error, "delete table failed: " + tableName + ": " + query.lastError().text());
            return false;
        }
        return true;
    }

    bool DatabaseManager::insertSong(const QString &tableName, const Song &song) {
        QSqlQuery query(m_database);
        query.prepare(QString("INSERT INTO %1 (fullName, absolutePath, songName, artist, duration) "
            "VALUES (?, ?, ?, ?, ?)").arg(tableName));
        query.addBindValue(song.m_title);
        query.addBindValue(song.m_path);
        query.addBindValue(song.m_name);
        query.addBindValue(song.m_artist);
        query.addBindValue(song.m_duration);

        if (!query.exec()) {
            Log.log(Log::QLogger::LogLevel::Error, "Insert song failed: " +  query.lastError().text());
            return false;
        }
        return true;
    }

    bool DatabaseManager::deleteSong(const QString &tableName, const QString &absolutePath) {
        QSqlQuery query(m_database);
        query.prepare(QString("DELETE FROM %1 WHERE absolutePath = ?").arg(tableName));
        query.addBindValue(absolutePath);

        if (!query.exec()) {
            Log.log(Log::QLogger::LogLevel::Error, "Delete song failed: " + query.lastError().text());
            return false;
        }
        return true;
    }

    QVector<Song> DatabaseManager::readSongs(const QString &tableName) {
        QVector<Song> result;
        QSqlQuery query(m_database);

        const QString sql = QString("SELECT fullName, absolutePath, songName, artist, duration FROM %1").arg(tableName);
        if (!query.exec(sql)) {
            Log.log(Log::QLogger::LogLevel::Error, "Query failed: " + query.lastError().text());
            return result;
        }

        while (query.next()) {
            Song song;
            song.m_path = query.value(1).toString();
            song.m_title = query.value(0).toString();
            song.m_name = query.value(2).toString();
            song.m_artist = query.value(3).toString();
            song.m_duration = query.value(4).toInt();
            result.append(song);
        }
        return result;
    }

    DatabaseManager::~DatabaseManager() {
        if (m_database.isOpen()) {
            m_database.close();
        }
    }
}
