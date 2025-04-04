//
// Created by cww on 25-4-4.
//
#include <Core.h>
#include <ICore.h>
#include <Player.h>
#include <Settings.h>
#include <CoreConstants.h>

inline void initQRC() {
    Q_INIT_RESOURCE(core);
}
namespace Core {
    Core::Core(QObject *parent) : ICore(parent) {
        initQRC();
        this->setObjectName("Core");
        Log = Service::Logger_QT(this->objectName());
        m_player = new Engine::Player(this);
        m_settings = new Service::Settings(SETTINGS_WIN32, this);
        // test sgm
        // test sgm
    }


    void Core::loadMusic(const QString &musicPath) {
        m_player->setMusicSource(musicPath);
    }

    void Core::setVolume(const float volume) {
        m_player->setVolume(volume);
    }

    void Core::nextMusic() {
    }

    void Core::preMusic() {
    }

    void Core::playToggle() {
        m_player->playTg();
    }

    ICore* ICore::create(QObject *parent) {
        return new Core(parent);
    }
}
