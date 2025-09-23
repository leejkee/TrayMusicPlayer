//
// Created by 31305 on 2025/9/9.
//
#pragma once
#include <QWidget>

namespace Tray::Ui
{
class WindowManagerPrivate;

class WindowManager final : public QWidget
{
    Q_OBJECT

public:
    struct WindowInitData
    {
        QString initKey;
        QStringList initLocalPaths;
        QStringList initUserKeys;
        QStringList initTitleList;
        int initVolume;
    };

    explicit WindowManager(const WindowInitData& initData
                           , QWidget* parent = nullptr);

    ~WindowManager() override;

Q_SIGNALS:
    void signalPlayToggle();

    void signalNextMusic();

    void signalPreMusic();

    void signalSetVolume(int);

    void signalSetPlayerPosition(qint64);

    void signalChangePlayMode();

    void signalPlaylistButtonClicked(const QString&);

    void signalViewPlayButtonClicked(const QString&, int);

    void signalUserPlaylistButtonAdded(const QString&);

    void signalLocalMusicDirectoryAdded(const QString&);

    void signalLocalMusicDirectoryRemoved(const QString&);

    void signalCopyMusicToList(const QString&, const QString&, int);

    void signalRemoveMusicFromList(const QString&, const QString&);

    void signalUserPlaylistButtonRemoved(const QString&);

public Q_SLOTS:
    void updateCurrentMusic(int index, const QString& name, int duration);

    void updatePlayingStatus(bool b);

    void updateProgressBarPosition(qint64 pos);

    void updateMuteIcon(bool isMuted);

    void updatePlayModeIcon(int mode);

    void removeUserListButton(const QString& listKey);

    void addUserListButton(const QString& listKey);

    void handlePlaylistDisplayingOnView(const QString& listKey
                                        , const QStringList& titleList);

    void handleListCacheUpdated(const QString& listKey
                                , const QStringList& titleList);

    void handleUserListKeysUpdated(const QStringList& keys);

    void handleCurrentPlaylistKeyChanged(const QString& listKey);

    void handleLocalPathSettingsUpdated(const QStringList& localPaths);

    void updateLyric(const QStringList& lyricText
                     , const QList<int64_t>& lyricsTiming);

    void updateLyricLineIndex(int index);

private:
    std::unique_ptr<WindowManagerPrivate> d;

    void createConnections();

    void initDefaultSettings(const WindowInitData& initData);
};
}
