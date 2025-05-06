//
// Created by cww on 25-4-16.
//
#pragma once
#include "../Song.h"
#include <QLogger.h>
#include <QSqlDatabase>
#include <QObject>
#include <QVector>


namespace Tray::Core {
    class DatabaseManager final : public QObject {
        Q_OBJECT

    public:
        explicit DatabaseManager(const QString &dbPath, const QString &connectionName, QObject *parent = nullptr);

        ~DatabaseManager() override;

        [[nodiscard]] bool isOpen() const;

        bool createTable(const QString &tableName);

        bool deleteTable(const QString &tableName);

        bool insertSong(const QString &tableName, const Song &song);

        bool deleteSong(const QString &tableName, const QString &absolutePath);

        QVector<Song> readAllSongsFromTable(const QString &tableName);

    private:
        static inline const QString FIELD_ID = "id";
        static inline const QString FIELD_FULL_NAME = "fullName";
        static inline const QString FIELD_ABSOLUTE_PATH = "absolutePath";
        static inline const QString FIELD_MUSIC_NAME = "songName";
        static inline const QString FIELD_ARTIST = "artist";
        static inline const QString FIELD_DURATION = "duration";

        static inline const QString TYPE_INTEGER = "INTEGER";
        static inline const QString TYPE_TEXT = "TEXT";
        static inline const QString CONSTRAINT_NOTNULL = "NOT NULL";
        static inline const QString CONSTRAINT_PK_AI = "PRIMARY KEY AUTOINCREMENT";

        static bool isValidTableName(const QString &tableName);
        QSqlDatabase m_databaseConnection;
        QString m_connectionName;
        Log::QLogger Log;
    };
}
