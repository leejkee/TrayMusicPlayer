//
// Created by cww on 25-4-7.
//
#include "ListCache.h"
#include <TrayConfig.h>
#include <DatabaseManager.h>
#include <QLogger.h>
#include <QDirIterator>


namespace Tray::Core {
    ListCache::ListCache(QObject *parent): QObject(parent) {
        setObjectName(QStringLiteral("ListCache"));
        Log = Log::QLogger(this->objectName());
        Log.log(Log::QLogger::LogLevel::Info, "ListCache: no list has been initialized.");
    }

    ListCache::ListCache(const QStringList &localDir, const QStringList &userListKeys, QObject *parent) : QObject
        (parent) {
        setObjectName(QStringLiteral("ListCache"));
        Log = Log::QLogger(this->objectName());
        initLocalPlaylist(localDir);
        initUserPlaylists(userListKeys);
        Log.log(Log::QLogger::LogLevel::Info, "ListCache: local list and user lists have been initialized.");
    }

    void ListCache::initLocalPlaylist(const QStringList &localDir) {
        QVector<Song> localList;
        for (const auto &filePath: localDir) {
            QDirIterator it(filePath, MUSIC_FILTERS, QDir::Files, QDirIterator::Subdirectories);
            while (it.hasNext()) {
                it.next();
                Song song(it.filePath());
                localList.append(song);
            }
        }
        m_listCache[LOCAL_LIST_KEY] = localList;
    }

    void ListCache::initUserPlaylists(const QStringList &userListKeys) {
        const auto connectName = QStringLiteral("listCache_readList");
        auto dbConnection = DatabaseManager(DB_PATH, connectName);
        for (const auto &key: userListKeys) {
            if (key == LOCAL_LIST_KEY) {
                Log.log(Log::QLogger::LogLevel::Error,
                        "Init user playlist error, invalid list key: " + key +
                        ". 'Local' cannot be used as a user list key.");
                continue;
            }
            if (!m_listCache.contains(key)) {
                m_listCache[key] = dbConnection.readAllSongsFromTable(key);
            }
        }
    }

    QVector<Song> ListCache::findList(const QString &listName) const {
        if (m_listCache.contains(listName)) {
            return m_listCache.value(listName);
        }
        Log.log(Log::QLogger::LogLevel::Warning, "Cannot find such PlayList called [" + listName + "]");
        return {};
    }

    void ListCache::newUserPlaylist(const QString &key) {
        if (key == LOCAL_LIST_KEY) {
            Log.log(Log::QLogger::LogLevel::Error,
                    "New user playlist error, invalid list key: " + key +
                    ". 'Local' cannot be used as a user list key.");
            return;
        }
        if (!m_listCache.contains(key)) {
            m_listCache[key] = {};
        } else {
            Log.log(Log::QLogger::LogLevel::Error, "Cannot add list key: '" + key + "' already exists in the cache.");
        }
    }

    QStringList ListCache::getMusicTitleList(const QString &name) const {
        QStringList list{};
        if (m_listCache.contains(name)) {
            const auto musicList = m_listCache.value(name);
            for (const auto &music: musicList) {
                list.append(music.m_title);
            }
        }
        else {
            Log.log(Log::QLogger::LogLevel::Error, "No such music list called : " + name + " return an empty title list.");
        }
        return list;
    }

    void ListCache::insertMusicToList(const QString &key, const Song &song) {
        if (m_listCache.contains(key)) {
            m_listCache[key].append(song);
            const auto dbconnectionName = "insert_to_" + key;
            auto dbconnection = DatabaseManager(DB_PATH, dbconnectionName);
            if (dbconnection.insertSong(key, song)) {
                Log.log(Log::QLogger::LogLevel::Info, QString("Music '%1' has been added to '%2'.").arg(song.m_title, key));
            }
            Q_EMIT signalMusicInserted(key, song);
        } else {
            Log.log(Log::QLogger::LogLevel::Error, "No such playlist [" + key + "]");
        }
    }
}
