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

    Core::Core(QObject *parent) : QObject(parent), d(std::make_unique<CorePrivate>()) {
        this->setObjectName(QStringLiteral("Core"));
        d->Log = Log::QLogger(this->objectName());
        d->m_player = new Player(this);
        d->m_playList = new PlayList(this);
        d->m_listCache = new ListCache( this);
        d->m_settings = new Settings(SETTINGS_WIN32_PATH, this);
        d->Log.log(Log::QLogger::LogLevel::Info, "Initializing Core successfully");
        createConnections();
    }

    void Core::createConnections() {
        connect(d->m_player, &Player::signalPlayingChanged, this, [this](const bool b) {
            Q_EMIT signalPlayingStatusChanged(b);
        });


        connect(d->m_playList, &PlayList::signalMusicChanged, this, [this]
        (const qsizetype index, const QString &name, const int duration) {
                    Q_EMIT signalCurrentMusicChanged(static_cast<int>(index), name, duration);
                }
        );
        // music changed

        connect(d->m_player, &Player::signalIsMuted, this, [this](const bool b) {
            Q_EMIT signalIsMuted(b);
        });

        connect(d->m_player, &Player::signalPositionChanged, this, [this](const qint64 pos) {
            Q_EMIT signalPositionChanged(pos);
        });

        connect(d->m_playList, &PlayList::signalPlayModeChanged, this, [this](const PlayMode mode) {
            Q_EMIT signalPlayModeChanged(mode);
        });

        connect(d->m_player, &Player::signalMusicOver, this, &Core::nextMusic);

        // connect(m_settings, &Service::Settings::signalUserListAdded, this, &Core::createUserPlaylistToDB);
        //
        // // update the local paths in UI
        // connect(m_settings, &Service::Settings::signalLocalSettingsChanged, this, [this]() {
        //     Q_EMIT signalLocalPathsChanged();
        // });
        //
        // // update the local paths in Core::Settings
        // connect(m_settings, &Service::Settings::signalLocalSettingsChanged, this, &Core::updateLocalMusicList);
        //
        // connect(m_listCache, &Service::ListCache::signalMusicInserted, this, &Core::insertSongToDB);
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

    // todo

    /* Interface End */

    void Core::playLocalMusicFromFirst() {
        Q_EMIT signalMusicListChanged(LOCAL_LIST_KEY, d->d->m_listCache->getMusicTitleList(LOCAL_LIST_KEY));
    }


    QStringList Core::getLocalMusicTitleList() {
        return d->m_listCache->getMusicTitleList(LOCAL_LIST_KEY);
    }


    /*       DB Operations Start       */
    void Core::createUserPlaylistToDB(const QString &listName) const {
        const auto connectName = "create_" + listName; {
            if (auto dbConnection = DatabaseManager(DB_PATH, connectName); dbConnection.
                createTable(listName)) {
                d->Log.log(Log::QLogger::LogLevel::Info, "createTable successfully: " + listName);
            }
        }
        QSqlDatabase::removeDatabase(connectName);
    }

    QVector<Song> Core::readUserPlaylistFromDB(const QString &listName) {
        QVector<Song> list;
        const auto connectName = "read_" + listName; {
            auto dbConnection = DatabaseManager(DB_PATH, connectName);
            list = dbConnection.readAllSongsFromTable(listName);
        }
        QSqlDatabase::removeDatabase(connectName);
        return list;
    }

    void Core::insertSongToDB(const QString &listName, const Service::Song &song) {
        const auto connectName = "insert_" + listName; {
            if (auto dbConnection = Service::DatabaseManager(DB_PATH, connectName); dbConnection.insertSong(
                listName, song)) {
                Log.log(Service::Logger_QT::LogLevel::Info, "insertSong successfully: " + listName);
            }
        }
        QSqlDatabase::removeDatabase(connectName);
    }

    /*       DB Operations End       */

    void Core::updateLocalMusicList() {
        d->m_listCache->loadLocalMusic(d->m_settings->getLocalMusicDirectories());
        if (d->m_playList->getListKey() == LOCAL_LIST_KEY) {
            d->m_playList->loadMusicList(LOCAL_LIST_KEY, d->m_listCache->findList(LOCAL_LIST_KEY));
        }
        Q_EMIT signalMusicListChanged(LOCAL_LIST_KEY, d->m_listCache->getMusicTitleList(LOCAL_LIST_KEY));
    }


    // todo


    void Core::addMusicToList(const QString &sourceListKey, const QString &destinationListKey, const int index) {
        const auto sourceList = d->m_listCache->findList(sourceListKey);
        if (sourceList.isEmpty()) {
            d->Log.log(Log::QLogger::LogLevel::Error, "No user list called: " + sourceListKey);
            return;
        }
        const auto song = sourceList[index];
        d->m_listCache->insertMusicToList(destinationListKey, song);
    }

}
