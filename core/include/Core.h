#pragma once

#include "../QLogger.h"
#include <QObject>


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
    class Core final : public  QObject{
    public:
        explicit Core(QObject *parent = nullptr);


        void setVolume(unsigned int volume);

        void playToggle();

        void playToggleWithListAndIndex(const QString &listKey, int) ;

        void nextMusic();

        void preMusic() ;

        void switchPlaylist(const QString &listName);

        QStringList getLocalMusicTitleList();

        void setPlayerPosition(qint64 position);

        void changePlayMode();

        void requestPlaylist(const QString &);

        // init the default settings
        void initDefaultSettings();

        QStringList getKeysOfUserPlaylist();

        void newUserList(const QString &);

        QStringList getLocalMusicPaths();

        void addLocalMusicPath(const QString &);

        void removeLocalMusicPath(const QString &);

        void addMusicToList(const QString &sourceListKey, const QString &destinationListKey, int index);

    private:
        Engine::Player *m_player;
        Log::QLogger Log;
        Service::Settings *m_settings;
        Service::PlayList *m_playList;
        Service::ListCache *m_listCache;

        void createUserPlaylistToDB(const QString &listName) const;

        static QVector<Service::Song> readUserPlaylistFromDB(const QString &listName);

        void insertSongToDB(const QString &listName, const Service::Song &song);

        void updateLocalMusicList();

        void playLocalMusicFromFirst();

        void createConnections();

        void initCacheUserPlaylist() const;
    };
} // namespace Core
