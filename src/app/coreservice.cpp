//
// Created by 31305 on 2025/8/26.
//
#include "coreservice.h"
#include <player/player.h>
#include <playlist/playlist.h>
#include <listcache/listcache.h>
#include <settings/settings.h>
#include <log/log.h>
#include <lyricservice/lyricservice.h>

namespace Tray::App
{
class CoreServicePrivate
{
public:
    Core::Player* m_player;
    Core::PlayList* m_playlist;
    Core::Settings* m_settings;
    Core::ListCache* m_listCache;
    Core::LyricService* m_lyricService;
};

CoreService::CoreService(QObject* parent)
    : QObject(parent),
      d(std::make_unique<CoreServicePrivate>())
{
    d->m_settings = new Core::Settings(this);
    d->m_player = new Core::Player(this);
    d->m_playlist = new Core::PlayList(this);
    d->m_lyricService = new Core::LyricService(this);
    d->m_listCache = new Core::ListCache(this);
    initConnections();
    initPreload();
}

void CoreService::initConnections()
{
}

/// init function
/// 1) settings init
/// 2) listCache loaded
/// 3) playlist loaded
/// 4) player loaded
///

void CoreService::initPreload()
{
    d->m_listCache->init(d->m_settings->getLocalMusicDirectories()
                         , d->m_settings->getKeysUserPlaylist());
    const auto preloadKey = d->m_settings->getPreloadKey();
    d->m_playlist->loadMusicList(preloadKey
                                 , d->m_listCache->
                                      getMusicTitleList(preloadKey));
}

CoreService::~CoreService() = default;


/* Interface Begin */
void CoreService::setVolume(const int volume)
{
    d->m_player->setVolume(static_cast<float>(volume) / 100);
}

// 2
void CoreService::playToggle() { d->m_player->playTg(); }

// 3
void CoreService::playToggleWithListAndIndex(const QString& listKey
                                             , const int index)
{
    // check playlist ?
    if (d->m_playlist->getListKey() != listKey)
    {
        // yes, switch to new playlist
        LOG_INFO("Current list is not [" + listKey + "], switch to it");
        Q_EMIT signalRequestSwitchPlaylistAndPlayIndex(listKey, index);
    }
    else
    {
        // no, keep the current playlist
        if (d->m_playlist->getCurrentMusicIndex() != index)
        {
            d->m_playlist->setCurrentMusicIndex(index);
            d->m_player->setMusicSource(d->m_playlist->getCurrentMusicPath());
        }
        d->m_player->playTg();
    }
}

// 4
void CoreService::nextMusic()
{
    d->m_playlist->nextMusic();
    d->m_player->setMusicSource(d->m_playlist->getCurrentMusicPath());
    d->m_player->playTg();
}

// 5
void CoreService::preMusic()
{
    d->m_playlist->preMusic();
    d->m_player->setMusicSource(d->m_playlist->getCurrentMusicPath());
    d->m_player->playTg();
}

// 6
void CoreService::setPlayerPosition(const qint64 position)
{
    d->m_player->setMusicPosition(position);
}

// 7
void CoreService::changePlayMode() { d->m_playlist->changePlayMode(); }

// 8
void CoreService::requestPlaylist(const QString& listName)
{
    Q_EMIT signalRequestTitleList(listName);
}

// 9
QStringList CoreService::getKeysOfUserPlaylist()
{
    return d->m_settings->getKeysUserPlaylist();
}

// 10
void CoreService::addUserPlaylist(const QString& listName)
{
    Q_EMIT signalAddUserPlaylistToCache(listName);
}

// 11
void CoreService::removeUserPlaylist(const QString& key)
{
    Q_EMIT signalRemoveUserPlaylistFromCache(key);
}

// 12
QStringList CoreService::getLocalMusicPaths()
{
    return d->m_settings->getLocalMusicDirectories();
}

// 13
void CoreService::appendLocalMusicPath(const QString& path)
{
    d->m_settings->addLocalMusicDirectory(path);
}

// 14
void CoreService::removeLocalMusicPath(const QString& path)
{
    d->m_settings->removeLocalMusicDirectory(path);
}

// 15
void CoreService::addMusicToList(const QString& sourceListKey
                                 , const QString& destinationListKey
                                 , const int index)
{
    Q_EMIT signalCopyMusicFromListToList(sourceListKey
                                         , destinationListKey
                                         , index);
}

// 16
void CoreService::removeMusicFromList(const QString& key
                                      , const QString& songTitle)
{
    Q_EMIT signalRemoveMusicFromList(key, songTitle);
}

// 17
void CoreService::setMute()
{
    d->m_player->setMute();
}
/* Interface End  */
}
