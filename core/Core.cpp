//
// Created by cww on 25-4-4.
//
#include "include/Core.h"
#include "config.h"
#include <Player.h>
#include <PlayList.h>
#include <Settings.h>
#include <ListCache.h>
#include <DatabaseManager.h>


namespace Core {
    Core::Core(QObject *parent) : ICore(parent) {
        this->setObjectName(QStringLiteral("Core"));
        Log = Service::Logger_QT(this->objectName());
        m_player = new Engine::Player(this);
        m_settings = new Service::Settings(SETTINGS_WIN32, this);
        m_playList = new Service::PlayList(this);
        m_listCache = new Service::ListCache(m_settings->getLocalMusicDirectories(), this);
        Log.log(Service::Logger_QT::LogLevel::Info, "Initializing Core successfully");
        createConnections();
    }

    void Core::createConnections() {
        connect(m_player, &Engine::Player::signalPlayingChanged, this, [this](const bool b) {
            // Log.log(Service::Logger_QT::LogLevel::Info, "signal emitted, to tell ui the playing status changed: " +
            // QString::number(b));
            Q_EMIT signalPlayingChanged(b);
        });


        connect(m_playList, &Service::PlayList::signalMusicChanged, this, [this]
        (const qsizetype index, const QString &name, const int duration) {
                    Q_EMIT signalCurrentMusicChanged(static_cast<int>(index), name, duration);
                }
        );
        // music changed

        connect(m_player, &Engine::Player::signalIsMuted, this, [this](const bool b) {
            // Log.log(Service::Logger_QT::LogLevel::Info, "signal emitted, to tell ui the output is Muted: " + QString::number(b));
            Q_EMIT signalIsMuted(b);
        });

        connect(m_player, &Engine::Player::signalPositionChanged, this, [this](const qint64 pos) {
            // Log.log(Service::Logger_QT::LogLevel::Info, "signal emitted, to tell ui the music position is updated: " + QString::number(pos));
            Q_EMIT signalPositionChanged(pos);
        });

        connect(m_playList, &Service::PlayList::signalPlayModeChanged, this, [this](const Service::PlayMode mode) {
            // Log.log(Service::Logger_QT::LogLevel::Info, "signal emitted, to notice the ui to update the play mode" +
            // PlayModeToString(mode));
            Q_EMIT signalPlayModeChanged(static_cast<int>(mode));
        });

        connect(m_player, &Engine::Player::signalMusicOver, this, &Core::nextMusic);

        connect(m_settings, &Service::Settings::signalUserListAdded, this, &Core::createUserPlaylistToDB);

        // update the local paths in UI
        connect(m_settings, &Service::Settings::signalLocalSettingsChanged, this, [this]() {
            Q_EMIT signalLocalPathsChanged();
        });

        // update the local paths in Core::Settings
        connect(m_settings, &Service::Settings::signalLocalSettingsChanged, this, &Core::updateLocalMusicList);

        connect(m_listCache, &Service::ListCache::signalMusicInserted, this, &Core::insertSongToDB);
    }

    void Core::initCacheUserPlaylist() const {
        for (const auto &key: m_settings->getKeysUserPlaylist()) {
            m_listCache->loadUserList(key, readUserPlaylistFromDB(key));
        }
    }

    void Core::initDefaultSettings() {
        setVolume(m_settings->getDefaultVolume());
        playLocalMusicFromFirst();
        initCacheUserPlaylist();
    }


    /* Interface Begin */
    // 1
    void Core::playToggle() {
        m_player->playTg();
    }

    // 2
    void Core::nextMusic() {
        m_playList->nextMusic();
        m_player->setMusicSource(m_playList->getCurrentMusicPath());
        m_player->playTg();
    }

    // 3
    void Core::preMusic() {
        m_playList->preMusic();
        m_player->setMusicSource(m_playList->getCurrentMusicPath());
        m_player->playTg();
    }

