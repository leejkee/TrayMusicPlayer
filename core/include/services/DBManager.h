//
// Created by cww on 25-4-16.
//
#pragma once
#include <Song.h>
#include <Logger_qt.h>
#include <QSqlDatabase>
#include <QObject>


namespace Core::Service {
    class DBManager final : public QObject {
    public:
        explicit DBManager(const QString &dbPath, QObject *parent = nullptr);

        ~DBManager() override;

        // void createTable(const QString &tableName) const;
        //
        // [[nodiscard]] QList<Song> getMusicList(const QString &tableName) const;
        //
        // void insertSongToTable(const QString &tableName, const Song &song) const;
        //
        // void delViaName(const QString &tableName, const QString &songName) const;
        bool isOpen() const;

    private:
        QSqlDatabase m_db;
        Logger_QT Log;
    };
}
