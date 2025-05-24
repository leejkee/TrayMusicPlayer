//
// Created by cww on 25-4-7.
//
#include "ListCache.h"
#include <TrayConfig.h>
#include <DatabaseManager.h>
#include <QLogger.h>
#include <QDirIterator>
#include <algorithm>


namespace Tray::Core {
    ListCache::ListCache(QObject *parent): QObject(parent) {
        setObjectName(QStringLiteral("ListCache"));
        Log = Log::QLogger(this->objectName());
        Log.log(Log::QLogger::LogLevel::Info, "Constructor: no list has been initialized.");
    }

    ListCache::ListCache(const QStringList &localDir, const QStringList &userListKeys, QObject *parent) : QObject
        (parent) {
        setObjectName(QStringLiteral("ListCache"));
        Log = Log::QLogger(this->objectName());
        init(localDir, userListKeys);
        Log.log(Log::QLogger::LogLevel::Info, "Constructor: local list and user lists have been initialized.");
    }


    void ListCache::init(const QStringList &localDir, const QStringList &userListKeys) {
        initLocalPlaylist(localDir);
        initUserPlaylists(userListKeys);
        Q_EMIT signalInitCompleted();
    }

    void ListCache::initLocalPlaylist(const QStringList &localDir) {
        QList<Song> localList;
        for (const auto &filePath: localDir) {
            QDirIterator it(filePath, MUSIC_FILTERS, QDir::Files, QDirIterator::Subdirectories);
            while (it.hasNext()) {
                it.next();
                Song song(it.filePath());
                localList.append(song);
            }
        }
        setList(LOCAL_LIST_KEY, localList);
    }


    void ListCache::initUserPlaylists(const QStringList &userListKeys) {
        const auto connectName = QStringLiteral("listCache_initUserLists");
        auto dbConnection = DatabaseManager(connectName);
        for (const auto &key: userListKeys) {
            if (key == LOCAL_LIST_KEY) {
                Log.log(Log::QLogger::LogLevel::Error,
                        "Init user playlist error, invalid list key: " + key +
                        ". 'Local' cannot be used as a user list key.");
                continue;
            }
            setList(key, dbConnection.readAllSongsFromTable(key));
            Log.log(Log::QLogger::LogLevel::Info, "Init user playlist: " + key + ", completed");
        }
    }

    QList<Song> ListCache::findList(const QString &listName) const {
        if (m_listCache.contains(listName)) {
            return m_listCache.value(listName);
        }
        Log.log(Log::QLogger::LogLevel::Warning, "Cannot find such PlayList called [" + listName + "]");
        return {};
    }


