#pragma once
#include "ICore.h"
#include "Logger_QT.h"


namespace Core::Engine {
    class Player;
}


namespace Core::Service {
    class Settings;
}


namespace Core {
    class Core final : public ICore {
    public:
        explicit Core(QObject *parent = nullptr);

        void loadMusic(const QString &musicPath) override;

        void setVolume(float volume) override;
        void playToggle() override;

        void nextMusic() override;

        void preMusic() override;
    private:
        Engine::Player *m_player;
        Service::Logger_QT Log;
        Service::Settings *m_settings;
    };
} // namespace Core
