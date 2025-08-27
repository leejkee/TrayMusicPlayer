//
// Created by cww on 25-4-7.
//
#include <listcache/listcache.h>
#include <databasemanager/databasemanager.h>
#include <trayconfig.h>
#include <log/log.h>
#include <QDirIterator>
#include <algorithm>


namespace Tray::Core
{
ListCache::ListCache(QObject* parent)
    : QObject(parent)
{
    setObjectName(QStringLiteral("ListCache"));
    LOG_INFO("Constructor: no list has been initialized.");
}

ListCache::ListCache(const QStringList& localDir
                     , const QStringList& userListKeys
                     , QObject* parent)
    : QObject(parent)
{
    setObjectName(QStringLiteral("ListCache"));
    init(localDir, userListKeys);
    LOG_INFO("Constructor: local list and user lists have been initialized.");
}


void ListCache::init(const QStringList& localDir
                     , const QStringList& userListKeys)
{
    initLocalPlaylist(localDir);
    initUserPlaylists(userListKeys);
    Q_EMIT signalInitCompleted();
}

void ListCache::initLocalPlaylist(const QStringList& localDir)
{
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
    setList(LOCAL_LIST_KEY, localList);
}


void ListCache::initUserPlaylists(const QStringList& userListKeys)
{
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
        setList(key, dbConnection.readAllSongsFromTable(key));
        LOG_INFO(QString("Init user playlist: %1, completed").arg(key));
    }
}

QList<MusicMetaData> ListCache::findList(const QString& listName) const
{
    if (m_listCache.contains(listName))
    {
        return m_listCache.value(listName);
    }
    LOG_WARNING(QString("Cannot find such PlayList called [%1]").arg(listName));
    return {};
}


QStringList ListCache::getMusicTitleList(const QString& name) const
{
    QStringList list{};
    if (m_listCache.contains(name))
    {
        const auto musicList = m_listCache.value(name);
        for (const auto& music : musicList)
        {
            list.append(music.m_title);
        }
    }
    else
    {
        LOG_ERROR(QString(
                      "No such music list called : %1 return an empty title list."
                  ).arg(name));
    }
    return list;
}

void ListCache::newUserPlaylist(const QString& key)
{
    if (key == LOCAL_LIST_KEY)
    {
        LOG_ERROR(QString(
                      "New user playlist error, invalid list key: %1. 'Local' cannot be used as a user list key."
                  ).arg(key));
        return;
    }
    if (!m_listCache.contains(key))
    {
        m_listCache[key] = QList<MusicMetaData>();
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
                      "Cannot add list key: '%1' already exists in the cache.").
                  arg(key));
    }
}

void ListCache::deleteUserPlaylist(const QString& key)
{
    if (key == LOCAL_LIST_KEY)
    {
        LOG_ERROR(QString("Delete user playlist error, invalid list key: %1").
                  arg(key));
        return;
    }
    if (!m_listCache.contains(key))
    {
        LOG_ERROR(QString("No such user playlist called : %1, delete failed").
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
    const MusicMetaData song = m_listCache.value(sourceKey).at(index);
    m_listCache[destinationKey].append(song);

    Q_EMIT signalNotifyPlayListCacheModified(destinationKey
                                             , m_listCache[destinationKey]);
    Q_EMIT signalNotifyUiCacheModified(destinationKey
                                       , getMusicTitleList(destinationKey));

    if (auto dbconnection = DatabaseManager("insert_to_" + destinationKey);
        dbconnection.insertSong(destinationKey, song))
    {
        LOG_INFO(QString("'%1' has been added to table '%2'.").arg(song.m_title,
                     destinationKey));
    }
    else
    {
        LOG_ERROR(QString("Cannot add '%1' to table '%2'.").arg(song.m_title,
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
    auto& list = m_listCache[key];
    const auto removedMusicTitle = list.at(index).m_title;
    list.removeAt(index);
    Q_EMIT signalNotifyPlayListCacheModified(key, findList(key));
    Q_EMIT signalNotifyUiCacheModified(key, getMusicTitleList(key));
    const auto dbConnectionName = "delete_" + key;
    if (auto dbConnection = DatabaseManager(dbConnectionName); dbConnection.
        deleteSongWithTitle(
                            key
                            , removedMusicTitle))
    {
        LOG_INFO(QString("'%1' has been removed from table '%2'.").arg(
                     removedMusicTitle, key));
    }
}

void ListCache::removeSongFromListByTitle(const QString& key
                                          , const QString& songTitle)
{
    if (!m_listCache.contains(key))
    {
        LOG_ERROR(QString("Delete failed, no such user playlist called : %1").
                  arg(key));
        return;
    }
    const auto it = m_listCache.find(key);
    if (it == m_listCache.end())
    {
        LOG_ERROR(QString("Delete error, no such playlist [%1]").arg(key));
        return;
    }

    // delete from cache (by reference)
    auto& keyList = it.value();
    const auto iterator = std::find_if(keyList.begin()
                                       , keyList.end()
                                       , [&songTitle](const MusicMetaData& song)
                                       {
                                           return song.m_title == songTitle;
                                       });
    if (iterator != keyList.end())
    {
        keyList.erase(iterator);
        Q_EMIT signalNotifyPlayListCacheModified(key, findList(key));
        Q_EMIT signalNotifyUiCacheModified(key, getMusicTitleList(key));
        const auto dbConnectionName = "delete_" + key;
        if (auto dbConnection = DatabaseManager(dbConnectionName); dbConnection.
            deleteSongWithTitle(
                                key
                                , songTitle))
        {
            LOG_INFO(QString("'%1' has been removed from table '%2'.").arg(
                         songTitle, key));
        }
    }
}

void ListCache::setList(const QString& key, const QList<MusicMetaData>& list)
{
    m_listCache[key] = list;
    Q_EMIT signalNotifyPlayListCacheModified(key, findList(key));
    Q_EMIT signalNotifyUiCacheModified(key, getMusicTitleList(key));
}

void ListCache::respondMusicTitleList(const QString& key)
{
    Q_EMIT signalSendUiCurrentTitleList(key, getMusicTitleList(key));
}

void ListCache::handleSwitchPlaylistAndPlayIndex(
    const QString& key
    , const int index)
{
    Q_EMIT signalRespondPlayListSwitchAndPlayIndex(key, findList(key), index);
}
}
