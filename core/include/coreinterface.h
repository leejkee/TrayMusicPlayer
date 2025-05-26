//
// Created by cww on 25-5-26.
//
#pragma once
#include <QObject>

namespace Tray::Core {

class CoreInterface : public QObject {
    Q_OBJECT

public:
    explicit CoreInterface(QObject *parent = nullptr) : QObject(parent) {};

    virtual void initWork();

    static CoreInterface *newCoreInstance(QObject *parent = nullptr);

Q_SIGNALS:
    void signalNotifyUiPlayingStatusChanged(bool b);

    void signalNotifyUiCurrentMusicChanged(int, const QString &, int);

    void signalIsMuted(bool);

    void signalPositionChanged(qint64);

    void signalNotifyUiPlayModeChanged(int mode);

    void signalPlaylistSwitched(const QString &key, const QStringList &titleList);

    void signalNotifyUiUserKeySetsChanged(const QStringList &list);

    void signalPlaylistModified(const QString &key, const QStringList &list);

    void signalNotifyUiCurrentListKeyChanged(const QString &key);

    void signalCopyMusicFromListToList(const QString &sourceKey, const QString &destinationKey, int index);

    void signalRemoveMusicFromList(const QString &key, const QString &title);

    // to listCache section begin
    void signalRemoveUserPlaylistFromCache(const QString &key);

    void signalAddUserPlaylistToCache(const QString &chars);

    void signalRequestTitleList(const QString &key);

    void signalRequestSwitchPlaylistAndPlayIndex(const QString &key, int index);

    // to listCache section end

    // to Ui
    void signalNotifyUiToRemoveUserPlaylist(const QString &key);

    void signalNotifyUiToAddUserPlaylist(const QString &key);

    void signalNotifyUiToUpdateLocalPaths(const QStringList &list);

    void signalSendUiCurrentTitleList(const QString &key, const QStringList &list);

    void signalNotifyUiCacheModified(const QString &key, const QStringList &list);

    void signalInitUiDefaultSettings(const QStringList &localList, const QStringList &userKeys, unsigned volume);

public Q_SLOTS:
    virtual void setVolume(unsigned int volume);

    virtual void playToggle();

    virtual void playToggleWithListAndIndex(const QString &listKey, int);

    virtual void nextMusic();

    virtual void preMusic();

    virtual void setPlayerPosition(qint64 position);

    virtual void changePlayMode();

    virtual void requestPlaylist(const QString &);

    virtual QStringList getKeysOfUserPlaylist();

    virtual void addUserPlaylist(const QString &);

    virtual void removeUserPlaylist(const QString &);

    virtual QStringList getLocalMusicPaths();

    virtual void appendLocalMusicPath(const QString &);

    virtual void removeLocalMusicPath(const QString &);

    virtual void addMusicToList(const QString &sourceListKey, const QString &destinationListKey, int index);

    virtual void removeMusicFromList(const QString &key, const QString &songTitle);
};
}
