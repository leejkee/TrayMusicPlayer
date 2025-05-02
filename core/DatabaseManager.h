//
// Created by cww on 25-4-16.
//
#pragma once
#include "Song.h"
#include "QLogger.h"
#include <QSqlDatabase>

namespace Tray::Core {
    class DatabaseManager final : public QObject {
    public:
        explicit DatabaseManager(const QString& dbPath, const QString& connectionName = "main", QObject *parent = nullptr);

        ~DatabaseManager() override;

        [[nodiscard]] bool isOpen() const;
        bool createTable(const QString& tableName);
        bool deleteTable(const QString& tableName);

        bool insertSong(const QString& tableName, const Song& song);
        bool deleteSong(const QString& tableName, const QString& absolutePath); // 可选按路径删除

        QVector<Song> readSongs(const QString& tableName);

    private:
        QSqlDatabase m_database;
        Log::QLogger Log;
    };

}