    // 4
    void Core::playToggleWithListAndIndex(const QString &listKey, const int index) {
        /// check playlist ?
        if (m_playList->getListKey() != listKey) {
            // yes
            Log.log(Service::Logger_QT::LogLevel::Info, "Current list is not [" + listKey + "], switch to it");
            m_playList->loadMusicList(listKey, m_listCache->findList(listKey));
            m_playList->setCurrentMusicIndex(index);
            m_player->setMusicSource(m_playList->getCurrentMusicPath());
        } else {
            // no
            /// check music ?
            if (index != m_playList->getCurrentMusicIndex()) {
                // yes
                m_playList->setCurrentMusicIndex(index);
                m_player->setMusicSource(m_playList->getCurrentMusicPath());
            }
        }
        m_player->playTg();
    }

    // 5
    void Core::setVolume(const unsigned volume) {
        m_player->setVolume(static_cast<float>(volume) / 100);
    }

    // 6
    void Core::switchPlaylist(const QString &listName) {
        m_playList->loadMusicList(listName, m_listCache->findList(listName));
    }

    // 7
    void Core::requestPlaylist(const QString &listName) {
        Q_EMIT signalMusicListChanged(listName, m_listCache->getMusicTitleList(listName));
    }

    // 8
    void Core::setPlayerPosition(const qint64 position) {
        m_player->setMusicPosition(position);
    }

    // 9
    void Core::changePlayMode() {
        m_playList->changePlayMode();
    }

    // 10
    QStringList Core::getKeysOfUserPlaylist() {
        return m_settings->getKeysUserPlaylist();
    }

    // 11
    void Core::newUserList(const QString &listName) {
        m_settings->addUserMusicList(listName);
    }

    // 12
    QStringList Core::getLocalMusicPaths() {
        return m_settings->getLocalMusicDirectories();
    }

    /* Interface End */

    void Core::playLocalMusicFromFirst() {
        Q_EMIT signalMusicListChanged(LOCAL_LIST_KEY, m_listCache->getMusicTitleList(LOCAL_LIST_KEY));
    }


    QStringList Core::getLocalMusicTitleList() {
        return m_listCache->getMusicTitleList(LOCAL_LIST_KEY);
    }


    /*       DB Operations Start       */
    void Core::createUserPlaylistToDB(const QString &listName) const {
        const auto connectName = "create_" + listName; {
            if (auto dbConnection = Service::DatabaseManager(DB_PATH, connectName); dbConnection.
                createTable(listName)) {
                Log.log(Service::Logger_QT::LogLevel::Info, "createTable successfully: " + listName);
            }
        }
        QSqlDatabase::removeDatabase(connectName);
    }

    QVector<Service::Song> Core::readUserPlaylistFromDB(const QString &listName) {
        QVector<Service::Song> list;
        const auto connectName = "read_" + listName; {
            auto dbConnection = Service::DatabaseManager(DB_PATH, connectName);
            list = dbConnection.readSongs(listName);
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
        m_listCache->loadLocalMusic(m_settings->getLocalMusicDirectories());
        if (m_playList->getListKey() == LOCAL_LIST_KEY) {
            m_playList->loadMusicList(LOCAL_LIST_KEY, m_listCache->findList(LOCAL_LIST_KEY));
        }
        Q_EMIT signalMusicListChanged(LOCAL_LIST_KEY, m_listCache->getMusicTitleList(LOCAL_LIST_KEY));
    }


    void Core::addLocalMusicPath(const QString &path) {
        m_settings->addLocalMusicDirectory(path);
    }

    void Core::removeLocalMusicPath(const QString &path) {
        m_settings->removeLocalMusicDirectory(path);
    }

    void Core::addMusicToList(const QString &sourceListKey, const QString &destinationListKey, const int index) {
        const auto sourceList = m_listCache->findList(sourceListKey);
        if (sourceList.isEmpty()) {
            Log.log(Service::Logger_QT::LogLevel::Error, "No user list called: " + sourceListKey);
            return;
        }
        const auto song = sourceList[index];
        m_listCache->insertMusicToList(destinationListKey, song);
    }

    ICore *ICore::create(QObject *parent) {
        return new Core(parent);
    }
}
