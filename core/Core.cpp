//
// Created by cww on 25-4-4.
//
#include "TrayConfig.h"
#include <Core.h>
#include <Player.h>
#include <PlayList.h>
#include <ListCache.h>
#include <Settings.h>
#include <DatabaseManager.h>
#include <QThread>


namespace Tray::Core {
    class CorePrivate {
    public:
        explicit CorePrivate(QObject *p_core);

        Player *m_player;
        Log::QLogger Log;
        PlayList *m_playList;
        ListCache *m_listCache;
        Settings *m_settings;
        QThread *m_listCacheThread;
        QObject *p_ptr;
    };

    CorePrivate::CorePrivate(QObject *p_core): p_ptr(p_core) {
        Log = Log::QLogger(p_ptr->objectName());
        m_player = new Player(p_ptr);
        m_playList = new PlayList(p_ptr);
        m_settings = new Settings(p_ptr);
        m_listCache = new ListCache(Q_NULLPTR);
        m_listCacheThread = new QThread(p_ptr);
        m_listCache->moveToThread(m_listCacheThread);
        Log.log(Log::QLogger::LogLevel::Info, "Initializing Core successfully");
    }

    Core::Core(QObject *parent) : QObject(parent) {
        this->setObjectName(QStringLiteral("Core"));
        d = std::make_unique<CorePrivate>(this);

        connect(d->m_listCacheThread, &QThread::started, this, &Core::initListCache);
        connect(d->m_listCache, &ListCache::signalInitCompleted, this, &Core::handleListCacheInitializationComplete);
        d->m_listCacheThread->start();

        connect(d->m_player, &Player::signalPlayingChanged, this, [this](const bool b) {
            Q_EMIT signalPlayingStatusChanged(b);
        });

        connect(d->m_playList, &PlayList::signalMusicChanged, this, [this]
        (const qsizetype index, const QString &name, const int duration) {
                    Q_EMIT signalCurrentMusicChanged(static_cast<int>(index), name, duration);
                }
        );

        connect(d->m_player, &Player::signalIsMuted, this, [this](const bool b) {
            Q_EMIT signalIsMuted(b);
        });

        connect(d->m_player, &Player::signalPositionChanged, this, [this](const qint64 pos) {
            Q_EMIT signalPositionChanged(pos);
        });

        connect(d->m_playList, &PlayList::signalPlayModeChanged, this, [this](const int mode) {
            Q_EMIT signalPlayModeChanged(mode);
        });

        connect(d->m_player, &Player::signalMusicOver, this, &Core::nextMusic);

        // OK
        connect(d->m_listCache, &ListCache::signalUserPlaylistCreated, d->m_settings, &Settings::addUserPlaylist);

        // update playlist
        connect(d->m_listCache, &ListCache::signalPlaylistModified, this, &Core::updateCurrentPlaylist);

        connect(d->m_listCache, &ListCache::signalPlaylistModified, this, [this](const QString &key) {
            Q_EMIT signalPlaylistModified(key, d->m_listCache->getMusicTitleList(key));
        });

        connect(d->m_settings, &Settings::signalUserPlaylistsChanged, this, [this](const QStringList &list) {
            Q_EMIT signalUserPlaylistSetsChanged(list);
        });

        connect(d->m_settings, &Settings::signalLocalDirectoryChanged, d->m_listCache, &ListCache::initLocalPlaylist);

        connect(d->m_listCache, &ListCache::signalUserPlaylistDeleted, d->m_settings, &Settings::removeUserPlaylist);

        connect(d->m_playList, &PlayList::signalCurrentPlaylistKeyChanged, this, [ this](const QString &key) {
            Q_EMIT signalCurrentPlaylistKeyChanged(key);
        });
        initDefaultSettings();
    }

    Core::~Core() {
        if (d->m_listCache) {
            delete d->m_listCache;
        }

        if (d->m_listCacheThread && d->m_listCacheThread->isRunning()) {
            d->m_listCacheThread->quit();
            d->m_listCacheThread->wait();
        }
    }


    void Core::initListCache() {
        QMetaObject::invokeMethod(d->m_listCache, "init", Qt::QueuedConnection,
                                  Q_ARG(QStringList, d->m_settings->getLocalMusicDirectories()),
                                  Q_ARG(QStringList, d->m_settings->getKeysUserPlaylist()));
    }

    void Core::handleListCacheInitializationComplete() {
        d->Log.log(Log::QLogger::LogLevel::Info, "List cache initialization completed");
    }

