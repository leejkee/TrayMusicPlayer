#pragma once
#include <ICore.h>
#include <Logger_QT.h>
#include <PlayList.h>


namespace Core::Engine {
    class Player;
}


namespace Core::Service {
    class Settings;
    class PlayList;
    class ListCache;
    class Song;
}


namespace Core {
    class Core final : public ICore {
    public:
        explicit Core(QObject *parent = nullptr);

        void createConnections();

        void loadMusic(const QString &musicPath) override;

        void setVolume(unsigned int volume) override;

        void playToggle() override;

        void playToggleIndex(int) override;

        void nextMusic() override;

        void preMusic() override;

        void setPlayMode(const Service::PlayMode &) override;

        void switchMusicListByName(const QString &listName) override;

        void setMusicPosition(qint64 position) override;

        void changePlayMode() override;

        QStringList getMusicListByName(const QString &name) override;

        void requestMusicListByName(const QString &) override;

        // init the default settings
        void initDefaultSettings() override;

        QStringList getKeysUserList() override;

    private:
        Engine::Player *m_player;
        Service::Logger_QT Log;
        Service::Settings *m_settings;
        Service::PlayList *m_playList;
        Service::ListCache *m_listCache;
    };
} // namespace Core
