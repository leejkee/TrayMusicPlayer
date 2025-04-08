//
// Created by cww on 25-4-4.
//
#include <Core.h>
#include <ICore.h>
#include <Player.h>
#include <Settings.h>
#include <CoreConstants.h>
#include <ListCache.h>

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
        initDefaultSettings();
    }

    void Core::initDefaultSettings() {
        setVolume(m_settings->getDefaultVolume());

        m_playList->loadMusicList(m_listCache->findList(LOCAL_LIST_KEY));

        // test sgm
        m_playList->setCurrentMusicIndex(0);
        m_player->setMusicSource(m_playList->getCurrentMusicPath());
        // test sgm

        connect(m_player, &Engine::Player::signalPlayingChanged, this, [this](const bool b) {
            Log.log(Service::Logger_QT::LogLevel::Info, "signal emitted, to tell ui the playing status changed: " +
            QString::number(b));
            Q_EMIT signalPlayingChanged(b);
        });

        connect(m_playList, &Service::PlayList::signalMusicNameChanged, this, [this](const QString &name) {
            Log.log(Service::Logger_QT::LogLevel::Info, "signal emitted, to tell ui the current music name changed: " + name);
            Q_EMIT signalCurrentMusicNameChanged(name);
        });

        connect(m_player, &Engine::Player::signalIsMuted, this, [this](const bool b) {
            Log.log(Service::Logger_QT::LogLevel::Info, "signal emitted, to tell ui the output is Muted: " + QString::number(b));
            Q_EMIT signalIsMuted(b);
        });

    }

    void Core::loadMusic(const QString &musicPath) {
        m_player->setMusicSource(musicPath);
    }

    void Core::setVolume(const unsigned volume) {
        m_player->setVolume(static_cast<float>(volume) / 100);
    }

    void Core::nextMusic() {
        Log.log(Service::Logger_QT::LogLevel::Info, "nextMusic");
        m_playList->setCurrentMusicIndex(m_playList->getCurrentMusicIndex() + 1);
        m_player->setMusicSource(m_playList->getCurrentMusicPath());
        m_player->playTg();
    }

    void Core::preMusic() {
        Log.log(Service::Logger_QT::LogLevel::Info, "preMusic");
        m_playList->setCurrentMusicIndex(m_playList->getCurrentMusicIndex() - 1);
        m_player->setMusicSource(m_playList->getCurrentMusicPath());
        m_player->playTg();
    }

    void Core::playToggle() {
        m_player->playTg();
    }

    void Core::switchMusicListByName(const QString &listName) {
        m_playList->loadMusicList(m_listCache->findList(listName));
    }

    ICore *ICore::create(QObject *parent) {
        return new Core(parent);
    }
}
