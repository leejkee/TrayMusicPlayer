#pragma once

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
        explicit Core(const QString &iniPath, QObject *parent = nullptr);
        ~Core() override;

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


        QStringList getKeysOfUserPlaylist();

        void newUserList(const QString &);

        QStringList getLocalMusicPaths();

        void appendLocalMusicPath(const QString &);

        void removeLocalMusicPath(const QString &);

        void addMusicToList(const QString &sourceListKey, const QString &destinationListKey, int index);

        void removeMusicFromList(const QString &key, const QString &songTitle);

    Q_SIGNALS:
        void signalPlayingStatusChanged(bool b);

        void signalCurrentMusicChanged(int, const QString &, int);

        void signalIsMuted(bool);

        void signalPositionChanged(qint64);

        void signalPlayModeChanged(int mode);

        void signalPlaylistSwitched(const QString &key, const QStringList &titleList);

        ///
        ///@brief Signals that the set of user playlists has changed.
        ///
        ///This signal is emitted whenever a user playlist is added or deleted.
        ///It notifies the UI to refresh its display of the playlist collection.
        ///
        ///@param list The current list of user playlist names.
        void signalUserPlaylistSetsChanged(const QStringList &list);

        void signalPlaylistModified(const QString & key, const QStringList & list);

    private:
        std::unique_ptr<CorePrivate> d;


        void updateCurrentPlaylist(const QString &key);

        void playLocalMusicFromFirst();

        void createConnections();
        // init the default settings
        void initDefaultSettings();
    };
} // namespace Core
