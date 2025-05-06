#pragma once

#include "Song.h"
#include "PlayMode.h"
#include <QLogger.h>
#include <QObject>
#include <memory>



namespace Tray::Core {
    class Player;
    class PlayList;
    class ListCache;
    class CorePrivate;

    class Core final : public QObject {
        Q_OBJECT
    public:
        explicit Core(QObject *parent = nullptr);

        void setVolume(unsigned int volume);

        void playToggle();

        void playToggleWithListAndIndex(const QString &listKey, int);

        void nextMusic();

        void preMusic();

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

        Q_SIGNALS:
        void signalPlayingStatusChanged(bool b);
        void signalCurrentMusicChanged(int, const QString &, int);
        void signalIsMuted(bool);
        void signalPositionChanged(qint64);
        void signalPlayModeChanged(PlayMode mode);


    private:
        std::unique_ptr<CorePrivate> d;

        void createUserPlaylistToDB(const QString &listName) const;

        static QVector<Song> readUserPlaylistFromDB(const QString &listName);

        void insertSongToDB(const QString &listName, const Song &song);

        void updateLocalMusicList();

        void playLocalMusicFromFirst();

        void createConnections();

        void initCacheUserPlaylist() const;
    };
} // namespace Core
