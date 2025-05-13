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


namespace Tray::Core {
    class CorePrivate {
    public:
        Player *m_player;
        Log::QLogger Log;
        PlayList *m_playList;
        ListCache *m_listCache;
        Settings *m_settings;
    };

    Core::Core(const QString &iniPath, QObject *parent) : QObject(parent), d(std::make_unique<CorePrivate>()) {
        this->setObjectName(QStringLiteral("Core"));
        d->Log = Log::QLogger(this->objectName());
        d->m_player = new Player(this);
        d->m_playList = new PlayList(this);
        d->m_listCache = new ListCache(this);
        d->m_settings = new Settings(iniPath, this);
        d->Log.log(Log::QLogger::LogLevel::Info, "Initializing Core successfully");
        createConnections();
        initDefaultSettings();
    }

    Core::~Core() = default;

    void Core::createConnections() {
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

        connect(d->m_playList, &PlayList::signalPlayModeChanged, this, [this](const PlayMode mode) {
            Q_EMIT signalPlayModeChanged(static_cast<int>(mode));
        });

        connect(d->m_player, &Player::signalMusicOver, this, &Core::nextMusic);

        connect(d->m_listCache, &ListCache::signalUserPlaylistCreated, d->m_settings, &Settings::addUserMusicList);

        connect(d->m_listCache, &ListCache::signalPlayListChanged, this, &Core::updatePlaylist);
    }

    void Core::initDefaultSettings() {
        setVolume(d->m_settings->getDefaultVolume());
        d->m_listCache->initLocalPlaylist(d->m_settings->getLocalMusicDirectories());
        d->m_listCache->initUserPlaylists(d->m_settings->getKeysUserPlaylist());
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
        /// check playlist ?
        if (d->m_playList->getListKey() != listKey) {
            // yes
            d->Log.log(Log::QLogger::LogLevel::Info, "Current list is not [" + listKey + "], switch to it");
            d->m_playList->loadMusicList(listKey, d->m_listCache->findList(listKey));
            d->m_playList->setCurrentMusicIndex(index);
            d->m_player->setMusicSource(d->m_playList->getCurrentMusicPath());
        } else {
            // no
            /// check music ?
            if (index != d->m_playList->getCurrentMusicIndex()) {
                // yes
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
        Q_EMIT signalMusicListChanged(listName, d->m_listCache->getMusicTitleList(listName));
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
    void Core::newUserList(const QString &listName) {
        d->m_listCache->newUserPlaylist(listName);
    }


    // 12
    QStringList Core::getLocalMusicPaths() {
        return d->m_settings->getLocalMusicDirectories();
    }


    // 13
    void Core::appendLocalMusicPath(const QString &path) {
        d->m_settings->addLocalMusicDirectory(path);
        d->m_listCache->initLocalPlaylist(d->m_settings->getLocalMusicDirectories());
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
    /* Interface End */

    void Core::playLocalMusicFromFirst() {
        Q_EMIT signalMusicListChanged(LOCAL_LIST_KEY, d->m_listCache->getMusicTitleList(LOCAL_LIST_KEY));
    }


    QStringList Core::getLocalMusicTitleList() {
        return d->m_listCache->getMusicTitleList(LOCAL_LIST_KEY);
    }


    void Core::updatePlaylist(const QString &key) {
        if (d->m_playList->getListKey() == key) {
            d->m_playList->loadMusicList(key, d->m_listCache->findList(key));
            Q_EMIT signalMusicListChanged(key, d->m_listCache->getMusicTitleList(key));
        }
    }
}
