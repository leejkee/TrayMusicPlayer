//
// Created by cww on 25-4-16.
//
#include <DBManager.h>
#include <QSqlError>
#include <QSqlQuery>


namespace Core::Service {
    DBManager::DBManager(const QString &dbPath, QObject *parent) : QObject(parent) {
        setObjectName("DBManager");
        Log = Logger_QT(this->objectName());
        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName(dbPath);
        if (!m_db.open()) {
            Log.log(Logger_QT::LogLevel::Error, QString("Cannot open Database [%1]: Init failed, %2").arg(dbPath,
                        m_db.lastError().text()));
        }
    };


    DBManager::~DBManager() {
        if (m_db.isOpen()) {
            m_db.close();
        }
    }

    bool DBManager::isOpen() const {
        return m_db.isOpen();
    }


    // void DBManager::createTable(const QString &tableName) const {
    //     if (!m_db.isOpen()) {
    //         qDebug() << "Database is not open" << m_db.lastError().text();
    //         return;
    //     }
    //     if (m_db.tables().contains(tableName)) {
    //         qDebug() << "Table " << tableName << " exists.";
    //         return;
    //     }
    //     // if (tableName.isEmpty() || tableName.at(0).isDigit()) {
    //     //     qDebug() << "Table Name: " << tableName << " is invalid.";
    //     //     return;
    //     // }
    //     qDebug() << "Table " << tableName << " created.";
    //     const QString COLUMN_ID = "id";
    //     const QString COLUMN_PATH = "path";
    //     const QString COLUMN_NAME = "name";
    //     const QString COLUMN_DURATION = "duration";
    //     // create table and add list to table
    //     const QString queryCreateTable{
    //         QString("CREATE TABLE IF NOT EXISTS %1 ("
    //             "%2 INTEGER PRIMARY KEY AUTOINCREMENT,"
    //             "%3 TEXT NOT NULL,"
    //             "%4 TEXT,"
    //             "%5 INTEGER)")
    //         .arg(tableName, COLUMN_ID, COLUMN_PATH, COLUMN_NAME, COLUMN_DURATION)
    //     };
    //     if (QSqlQuery query(m_db); !query.exec(queryCreateTable)) {
    //         qDebug() << "Failed to create table:" << query.lastError().text();
    //     } else {
    //         qDebug() << "Table created successfully!";
    //     }
    // }
    //
    // QList<Song> DBManager::getMusicList(const QString &tableName) const {
    //     QList<Song> songList{};
    //
    //     if (!m_db.isOpen()) {
    //         qDebug() << "Database is not open" << m_db.lastError().text();
    //         return songList;
    //     }
    //
    //     QSqlQuery query(m_db);
    //     const QString queryString = QString("SELECT path, name, duration FROM %1").arg(tableName);
    //
    //     if (!query.exec(queryString)) {
    //         qDebug() << "Failed to query table:" << query.lastError().text();
    //         return songList;
    //     }
    //     while (query.next()) {
    //         const QString path = query.value(1).toString();
    //         const QString name = query.value(2).toString();
    //         const int duration = query.value(3).toInt();
    //         songList.append(Song(name, path, duration));
    //     }
    //     return songList;
    // }
    //
    // void DBManager::insertSongToTable(const QString &tableName, const Song &song) const {
    //     if (!m_db.isOpen()) {
    //         qDebug() << "Database is not open" << m_db.lastError().text();
    //         return;
    //     }
    //
    //     QSqlQuery query(m_db);
    //     const QString insertQuery = QString(
    //         "INSERT INTO %1 (path, name, duration) VALUES (:path, :name, :duration)"
    //     ).arg(tableName);
    //
    //     query.prepare(insertQuery);
    //     query.bindValue(":path", song.getPath());
    //     query.bindValue(":name", song.getFullName());
    //     query.bindValue(":duration", song.getDuration());
    //
    //     if (!query.exec()) {
    //         qDebug() << "Failed to insert song into table:" << query.lastError().text();
    //     } else {
    //         qDebug() << "Song inserted successfully!";
    //     }
    // }
    //
    // void DBManager::delViaName(const QString &tableName, const QString &songName) const {
    //     if (!m_db.isOpen()) {
    //         qDebug() << "Database is not open" << m_db.lastError().text();
    //     }
    //
    //     QSqlQuery query(m_db);
    //     auto sql = QStringLiteral("DELETE FROM %1 WHERE name = :songName");
    //     sql = sql.arg(tableName);
    //     query.prepare(sql);
    //     query.bindValue(":songName", songName);
    //     if (!query.exec()) {
    //         qDebug() << "Failed to delete song from table:" << query.lastError().text();
    //     } else {
    //         qDebug() << "Song" << songName << "deleted successfully!";
    //     }
    // }
}
