//
// Created by 31305 on 2025/8/26.
//
#pragma once
#include <memory>
#include <QObject>

namespace Tray::App
{
class CoreServicePrivate;
class CoreService final: public QObject
{
public:
    explicit CoreService(QObject *parent = nullptr);

    void initConnections();

    void initPreload();

    ~CoreService() override;


Q_SIGNALS:

    /*!
     * \brief Requests to switch to a playlist and play a song at the given index
     *
     * This signal is emitted when the user clicks the play button on a song
     * that belongs to a different playlist. The system should load the target
     * playlist by its name and start playing the song at the specified index.
     *
     * \param listKey The name of the target playlist (used as the key in internal storage)
     * \param index The index of the song within that playlist (0-based)
     *
     * \sa signalRequestTitleList()
     */
    void signalRequestSwitchPlaylistAndPlayIndex(const QString& listKey, int index);

    void signalRequestTitleList(const QString& listKey);

    void signalAddUserPlaylistToCache(const QString& chars);
    void signalRemoveUserPlaylistFromCache(const QString& key);
    void signalCopyMusicFromListToList(const QString& chars
                                      , const QString& string
                                      , int index);
    void signalRemoveMusicFromList(const QString& key, const QString& chars);

public Q_SLOTS:

    void setVolume(int volume);
    void playToggle();
    void playToggleWithListAndIndex(const QString& listKey, int index);
    void nextMusic();
    void preMusic();
    void setPlayerPosition(qint64 position);
    void changePlayMode();
    void requestPlaylist(const QString& listName);
    QStringList getKeysOfUserPlaylist();
    void addUserPlaylist(const QString& listName);
    void removeUserPlaylist(const QString& key);
    QStringList getLocalMusicPaths();
    void appendLocalMusicPath(const QString& path);
    void removeLocalMusicPath(const QString& path);
    void addMusicToList(const QString& sourceListKey
                        , const QString& destinationListKey
                        , int index);
    void removeMusicFromList(const QString& key, const QString& songTitle);
    void setMute();

private:
    std::unique_ptr<CoreServicePrivate> d;
};

}
