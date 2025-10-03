//
// Created by cww on 25-4-16.
//
#pragma once
#include <musicmetadata.h>
#include <QSqlDatabase>
#include <QObject>
#include <QVector>

namespace Tray::Core
{
class DatabaseManager final : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseManager(const QString& connectionName
                             , QObject* parent = nullptr);

    ~DatabaseManager() override;

    [[nodiscard]] bool isOpen() const;

    bool createTable(const QString& tableName);

    bool deleteTable(const QString& tableName);

    bool insertSong(const QString& tableName, const MusicMetaData& song);

    bool deleteSongWithTitle(const QString& tableName, const QString& title);

    QList<MusicMetaData> readAllSongsFromTable(const QString& tableName);

private:
    static inline const auto FIELD_ID = QStringLiteral("id");
    static inline const auto FIELD_FULL_NAME = QStringLiteral("fullName");
    static inline const auto FIELD_ABSOLUTE_PATH =
            QStringLiteral("absolutePath");
    static inline const auto FIELD_MUSIC_NAME = QStringLiteral("songName");
    static inline const auto FIELD_ARTIST = QStringLiteral("artist");
    static inline const auto TYPE_INTEGER = QStringLiteral("INTEGER");
    static inline const auto TYPE_TEXT = QStringLiteral("TEXT");
    static inline const auto CONSTRAINT_NOTNULL = QStringLiteral("NOT NULL");
    static inline const auto CONSTRAINT_PK_AI =
            QStringLiteral("PRIMARY KEY AUTOINCREMENT");
    static inline const auto DB_PATH = QStringLiteral("data/music.db");

    QSqlDatabase m_databaseConnection;

    QString m_connectionName;

    static bool isValidTableName(const QString& tableName);
};
}
