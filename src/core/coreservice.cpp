//
// Created by 31305 on 2025/8/26.
//
#include <coreservice/coreservice.h>
#include <player/player.h>
#include <playlist/playlist.h>
#include <listcache/listcache.h>
#include <settings/settings.h>
#include <log/log.h>
#include <lyricservice/lyricservice.h>
#include "musicmetadata.h"

namespace Tray::Core
{
class CoreServicePrivate
{
public:
    Player* m_player;
    Playlist* m_playlist;
    Settings* m_settings;
    ListCache* m_listCache;
    LyricService* m_lyricService;
};

CoreService::CoreService(QObject* parent)
    : QObject(parent),
      d(std::make_unique<CoreServicePrivate>())
{
    d->m_settings = new Settings(this);
    d->m_player = new Player(this);
    d->m_playlist = new Playlist(this);
    d->m_lyricService = new LyricService(this);
    d->m_listCache = new ListCache(d->m_settings->getLocalMusicDirectories()
                                   , d->m_settings->getKeysUserPlaylist()
                                   , this);
    initConnections();
}


void CoreService::initConnections()
{
    connect(d->m_player
            , &Player::signalPlayingChanged
            , this
            , [this](const bool b)
            {
                Q_EMIT signalPlayingStatusChanged(b);
            });

    connect(d->m_playlist
            , &Playlist::signalCurrentMusicChanged
            , this
            , [this](const int index, const MusicMetaData& music)
            {
                Q_EMIT signalCurrentMusicSourceChanged(index
                    , music.m_title
                    , music.m_duration);
                d->m_lyricService->updateLRC(music.m_path);
                Q_EMIT signalLyricChanged(music.m_title
                                          , d->m_lyricService->lrcText()
                                          , d->m_lyricService->lrcTiming());
            });

    connect(d->m_player
            , &Player::signalPositionChanged
            , this
            , [this](const qint64 pos)
            {
                d->m_lyricService->handlePlayerPositionChange(pos);
                Q_EMIT signalPlayerPositionChanged(pos);
            });

    connect(
            d->m_playlist
            , &Playlist::signalPlayModeChanged
            , this
            , [this](const int mode)
            {
                Q_EMIT signalPlayModeChanged(mode);
            });

    connect(d->m_player
            , &Player::signalMusicOver
            , this
            , &CoreService::nextMusic);

    /// User playlist Add/Remove
    // listCache --> settings
    connect(d->m_listCache
            , &ListCache::signalUserPlaylistCreated
            , d->m_settings
            , &Settings::addUserPlaylist);

    connect(d->m_listCache
            , &ListCache::signalUserPlaylistDeleted
            , d->m_settings
            , &Settings::removeUserPlaylist);

    // listCache --> ui
    connect(d->m_listCache
            , &ListCache::signalUserPlaylistCreated
            , this
            , [this](const QString& key)
            {
                Q_EMIT signalUserPlaylistAdded(key);
            });

    connect(d->m_listCache
            , &ListCache::signalUserPlaylistDeleted
            , this
            , [this](const QString& key)
            {
                Q_EMIT signalUserPlaylistRemoved(key);
            });

    // settings --> ui (viewWidget)
    connect(d->m_settings
            , &Settings::signalUserKeySetsChanged
            , this
            , [this](const QStringList& keySets)
            {
                Q_EMIT signalUserKeysChanged(keySets);
            });
    /// User playlist Add/Remove

    /// List cache changed
    connect(d->m_listCache
            , &ListCache::signalNotifyPlayListCacheModified
            , d->m_playlist
            , &Playlist::handleCurrentListChanged);

    connect(d->m_listCache
            , &ListCache::signalNotifyUiCacheModified
            , this
            , [this](const QString& key, const QStringList& titleList)
            {
                Q_EMIT signalListCacheChanged(key, titleList);
            });
    /// List cache changed

    /// Local paths
    // settings --> listCache (Local)
    connect(d->m_settings
            , &Settings::signalLocalDirectoryChanged
            , d->m_listCache
            , &ListCache::initLocalPlaylist);

    // settings --> ui
    connect(d->m_settings
            , &Settings::signalLocalDirectoryChanged
            , this
            , [this](const QStringList& paths)
            {
                Q_EMIT signalLocalPathsChanged(paths);
            });
    /// Local paths

    connect(d->m_playlist
            , &Playlist::signalNotifyUiCurrentPlaylistKeyChanged
            , this
            , [this](const QString& key)
            {
                Q_EMIT signalCurrentListChanged(key);
            });

    connect(d->m_lyricService
            , &LyricService::signalTimingUpdated
            , this
            , [this](const int index)
            {
                Q_EMIT signalLyricLineIndexChanged(index);
            });
}

CoreService::~CoreService() = default;

/*!
    \internal
    Implementation of Player Controller Interface
*/

// 1) Player volume control
void CoreService::setVolume(const int volume)
{
    d->m_player->setVolume(static_cast<float>(volume) / 100);
}

// 2) Player playback control
void CoreService::playToggle()
{
    d->m_player->playTg();
}

// 3) Start playback
void CoreService::play()
{
    d->m_player->play();
}

// 4) Pause playback
void CoreService::pause()
{
    d->m_player->pause();
}

// 5) Mute controls
void CoreService::muteToggle()
{
    d->m_player->muteTg();
}

// 6) Mute on
void CoreService::muteOn()
{
    d->m_player->muteOn();
}

// 7) Mute off
void CoreService::muteOff()
{
    d->m_player->muteOff();
}

// 8) Position control
void CoreService::setPlayerPosition(const qint64 position)
{
    d->m_player->setMusicPosition(position);
}

/*!
    \internal
    Implementation of Playlist Controller Interface
*/

// 9) Playlist selection handling
void CoreService::handlePlaylistItemSelection(const QString& listKey
                                              , const int index)
{
    // check playlist ?
    if (d->m_playlist->getListKey() != listKey)
    {
        // yes, switch to new playlist
        LOG_INFO("Current list is not [" + listKey + "], switch to it");
        d->m_listCache->handleSwitchPlaylistAndPlayIndex(listKey, index);
        d->m_playlist->loadPlaylist(listKey, d->m_listCache->findList(listKey));
    }
    d->m_playlist->setCurrentMusicIndex(index);
    d->m_player->setMusicSource(d->m_playlist->getCurrentMusicPath());
    play();
}

// 10) Next track
void CoreService::nextMusic()
{
    d->m_playlist->nextMusic();
    d->m_player->setMusicSource(d->m_playlist->getCurrentMusicPath());
    play();
}

// 11) Previous track
void CoreService::preMusic()
{
    d->m_playlist->preMusic();
    d->m_player->setMusicSource(d->m_playlist->getCurrentMusicPath());
    play();
}

// 12) Playback mode cycling
void CoreService::changePlayMode()
{
    d->m_playlist->changePlayMode();
}

// 13) Playlist display
void CoreService::handleDisplayPlaylist(const QString& listKey)
{
    Q_EMIT signalCurrentTitleListChanged(listKey
                                         , d->m_listCache->
                                              getMusicTitleList(listKey));
}

// 14) User playlist management
void CoreService::addUserPlaylist(const QString& listKey)
{
    d->m_listCache->newUserPlaylist(listKey);
}

// 15) Remove user playlist
void CoreService::removeUserPlaylist(const QString& listKey)
{
    d->m_listCache->deleteUserPlaylist(listKey);
}

// 16) Music copy
void CoreService::copyMusicToList(const QString& sourceListKey
                                  , const QString& destinationListKey
                                  , const int index)
{
    d->m_listCache->copyMusicFromListToList(sourceListKey
                                            , destinationListKey
                                            , index);
}

// 17) Remove music from list
void CoreService::removeMusicFromList(const QString& key
                                      , const QString& songTitle)
{
    d->m_listCache->removeSongFromListByTitle(key, songTitle);
}

/*!
    \internal
    Implementation of Settings Interface
*/

// 18) User playlist keys
QStringList CoreService::getUserListKeys() const
{
    return d->m_settings->getKeysUserPlaylist();
}

// 19) Local music paths
QStringList CoreService::getLocalMusicPaths() const
{
    return d->m_settings->getLocalMusicDirectories();
}

QString CoreService::getPreloadKey() const
{
    return d->m_settings->getPreloadKey();
}

QStringList CoreService::getTitleList(const QString& listKey) const
{
    return d->m_listCache->getMusicTitleList(listKey);
}

int CoreService::getDefaultVolume() const
{
    return d->m_settings->getDefaultVolume();
}

// 20) Add local music path
void CoreService::appendLocalMusicPath(const QString& path)
{
    d->m_settings->addLocalMusicDirectory(path);
}

// 21) Remove local music path
void CoreService::removeLocalMusicPath(const QString& path)
{
    d->m_settings->removeLocalMusicDirectory(path);
}
}
