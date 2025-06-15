//
// Created by cww on 25-4-4.
//
#include "QLogger.h"
#include <core.h>
#include <databasemanager.h>
#include <listcache.h>
#include <player.h>
#include <playlist.h>
#include <qtmetamacros.h>
#include <settings.h>
#include <trayconfig.h>
#include <QThread>

namespace Tray::Core {
class CorePrivate {
public:
    explicit CorePrivate(QObject* p_core);

    void initListCache();

    void updatePlaylistIfPlayingThis(const QString& key,
                                     const QList<Song>& list);

    void handleSwitchPlaylistAndPlayIndex(const QString& key,
                                          const QList<Song>& list, int index);

    Player* m_player;
    Log::QLogger Log;
    PlayList* m_playList;
    ListCache* m_listCache;
    Settings* m_settings;
    QThread* m_listCacheThread;
    QObject* p_ptr;
};

CorePrivate::CorePrivate(QObject* p_core) : p_ptr(p_core) {
    Log = Log::QLogger(p_ptr->objectName());
    m_player = new Player(p_ptr);
    m_playList = new PlayList(p_ptr);
    m_settings = new Settings(p_ptr);
    m_listCache = new ListCache(Q_NULLPTR);
    m_listCacheThread = new QThread(p_ptr);
    m_listCache->moveToThread(m_listCacheThread);
    Log.log(Log::QLogger::LogLevel::Info, "Initializing Core successfully");
}

void CorePrivate::initListCache() {
    QMetaObject::invokeMethod(
        m_listCache, "init", Qt::QueuedConnection,
        Q_ARG(QStringList, m_settings->getLocalMusicDirectories()),
        Q_ARG(QStringList, m_settings->getKeysUserPlaylist()));
}

void CorePrivate::updatePlaylistIfPlayingThis(const QString& key,
                                              const QList<Song>& list) {
    if (m_playList->getListKey() == key) {
        m_playList->loadMusicList(key, list);
    }
}

void CorePrivate::handleSwitchPlaylistAndPlayIndex(const QString& key,
                                                   const QList<Song>& list,
                                                   const int index) {
    m_playList->loadMusicList(key, list);
    m_playList->setCurrentMusicIndex(index);
    m_player->setMusicSource(m_playList->getCurrentMusicPath());
    m_player->playTg();
}

Core::Core(QObject* parent) : QObject(parent) {
    this->setObjectName(QStringLiteral("Core"));
    d = std::make_unique<CorePrivate>(this);
    createConnections();
}

Core::~Core() {
    delete d->m_listCache;
    if (d->m_listCacheThread && d->m_listCacheThread->isRunning()) {
        d->m_listCacheThread->quit();
        d->m_listCacheThread->wait();
    }
}

void Core::createConnections() {
    connect(d->m_listCacheThread, &QThread::started, this,
            [this]() { d->initListCache(); });

    connect(d->m_listCache, &ListCache::signalInitCompleted, this, [this]() {
        Q_EMIT signalRequestTitleList(LOCAL_LIST_KEY);
        d->Log.log(Log::QLogger::LogLevel::Info,
                   "List cache initialization completed");
    });

    connect(
        d->m_player, &Player::signalPlayingChanged, this,
        [this](const bool b) { Q_EMIT signalNotifyUiPlayingStatusChanged(b); });

    connect(
        d->m_playList, &PlayList::signalCurrentMusicChanged, this,
        [this](const qsizetype index, const QString& name, const int duration) {
            Q_EMIT signalNotifyUiCurrentMusicChanged(static_cast<int>(index),
                                                     name, duration);
        });

    connect(d->m_player, &Player::signalMuteChanged, this,
            [this](const bool b) { Q_EMIT signalNotifyUiUpdateMuteIcon(b); });

    connect(d->m_player, &Player::signalPositionChanged, this,
            [this](const qint64 pos) { Q_EMIT signalPositionChanged(pos); });

    connect(
        d->m_playList, &PlayList::signalPlayModeChanged, this,
        [this](const int mode) { Q_EMIT signalNotifyUiPlayModeChanged(mode); });

    connect(d->m_player, &Player::signalMusicOver, this, &Core::nextMusic);

    /// User playlist Add/Remove
    // listCache --> settings
    connect(d->m_listCache, &ListCache::signalUserPlaylistCreated,
            d->m_settings, &Settings::addUserPlaylist);
    connect(d->m_listCache, &ListCache::signalUserPlaylistDeleted,
            d->m_settings, &Settings::removeUserPlaylist);
    // listCache --> ui
    connect(d->m_listCache, &ListCache::signalUserPlaylistCreated, this,
            [this](const QString& key) {
                Q_EMIT signalNotifyUiToAddUserPlaylist(key);
            });
    connect(d->m_listCache, &ListCache::signalUserPlaylistDeleted, this,
            [this](const QString& key) {
                Q_EMIT signalNotifyUiToRemoveUserPlaylist(key);
            });
    // settings --> ui (viewWidget)
    connect(d->m_settings, &Settings::signalUserKeySetsChanged, this,
            [this](const QStringList& list) {
                Q_EMIT signalNotifyUiUserKeySetsChanged(list);
            });
    /// User playlist Add/Remove

    /// List cache changed
    connect(d->m_listCache, &ListCache::signalNotifyPlayListCacheModified, this,
            [this](const QString& key, const QList<Song>& list) {
                d->updatePlaylistIfPlayingThis(key, list);
            });
    connect(d->m_listCache, &ListCache::signalNotifyUiCacheModified, this,
            [this](const QString& key, const QStringList& titleList) {
                Q_EMIT signalNotifyUiCacheModified(key, titleList);
            });
    /// List cache changed

    /// Local paths
    // settings --> listCache (Local)
    connect(d->m_settings, &Settings::signalLocalDirectoryChanged,
            d->m_listCache, &ListCache::initLocalPlaylist);
    // settings --> ui
    connect(d->m_settings, &Settings::signalLocalDirectoryChanged, this,
            [this](const QStringList& paths) {
                Q_EMIT signalNotifyUiToUpdateLocalPaths(paths);
            });
    /// Local paths

    connect(d->m_playList, &PlayList::signalNotifyUiCurrentPlaylistKeyChanged,
            this, [this](const QString& key) {
                Q_EMIT signalNotifyUiCurrentListKeyChanged(key);
            });

    /// User list Add/Remove
    connect(this, &Core::signalRemoveUserPlaylistFromCache, d->m_listCache,
            &ListCache::deleteUserPlaylist);
    connect(this, &Core::signalAddUserPlaylistToCache, d->m_listCache,
            &ListCache::newUserPlaylist);
    /// User list Add/Remove

    /// Switch view list
    connect(this, &Core::signalRequestTitleList, d->m_listCache,
            &ListCache::respondMusicTitleList);
    connect(d->m_listCache, &ListCache::signalSendUiCurrentTitleList, this,
            [this](const QString& key, const QStringList& list) {
                Q_EMIT signalSendUiCurrentTitleList(key, list);
            });
    /// Switch view list

    connect(this, &Core::signalCopyMusicFromListToList, d->m_listCache,
            &ListCache::copyMusicFromListToList);

    connect(this, &Core::signalRemoveMusicFromList, d->m_listCache,
            &ListCache::removeSongFromListByTitle);

    connect(this, &Core::signalRequestSwitchPlaylistAndPlayIndex,
            d->m_listCache, &ListCache::handleSwitchPlaylistAndPlayIndex);

    connect(
        d->m_listCache, &ListCache::signalRespondPlayListSwitchAndPlayIndex,
        this,
        [this](const QString& key, const QList<Song>& list, const int index) {
            d->handleSwitchPlaylistAndPlayIndex(key, list, index);
        });
}

void Core::initWork() {
    setVolume(d->m_settings->getDefaultVolume());

    Q_EMIT signalInitUiDefaultSettings(
        d->m_settings->getLocalMusicDirectories(),
        d->m_settings->getKeysUserPlaylist(),
        d->m_settings->getDefaultVolume());

    d->m_listCacheThread->start();
}

/* Interface Begin */
// 1
void Core::setVolume(const unsigned volume) {
    d->m_player->setVolume(static_cast<float>(volume) / 100);
}

// 2
void Core::playToggle() { d->m_player->playTg(); }

// 3
void Core::playToggleWithListAndIndex(const QString& listKey, const int index) {
    // check playlist ?
    if (d->m_playList->getListKey() != listKey) {
        // yes, switch to new playlist
        d->Log.log(Log::QLogger::LogLevel::Info,
                   "Current list is not [" + listKey + "], switch to it");
        Q_EMIT signalRequestSwitchPlaylistAndPlayIndex(listKey, index);
    } else {
        // no, keep the current playlist
        if (d->m_playList->getCurrentMusicIndex() != index) {
            d->m_playList->setCurrentMusicIndex(index);
            d->m_player->setMusicSource(d->m_playList->getCurrentMusicPath());
        }
        d->m_player->playTg();
    }
}

// 4
void Core::nextMusic() {
    d->m_playList->nextMusic();
    d->m_player->setMusicSource(d->m_playList->getCurrentMusicPath());
    d->m_player->playTg();
}

// 5
void Core::preMusic() {
    d->m_playList->preMusic();
    d->m_player->setMusicSource(d->m_playList->getCurrentMusicPath());
    d->m_player->playTg();
}

// 6
void Core::setPlayerPosition(const qint64 position) {
    d->m_player->setMusicPosition(position);
}

// 7
void Core::changePlayMode() { d->m_playList->changePlayMode(); }

// 8
void Core::requestPlaylist(const QString& listName) {
    Q_EMIT signalRequestTitleList(listName);
}

// 9
QStringList Core::getKeysOfUserPlaylist() {
    return d->m_settings->getKeysUserPlaylist();
}

// 10
void Core::addUserPlaylist(const QString& listName) {
    Q_EMIT signalAddUserPlaylistToCache(listName);
}

// 11
void Core::removeUserPlaylist(const QString& key) {
    Q_EMIT signalRemoveUserPlaylistFromCache(key);
}

// 12
QStringList Core::getLocalMusicPaths() {
    return d->m_settings->getLocalMusicDirectories();
}

// 13
void Core::appendLocalMusicPath(const QString& path) {
    d->m_settings->addLocalMusicDirectory(path);
}

// 14
void Core::removeLocalMusicPath(const QString& path) {
    d->m_settings->removeLocalMusicDirectory(path);
}

// 15
void Core::addMusicToList(const QString& sourceListKey,
                          const QString& destinationListKey, const int index) {
    Q_EMIT signalCopyMusicFromListToList(sourceListKey, destinationListKey,
                                         index);
}

// 16
void Core::removeMusicFromList(const QString& key, const QString& songTitle) {
    Q_EMIT signalRemoveMusicFromList(key, songTitle);
}

// 17
void Core::setMute() { d->m_player->setMute(); }
/* Interface End */
} // namespace Tray::Core