    void Core::initDefaultSettings() {
        setVolume(d->m_settings->getDefaultVolume());
        playLocalMusicFromFirst();
    }

    /* Interface Begin */
    // 1
    void Core::playToggle() {
        d->m_player->playTg();
    }


    // 2
    void Core::nextMusic() {
        d->m_playList->nextMusic();
        d->m_player->setMusicSource(d->m_playList->getCurrentMusicPath());
        d->m_player->playTg();
    }


    // 3
    void Core::preMusic() {
        d->m_playList->preMusic();
        d->m_player->setMusicSource(d->m_playList->getCurrentMusicPath());
        d->m_player->playTg();
    }


    // 4
    void Core::playToggleWithListAndIndex(const QString &listKey, const int index) {
        // check playlist ?
        if (d->m_playList->getListKey() != listKey) {
            // yes, switch to new playlist
            d->Log.log(Log::QLogger::LogLevel::Info, "Current list is not [" + listKey + "], switch to it");
            d->m_playList->loadMusicList(listKey, d->m_listCache->findList(listKey));
            d->m_playList->setCurrentMusicIndex(index);
            d->m_player->setMusicSource(d->m_playList->getCurrentMusicPath());
        } else {
            // no, keep the current playlist
            if (d->m_playList->getCurrentMusicIndex() != index) {
                d->m_playList->setCurrentMusicIndex(index);
                d->m_player->setMusicSource(d->m_playList->getCurrentMusicPath());
            }
        }
        d->m_player->playTg();
    }


    // 5
    void Core::setVolume(const unsigned volume) {
        d->m_player->setVolume(static_cast<float>(volume) / 100);
    }


    // 6
    void Core::switchPlaylist(const QString &listName) {
        d->m_playList->loadMusicList(listName, d->m_listCache->findList(listName));
    }


    // 7
    void Core::requestPlaylist(const QString &listName) {
        Q_EMIT signalPlaylistSwitched(listName, d->m_listCache->getMusicTitleList(listName));
    }


    // 8
    void Core::setPlayerPosition(const qint64 position) {
        d->m_player->setMusicPosition(position);
    }


    // 9
    void Core::changePlayMode() {
        d->m_playList->changePlayMode();
    }


    // 10
    QStringList Core::getKeysOfUserPlaylist() {
        return d->m_settings->getKeysUserPlaylist();
    }


    // 11
    void Core::newUserPlaylist(const QString &listName) {
        d->m_listCache->newUserPlaylist(listName);
    }


    // 12
    QStringList Core::getLocalMusicPaths() {
        return d->m_settings->getLocalMusicDirectories();
    }


    // 13
    void Core::appendLocalMusicPath(const QString &path) {
        d->m_settings->addLocalMusicDirectory(path);
        // d->m_listCache->initLocalPlaylist(d->m_settings->getLocalMusicDirectories());
    }


    // 14
    void Core::removeLocalMusicPath(const QString &path) {
        d->m_settings->removeLocalMusicDirectory(path);
        d->m_listCache->initLocalPlaylist(d->m_settings->getLocalMusicDirectories());
    }


    // 15
    void Core::addMusicToList(const QString &sourceListKey, const QString &destinationListKey, const int index) {
        const auto sourceList = d->m_listCache->findList(sourceListKey);
        if (sourceList.isEmpty()) {
            d->Log.log(Log::QLogger::LogLevel::Error, "No user list called: " + sourceListKey);
            return;
        }
        d->m_listCache->insertMusicToList(destinationListKey, sourceList.at(index));
    }


    // 16
    void Core::removeMusicFromList(const QString &key, const QString &songTitle) {
        d->m_listCache->deleteSong(key, songTitle);
    }


    // 17
    void Core::deleteUserPlaylist(const QString &key) {
        d->m_listCache->deleteUserPlaylist(key);
    }


    /* Interface End */

    void Core::playLocalMusicFromFirst() {
        Q_EMIT signalPlaylistSwitched(LOCAL_LIST_KEY, d->m_listCache->getMusicTitleList(LOCAL_LIST_KEY));
    }


    QStringList Core::getLocalMusicTitleList() {
        return d->m_listCache->getMusicTitleList(LOCAL_LIST_KEY);
    }


    void Core::updateCurrentPlaylist(const QString &key) {
        if (d->m_playList->getListKey() == key) {
            d->m_playList->loadMusicList(key, d->m_listCache->findList(key));
        }
    }
}
