#pragma once
#include <ICore.h>
#include <Logger_qt.h>
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


        void setVolume(unsigned int volume) override;

        void playToggle() override;

        void playToggleWithListAndIndex(const QString &listKey, int) override;

        void nextMusic() override;

        void preMusic() override;

        void switchMusicListByName(const QString &listName) override;

        QStringList getLocalMusicTitleList() override;

        void setMusicPosition(qint64 position) override;

        void changePlayMode() override;

        void requestMusicListByName(const QString &) override;

        // init the default settings
        void initDefaultSettings() override;

        QStringList getKeysOfUserPlaylist() override;

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

        void updateLocalMusicList();

        void playLocalMusicFromFirst();

        void createConnections();
    };
} // namespace Core
