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
    struct Song;
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

        void switchMusicListByName(const QString &listName) override;

        void setMusicPosition(qint64 position) override;

        void changePlayMode() override;

        void requestMusicListByName(const QString &) override;

        // init the default settings
        void initDefaultSettings() override;

        QStringList getKeysUserList() override;

        void addUserList(const QString &) override;

        QStringList getLocalMusicPaths() override;

        void addLocalMusicPath(const QString &) override;

        void removeLocalMusicPath(const QString &) override;

    private:
        Engine::Player *m_player;
        Service::Logger_QT Log;
        Service::Settings *m_settings;
        Service::PlayList *m_playList;
        Service::ListCache *m_listCache;

        void addUserListToDB(const QString &listName) const;

        /// Retrieves the music titles belonging to the specified playlist.
        /// @return QStringList of music titles, empty if playlist doesn't exist.
        [[nodiscard]] QStringList getMusicListByName(const QString &name) const;

        void updateLocalMusicList();
    };
} // namespace Core
