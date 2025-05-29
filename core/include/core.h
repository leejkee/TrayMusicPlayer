#pragma once

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
    explicit Core(QObject* parent = nullptr);

    ~Core() override;

    void initWork();

Q_SIGNALS:
    void signalNotifyUiPlayingStatusChanged(bool b);

    void signalNotifyUiCurrentMusicChanged(int, const QString&, int);

    void signalPositionChanged(qint64);

    void signalNotifyUiPlayModeChanged(int mode);

    void signalPlaylistSwitched(const QString& key,
                                const QStringList& titleList);

    void signalNotifyUiUserKeySetsChanged(const QStringList& list);

    void signalPlaylistModified(const QString& key, const QStringList& list);

    void signalNotifyUiCurrentListKeyChanged(const QString& key);

    void signalCopyMusicFromListToList(const QString& sourceKey,
                                       const QString& destinationKey,
                                       int index);

    void signalRemoveMusicFromList(const QString& key, const QString& title);

    // to listCache section begin
    void signalRemoveUserPlaylistFromCache(const QString& key);

    void signalAddUserPlaylistToCache(const QString& chars);

    void signalRequestTitleList(const QString& key);

    void signalRequestSwitchPlaylistAndPlayIndex(const QString& key, int index);

    // to listCache section end

    // to Ui
    void signalNotifyUiToRemoveUserPlaylist(const QString& key);

    void signalNotifyUiToAddUserPlaylist(const QString& key);

    void signalNotifyUiToUpdateLocalPaths(const QStringList& list);

    void signalSendUiCurrentTitleList(const QString& key,
                                      const QStringList& list);

    void signalNotifyUiCacheModified(const QString& key,
                                     const QStringList& list);

    void signalInitUiDefaultSettings(const QStringList& localList,
                                     const QStringList& userKeys,
                                     unsigned volume);

    void signalNotifyUiUpdateMuteIcon(bool);

public Q_SLOTS:
    void setVolume(unsigned int volume);

    void setMute();

    void playToggle();

    void playToggleWithListAndIndex(const QString& listKey, int);

    void nextMusic();

    void preMusic();

    void setPlayerPosition(qint64 position);

    void changePlayMode();

    void requestPlaylist(const QString&);

    QStringList getKeysOfUserPlaylist();

    void addUserPlaylist(const QString&);

    void removeUserPlaylist(const QString&);

    QStringList getLocalMusicPaths();

    void appendLocalMusicPath(const QString&);

    void removeLocalMusicPath(const QString&);

    void addMusicToList(const QString& sourceListKey,
                        const QString& destinationListKey, int index);

    void removeMusicFromList(const QString& key, const QString& songTitle);

private:
    std::unique_ptr<CorePrivate> d;

    void createConnections();
};
} // namespace Tray::Core
