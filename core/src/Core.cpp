//
// Created by cww on 25-4-4.
//
#include <Core.h>
#include <ICore.h>
#include <Player.h>
#include <Settings.h>
#include <CoreConstants.h>
#include <ListCache.h>
#include <qscreen_platform.h>

inline void initMyQRC() {
    Q_INIT_RESOURCE(core);
}

namespace Core {
    Core::Core(QObject *parent) : ICore(parent) {
        initMyQRC();
        this->setObjectName(QStringLiteral("Core"));
        Log = Service::Logger_QT(this->objectName());
        m_player = new Engine::Player(this);
        m_settings = new Service::Settings(SETTINGS_WIN32, this);
        m_playList = new Service::PlayList({}, this);
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

        connect(m_playList, &Service::PlayList::signalMusicNameChanged, this, [this](const QString &name) {
            // Log.log(Service::Logger_QT::LogLevel::Info, "signal emitted, to tell ui the current music name changed: "
            //  + name);
            Q_EMIT signalCurrentMusicNameChanged(name);
        });

        connect(m_playList, &Service::PlayList::signalMusicDurationChanged, this, [this](const int seconds) {
            // Log.log(Service::Logger_QT::LogLevel::Info, "signal emitted, to tell ui the current music duration changed: "
            //  + QString::number(seconds) + " s.");
            Q_EMIT signalCurrentMusicDurationChanged(seconds);
        });

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
    }

    void Core::initDefaultSettings() {

        setVolume(m_settings->getDefaultVolume());
        m_playList->loadMusicList(m_listCache->findList(LOCAL_LIST_KEY));

        // test sgm
        m_playList->setCurrentMusicIndex(0);
        m_player->setMusicSource(m_playList->getCurrentMusicPath());
        // test sgm
    }

    void Core::loadMusic(const QString &musicPath) {
        m_player->setMusicSource(musicPath);
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

    void Core::setPlayMode(const Service::PlayMode &mode) {
        m_playList->setPlayMode(mode);
    }

    void Core::playToggle() {
        m_player->playTg();
    }

    void Core::switchMusicListByName(const QString &listName) {
        m_playList->loadMusicList(m_listCache->findList(listName));
    }

    QString Core::getDefaultMusicName() {
        return m_playList->getCurrentMusicTitle();
    }

    QStringList Core::getMusicListByName(const QString &listName) {
        const auto musicList = m_listCache->findList(listName);
        QStringList list;
        for (const auto &music : musicList) {
            list.append(music.getTitle());
        }
        return list;
    }

    void Core::setMusicPosition(const qint64 position) {
        m_player->setMusicPosition(position);
    }

    void Core::changePlayMode() {
        m_playList->changePlayMode();
    }

    ICore *ICore::create(QObject *parent) {
        return new Core(parent);
    }
}
