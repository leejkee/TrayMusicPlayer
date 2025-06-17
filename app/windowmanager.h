//
// Created by cww on 25-4-1.
//
#pragma once
#include <QWidget>
#include <memory>

namespace Tray::Core
{
class Core;
}

namespace Tray::Ui
{
class WindowManagerPrivate;

class WindowManager final : public QWidget
{
    Q_OBJECT

public:
    explicit WindowManager(Core::Core* core, QWidget* parent = nullptr);

    ~WindowManager() override;

    void initDefaultSettings(const QStringList& localDir, const QStringList& userKeys, unsigned volume);

Q_SIGNALS:
    void signalPlayToggle();

    void signalNextMusic();

    void signalPreMusic();

    void signalSetVolume(int);

    void signalSetPlayerPosition(qint64);

    void signalPlayModeChanged();

    void signalPlaylistButtonClicked(const QString&);

    void signalViewPlayButtonClicked(const QString&, int);

    void signalUserPlaylistButtonAdded(const QString&);

    void signalLocalMusicDirectoryAdded(const QString&);

    void signalLocalMusicDirectoryRemoved(const QString&);

    void signalMusicAddedToList(const QString&, const QString&, int);

    void signalMusicRemovedFromList(const QString&, const QString&);

    void signalUserPlaylistButtonRemoved(const QString&);

public Q_SLOTS:
    void updateCurrentMusic(int index, const QString& name, int duration);

    void updateProgressBarPosition(qint64 pos);

    void updateVolumeCtrlButtonIcon(bool b);

    void updatePlayModeIcon(int mode);

    void showCurrentTitleListToView(const QString& name, const QStringList& titleList);

    void updatePlayingStatus(bool b);

    void updateUserPlaylistKeys(const QStringList& list);

    void updateCurrentViewList(const QString& key, const QStringList& titleList);

    /// @brief This function is connected with the signal that will be emitted when current playlist changed
    /// this function calls the viewWidget's function to update the current playlist key
    /// @param key playlist key
    void updateCurrentPlaylistKey(const QString& key);

    void updateSettingsLocalPaths(const QStringList& paths);

    void removeUserPlaylistButton(const QString& key);

    void addUserPlaylistButton(const QString& key);

private:
    std::unique_ptr<WindowManagerPrivate> d;

    void createConnections();
};
}
