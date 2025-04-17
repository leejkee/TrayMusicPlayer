//
// Created by cww on 25-4-16.
//
#pragma once
#include <Song.h>
#include <Logger_qt.h>
#include <QSqlDatabase>

namespace Core::Service {
    class DatabaseManager final : public QObject {
    public:
        explicit DatabaseManager(const QString& dbPath, const QString& connectionName = "main", QObject *parent = nullptr);

        ~DatabaseManager() override;

        [[nodiscard]] bool isOpen() const;
        bool createTable(const QString& tableName);
        bool deleteTable(const QString& tableName);

        bool insertSong(const QString& tableName, const Song& song);
        bool deleteSong(const QString& tableName, const QString& absolutePath); // 可选按路径删除

        QVector<Song> loadAllSongs(const QString& tableName);

    private:
        QSqlDatabase m_database;
        Logger_QT Log;
    };

}