    QStringList ListCache::getMusicTitleList(const QString &name) const {
        QStringList list{};
        if (m_listCache.contains(name)) {
            const auto musicList = m_listCache.value(name);
            for (const auto &music: musicList) {
                list.append(music.m_title);
            }
        } else {
            Log.log(Log::QLogger::LogLevel::Error,
                    "No such music list called : " + name + " return an empty title list.");
        }
        return list;
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
            const auto dbConnectionName = "new_list_" + key;
            if (auto dbConnection = DatabaseManager(dbConnectionName); dbConnection.createTable(key)) {
                Log.log(Log::QLogger::LogLevel::Info, "Create table successfully: " + key);
                Q_EMIT signalUserPlaylistCreated(key);
            }
        } else {
            Log.log(Log::QLogger::LogLevel::Error, "Cannot add list key: '" + key + "' already exists in the cache.");
        }
    }

    void ListCache::deleteUserPlaylist(const QString &key) {
        if (key == LOCAL_LIST_KEY) {
            Log.log(Log::QLogger::LogLevel::Error, "Delete user playlist error, invalid list key: " + key);
            return;
        }
        if (!m_listCache.contains(key)) {
            Log.log(Log::QLogger::LogLevel::Error, "No such user playlist called : " + key + ", delete failed");
        } else {
            if (m_listCache.remove(key)) {
                const auto dbConnectionName = "delete_list_" + key;
                if (auto dbConnection = DatabaseManager(dbConnectionName); dbConnection.deleteTable(key)) {
                    Log.log(Log::QLogger::LogLevel::Info, "Delete successfully: " + key);
                }
                Q_EMIT signalUserPlaylistDeleted(key);
            }
        }
    }

    bool ListCache::copyMusicFromListToList(const QString &sourceKey, const QString &destinationKey, const int index) {
        if (!m_listCache.contains(sourceKey)) {
            Log.log(Log::QLogger::LogLevel::Error, "No such source list: " + sourceKey);
            return false;
        }
        if (!m_listCache.contains(destinationKey)) {
            Log.log(Log::QLogger::LogLevel::Error, "No such destination list: " + destinationKey);
            return false;
        }
        const Song song = m_listCache.value(sourceKey).at(index);
        m_listCache[destinationKey].append(song);

        Q_EMIT signalNotifyPlayListCacheModified(destinationKey, m_listCache[destinationKey]);
        Q_EMIT signalNotifyUiCacheModified(destinationKey, getMusicTitleList(destinationKey));

        if (auto dbconnection = DatabaseManager("insert_to_" + destinationKey);
            dbconnection.insertSong(destinationKey, song)) {
            Log.log(Log::QLogger::LogLevel::Info,
                    QString("'%1' has been added to table '%2'.").arg(song.m_title, destinationKey));
        } else {
            Log.log(Log::QLogger::LogLevel::Error,
                    QString("Cannot add '%1' to table '%2'.").arg(song.m_title, destinationKey));
            return false;
        }
        return true;
    }

    void ListCache::removeSongFromListByIndex(const QString &key, const int index) {
        if (!m_listCache.contains(key)) {
            Log.log(Log::QLogger::LogLevel::Error, "Delete failed, no such user playlist called : " + key);
            return;
        }
        auto &list = m_listCache[key];
        const auto removedMusicTitle = list.at(index).m_title;
        list.removeAt(index);
        Q_EMIT signalNotifyPlayListCacheModified(key, findList(key));
        Q_EMIT signalNotifyUiCacheModified(key, getMusicTitleList(key));
        const auto dbConnectionName = "delete_" + key;
        if (auto dbConnection = DatabaseManager(dbConnectionName); dbConnection.deleteSongWithTitle(
            key, removedMusicTitle)) {
            Log.log(Log::QLogger::LogLevel::Info,
                    QString("'%1' has been removed from table '%2'.").arg(removedMusicTitle, key));
        }
    }

    void ListCache::removeSongFromListByTitle(const QString &key, const QString &songTitle) {
        if (!m_listCache.contains(key)) {
            Log.log(Log::QLogger::LogLevel::Error, "Delete failed, no such user playlist called : " + key);
            return;
        }
        const auto it = m_listCache.find(key);
        if (it == m_listCache.end()) {
            Log.log(Log::QLogger::LogLevel::Error, "Delete error, no such playlist [" + key + "]");
            return;
        }

        // delete from cache (by reference)
        auto &keyList = it.value();
        const auto iterator = std::find_if(keyList.begin(), keyList.end(), [&songTitle](const Song &song) {
            return song.m_title == songTitle;
        });
        if (iterator != keyList.end()) {
            keyList.erase(iterator);
            Q_EMIT signalNotifyPlayListCacheModified(key, findList(key));
            Q_EMIT signalNotifyUiCacheModified(key, getMusicTitleList(key));
            const auto dbConnectionName = "delete_" + key;
            if (auto dbConnection = DatabaseManager(dbConnectionName); dbConnection.deleteSongWithTitle(
                key, songTitle)) {
                Log.log(Log::QLogger::LogLevel::Info,
                        QString("'%1' has been removed from table '%2'.").arg(songTitle, key));
            }
        }
    }

    void ListCache::setList(const QString &key, const QList<Song> &list) {
        m_listCache[key] = list;
        Q_EMIT signalNotifyPlayListCacheModified(key, findList(key));
        Q_EMIT signalNotifyUiCacheModified(key, getMusicTitleList(key));
    }

    void ListCache::respondMusicTitleList(const QString &key) {
        Q_EMIT signalSendUiCurrentTitleList(key, getMusicTitleList(key));
    }

    void ListCache::handleSwitchPlaylistAndPlayIndex(const QString &key, const int index) {
        Q_EMIT signalRespondPlayListSwitchAndPlayIndex(key, findList(key), index);
    }
}
