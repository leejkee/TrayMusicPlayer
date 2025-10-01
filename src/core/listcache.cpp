//
// Created by cww on 25-4-7.
//
#include <listcache/listcache.h>
#include <databasemanager/databasemanager.h>
#include <log/log.h>
#include <QDirIterator>
#include <algorithm>

namespace Tray::Core
{
ListCache::ListCache(QObject* parent)
    : QObject(parent)
{
    LOG_INFO("Constructor: no list has been initialized.");
}

ListCache::ListCache(const QStringList& localPaths
                     , const QStringList& userListKeys
                     , QObject* parent)
    : QObject(parent)
{
    initLocalPlaylist(localPaths);
    initUserPlaylists(userListKeys);
    LOG_INFO("Constructor: local list and user lists have been initialized.");
}

void ListCache::initLocalPlaylist(const QStringList& localDir)
{
    m_listCache[LOCAL_LIST_KEY] = std::make_shared<QList<MusicMetaData>>();
    QList<MusicMetaData> localList;
    for (const auto& filePath : localDir)
    {
        QDirIterator it(filePath
                        , MUSIC_FILTERS
                        , QDir::Files
                        , QDirIterator::Subdirectories);
        while (it.hasNext())
        {
            it.next();
            MusicMetaData song(it.filePath());
            localList.append(song);
        }
    }
    *m_listCache[LOCAL_LIST_KEY] = localList;
}


void ListCache::initUserPlaylists(const QStringList& userListKeys)
{
    if (userListKeys.isEmpty())
    {
        LOG_WARNING("No user list keys.");
        return;
    }
    const auto connectName = QStringLiteral("listCache_initUserLists");
    auto dbConnection = DatabaseManager(connectName);
    for (const auto& key : userListKeys)
    {
        if (key == LOCAL_LIST_KEY)
        {
            LOG_ERROR(QString(
                          "Init user playlist error, invalid list key: %1. 'Local' cannot be used as a user list key."
                      ).arg(key));
            continue;
        }
        m_listCache[key] = std::make_shared<QList<MusicMetaData>>();
        *m_listCache[key] = dbConnection.readAllSongsFromTable(key);
        LOG_INFO(QString("Init user playlist: %1, completed").arg(key));
    }
}

std::shared_ptr<QList<MusicMetaData>> ListCache::getPlaylist(
    const QString& listName) const
{
    if (m_listCache.contains(listName))
    {
        return m_listCache.value(listName);
    }
    LOG_ERROR(QString("Cannot find such PlayList called [%1]").arg(listName));
    return {};
}

QStringList ListCache::getMusicTitleList(const QString& key) const
{
    if (!m_listCache.contains(key))
    {
        LOG_ERROR(QString("No such list called [%1]").arg(key));
        return {};
    }
    QStringList list;
    const auto musicList = *m_listCache.value(key);
    for (const auto& music : musicList)
    {
        list.append(music.m_title);
    }
    return list;
}

void ListCache::newUserPlaylist(const QString& key)
{
    if (key == LOCAL_LIST_KEY)
    {
        LOG_ERROR(QString("Create user list failed, invalid list key [%1]").arg(
                      key));
        return;
    }
    if (!m_listCache.contains(key))
    {
        m_listCache[key] = std::make_shared<QList<MusicMetaData>>();
        const auto dbConnectionName = "new_list_" + key;
        if (auto dbConnection = DatabaseManager(dbConnectionName); dbConnection.
            createTable(key))
        {
            LOG_INFO(QString("Create table successfully: %1").arg(key));
            Q_EMIT signalUserPlaylistCreated(key);
        }
    }
    else
    {
        LOG_ERROR(QString(
                      "Create user list failed, list [%1] already exists in the cache"
                  ).arg(key));
    }
}

void ListCache::deleteUserPlaylist(const QString& key)
{
    if (key == LOCAL_LIST_KEY)
    {
        LOG_ERROR(QString("Delete user list failed, invalid list key: %1").
                  arg(key));
        return;
    }
    if (!m_listCache.contains(key))
    {
        LOG_ERROR(QString(
                      "Delete user list failed, no such user list called [%1]").
                  arg(key));
    }
    else
    {
        if (m_listCache.remove(key))
        {
            const auto dbConnectionName = "delete_list_" + key;
            if (auto dbConnection = DatabaseManager(dbConnectionName);
                dbConnection.deleteTable(key))
            {
                LOG_INFO(QString("Delete successfully: %1").arg(key));
            }
            Q_EMIT signalUserPlaylistDeleted(key);
        }
    }
}

bool ListCache::copyMusicFromListToList(const QString& sourceKey
                                        , const QString& destinationKey
                                        , const int index)
{
    if (!m_listCache.contains(sourceKey))
    {
        LOG_ERROR(QString("No such source list: %1").arg(sourceKey));
        return false;
    }
    if (!m_listCache.contains(destinationKey))
    {
        LOG_ERROR(QString("No such destination list: %1").arg(destinationKey));
        return false;
    }

    const MusicMetaData sourceSong = m_listCache.value(sourceKey)->at(index);
    auto f = [&sourceSong](QList<MusicMetaData>& list)
    {
        list.append(sourceSong);
    };
    modifyCache(destinationKey, f);

    if (auto dbconnection = DatabaseManager("insert_to_" + destinationKey);
        dbconnection.insertSong(destinationKey, sourceSong))
    {
        LOG_INFO(QString("'%1' has been added to table '%2'.").arg(sourceSong.
                     m_title,
                     destinationKey));
    }
    else
    {
        LOG_ERROR(QString("Cannot add '%1' to table '%2'.").arg(sourceSong.
                      m_title,
                      destinationKey));
        return false;
    }
    return true;
}

void ListCache::removeSongFromListByIndex(const QString& key, const int index)
{
    if (!m_listCache.contains(key))
    {
        LOG_ERROR(QString("Delete failed, no such user playlist called : %1").
                  arg(key));
        return;
    }

    const auto removedMusicTitle = m_listCache.value(key)->at(index).m_title;
    auto f = [index](QList<MusicMetaData>& list)
    {
        list.removeAt(index);
    };
    modifyCache(key, f);

    const auto dbConnectionName = "delete_" + key;
    if (auto dbConnection = DatabaseManager(dbConnectionName); dbConnection.
        deleteSongWithTitle(key, removedMusicTitle))
    {
        LOG_INFO(QString("'%1' has been removed from table '%2'.").arg(
                     removedMusicTitle, key));
    }
}

void ListCache::removeSongFromListByTitle(const QString& key
                                          , const QString& songTitle)
{
    const auto listIt = m_listCache.find(key);
    if (listIt == m_listCache.end())
    {
        LOG_ERROR(QString("List [%1] not found in cache").arg(key));
        return;
    }

    const auto ptr = listIt.value();
    const auto it = std::find_if(ptr->begin()
                                 , ptr->end()
                                 , [&songTitle](const MusicMetaData& music)
                                 {
                                     return music.m_title == songTitle;
                                 });

    if (it == ptr->end())
    {
        LOG_ERROR(QString("Remove song [%1] failed, not exists").arg(songTitle))
        ;
        return;
    }
    auto index = std::distance(ptr->begin(), it);

    auto f = [index](QList<MusicMetaData>& list)
    {
        list.removeAt(index);
    };
    modifyCache(key, f);
    const auto dbConnectionName = "delete_" + key;
    if (auto dbConnection = DatabaseManager(dbConnectionName); dbConnection.
        deleteSongWithTitle(key, songTitle))
    {
        LOG_INFO(QString("'%1' has been removed from table '%2'.").arg(
                     songTitle, key));
    }
}


void ListCache::modifyCache(const QString& key
                            , std::function<void(QList<MusicMetaData>&)> f)
{
    const auto it = m_listCache.find(key);
    if (it == m_listCache.end())
    {
        LOG_ERROR(QString("List [%1] not exists").arg(key));
        return;
    }

    auto oldPtr = it.value();
    const auto newPtr = std::make_shared<QList<MusicMetaData>>();
    if (oldPtr != nullptr)
    {
        *newPtr = *oldPtr;
    }
    f(*newPtr);
    it.value() = newPtr;

    Q_EMIT signalPlaylistCacheModified(key, newPtr);
    Q_EMIT signalNotifyUiCacheModified(key, getMusicTitleList(key));
}

void ListCache::respondMusicTitleList(const QString& key)
{
    Q_EMIT signalSendUiCurrentTitleList(key, getMusicTitleList(key));
}
}
