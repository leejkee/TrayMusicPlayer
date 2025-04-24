//
// Created by cww on 25-4-4.
//
#include <Core.h>
#include <ICore.h>
#include <Player.h>
#include <Settings.h>
#include <CoreConstants.h>
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


        connect(m_playList, &Service::PlayList::signalMusicChanged, this, [this](
            const qsizetype index, const QString &name,
            const int duration) {
                    Q_EMIT signalCurrentMusicChanged(static_cast<int>(index), name, duration);
                });
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

        connect(m_settings, &Service::Settings::signalUserListAdded, this, &Core::addUserListToDB);

        // update the local paths in UI
        connect(m_settings, &Service::Settings::signalLocalSettingsChanged, this, [this]() {
            Q_EMIT signalLocalPathsChanged();
        });


        // update the local paths in Core::Settings
        connect(m_settings, &Service::Settings::signalLocalSettingsChanged, this, &Core::updateLocalMusicList);
    }

    void Core::initDefaultSettings() {
        setVolume(m_settings->getDefaultVolume());
        playLocalMusicFromFirst();
    }


    void Core::setVolume(const unsigned volume) {
        m_player->setVolume(static_cast<float>(volume) / 100);
    }

    void Core::nextMusic() {
        m_playList->nextMusic();
        m_player->setMusicSource(m_playList->getCurrentMusicPath());
        m_player->playTg();
    }

    void Core::preMusic() {
        m_playList->preMusic();
        m_player->setMusicSource(m_playList->getCurrentMusicPath());
        m_player->playTg();
    }


    void Core::playToggle() {
        m_player->playTg();
    }

    void Core::playToggleWithListAndIndex(const QString &listKey, const int index) {
        if (m_playList->getListKey() != listKey) {
            Log.log(Service::Logger_QT::LogLevel::Info, "Current list is not [" + listKey + "], switch to it");
            m_playList->loadMusicList(listKey, m_listCache->findList(listKey));
            m_playList->setCurrentMusicIndex(0);
        } else {
            if (index != m_playList->getCurrentMusicIndex()) {
                m_playList->setCurrentMusicIndex(index);
            }
        }
        m_player->setMusicSource(m_playList->getCurrentMusicPath());
        m_player->playTg();
    }

    void Core::playLocalMusicFromFirst() {
        Q_EMIT signalMusicListChanged(LOCAL_LIST_KEY, m_listCache->getMusicTitleList(LOCAL_LIST_KEY));
    }

    void Core::switchMusicListByName(const QString &listName) {
        m_playList->loadMusicList(listName, m_listCache->findList(listName));
    }

    QStringList Core::getLocalMusicTitleList() {
        return m_listCache->getMusicTitleList(LOCAL_LIST_KEY);
    }


    void Core::setMusicPosition(const qint64 position) {
        m_player->setMusicPosition(position);
    }

    void Core::changePlayMode() {
        m_playList->changePlayMode();
    }

    void Core::requestMusicListByName(const QString &listName) {
        Q_EMIT signalMusicListChanged(listName, m_listCache->getMusicTitleList(listName));
    }

    QStringList Core::getKeysUserList() {
        return m_settings->getUserMusicList();
    }

    void Core::addUserList(const QString &listName) {
        m_settings->addUserMusicList(listName);
    }

    void Core::addUserListToDB(const QString &listName) const {
        const auto connectName = "c_" + listName; {
            if (auto dbConnection = Service::DatabaseManager(DB_PATH, connectName); !dbConnection.
                createTable(listName)) {
                Log.log(Service::Logger_QT::LogLevel::Error, "createTable failed: " + listName);
            }
        }
        QSqlDatabase::removeDatabase(connectName);
    }

    void Core::updateLocalMusicList() {
        m_listCache->loadLocalMusic(m_settings->getLocalMusicDirectories());
        if (m_playList->getListKey() == LOCAL_LIST_KEY) {
            m_playList->loadMusicList(LOCAL_LIST_KEY, m_listCache->findList(LOCAL_LIST_KEY));
        }
        Q_EMIT signalMusicListChanged(LOCAL_LIST_KEY, m_listCache->getMusicTitleList(LOCAL_LIST_KEY));
    }

    QStringList Core::getLocalMusicPaths() {
        return m_settings->getLocalMusicDirectories();
    }

    void Core::addLocalMusicPath(const QString &path) {
        m_settings->addLocalMusicDirectory(path);
    }

    void Core::removeLocalMusicPath(const QString &path) {
        m_settings->removeLocalMusicDirectory(path);
    }

    ICore *ICore::create(QObject *parent) {
        return new Core(parent);
    }
}
