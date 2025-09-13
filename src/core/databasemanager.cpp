//
// Created by cww on 25-4-16.
//
#include <databasemanager/databasemanager.h>
#include <common/trayconfig.h>
#include <log/log.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QRegularExpression>
#include <QUuid>
#include <QDir>


namespace Tray::Core
{
DatabaseManager::DatabaseManager(const QString& connectionName, QObject* parent)
    : QObject(parent)
{
    setObjectName("DatabaseManager");
    m_connectionName = connectionName.isEmpty()
                       ? ("TrayDB_" + QUuid::createUuid().
                           toString(QUuid::WithoutBraces))
                       : connectionName;
    const QFileInfo dbFileInfo(PROJECT_PATH + DB_PATH);
    if (const QDir dbDir = dbFileInfo.absoluteDir(); !dbDir.exists())
    {

        LOG_INFO(QString("Database directory does not exist, creating: %1").arg(dbDir.absolutePath()));
        if (!dbDir.mkpath("."))
        {
            LOG_WARNING("Could not create database directory!");
        }
    }

    if (QSqlDatabase::contains(m_connectionName))
    {
        m_databaseConnection = QSqlDatabase::database(m_connectionName);
    }
    else
    {
        m_databaseConnection = QSqlDatabase::addDatabase("QSQLITE"
            , m_connectionName);
        m_databaseConnection.setDatabaseName(PROJECT_PATH + DB_PATH);
    }

    if (!m_databaseConnection.open())
    {
        LOG_ERROR(QString("Cannot open Database [%1]: Init failed, %2").arg(PROJECT_PATH + DB_PATH, m_databaseConnection.lastError().text()));
    }
    else
    {
        LOG_INFO(QString("Open Database [%1] successfully: connectionName: %2").arg(PROJECT_PATH + DB_PATH, m_connectionName));
    }
}

DatabaseManager::~DatabaseManager()
{
    if (m_databaseConnection.isOpen())
    {
        m_databaseConnection.close();
    }
    m_databaseConnection = QSqlDatabase();
    // Detach current object from connection
    QSqlDatabase::removeDatabase(m_connectionName);
}

bool DatabaseManager::isOpen() const
{
    return m_databaseConnection.isOpen();
}

bool DatabaseManager::isValidTableName(const QString& tableName)
{
    static QRegularExpression re("^[a-zA-Z \\p{Han}]+$");
    return re.match(tableName).hasMatch();
}

bool DatabaseManager::createTable(const QString& tableName)
{
    if (!isValidTableName(tableName))
    {
        LOG_ERROR(QString("Invalid table name: %1").arg(tableName));
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
           .arg(tableName
                , FIELD_ID
                , TYPE_INTEGER
                , CONSTRAINT_PK_AI
                , FIELD_FULL_NAME
                , TYPE_TEXT
                , CONSTRAINT_NOTNULL
                , FIELD_ABSOLUTE_PATH
                , FIELD_MUSIC_NAME
                , FIELD_ARTIST
                , FIELD_DURATION);

    if (!query.exec(sql))
    {
        LOG_ERROR(QString("Create table failed: %1: %2").arg(tableName, query.lastError().text()));
        return false;
    }
    return true;
}

bool DatabaseManager::deleteTable(const QString& tableName)
{
    if (!isValidTableName(tableName))
    {
        LOG_ERROR(QString("Invalid table name: %1").arg(tableName));
        return false;
    }
    QSqlQuery query(m_databaseConnection);
    if (const QString sql = QString("DROP TABLE IF EXISTS %1").arg(tableName); !
        query.exec(sql))
    {
        LOG_ERROR(QString("Drop table failed: %1: %2").arg(tableName, query.lastError().text()));
        return false;
    }
    LOG_INFO(QString("Table dropped successfully (or did not exist): %1").arg(tableName));
    return true;
}

bool DatabaseManager::insertSong(const QString& tableName
                                 , const MusicMetaData& song)
{
    if (!isValidTableName(tableName))
    {
        LOG_ERROR(QString("Invalid table name: %1").arg(tableName));
        return false;
    }

    QSqlQuery query(m_databaseConnection);
    query.
            prepare(QString("INSERT INTO %1 (%2, %3, %4, %5, %6) VALUES (?, ?, ?, ?, ?)")
                   .arg(tableName
                        , FIELD_FULL_NAME
                        , FIELD_ABSOLUTE_PATH
                        , FIELD_MUSIC_NAME
                        , FIELD_ARTIST
                        , FIELD_DURATION));

    query.addBindValue(song.m_title);
    query.addBindValue(song.m_path);
    query.addBindValue(song.m_name);
    query.addBindValue(song.m_artist);
    query.addBindValue(song.m_duration);

    if (!query.exec())
    {
        LOG_ERROR(QString("Insert song failed: %1").arg(query.lastError().text()));
        return false;
    }
    return true;
}

bool DatabaseManager::deleteSongWithTitle(const QString& tableName
                                          , const QString& title)
{
    if (!isValidTableName(tableName))
    {
        LOG_ERROR(QString("Invalid table name: %1").arg(tableName));
        return false;
    }

    QSqlQuery query(m_databaseConnection);
    query.prepare(QString("DELETE FROM %1 WHERE %2 = ?")
                 .arg(tableName, FIELD_FULL_NAME));
    query.addBindValue(title);

    if (!query.exec())
    {
        LOG_ERROR(QString("Delete song failed: %1").arg(query.lastError().text()));
        return false;
    }
    return true;
}

QList<MusicMetaData> DatabaseManager::readAllSongsFromTable(
    const QString& tableName)
{
    QList<MusicMetaData> result;
    if (!isValidTableName(tableName))
    {
        LOG_ERROR(QString("Invalid table name: %1").arg(tableName));
        return result;
    }

    QSqlQuery query(m_databaseConnection);
    const QString sql = QString("SELECT %1, %2, %3, %4, %5 FROM %6")
           .arg(FIELD_FULL_NAME
                , FIELD_ABSOLUTE_PATH
                , FIELD_MUSIC_NAME
                , FIELD_ARTIST
                , FIELD_DURATION
                , tableName);

    if (!query.exec(sql))
    {
        LOG_ERROR(QString("Query failed: %1").arg(query.lastError().text()));
        return result;
    }

    while (query.next())
    {
        MusicMetaData song;
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
