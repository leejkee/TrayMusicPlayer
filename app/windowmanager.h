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

    void updatePlayingStatus(bool b);

private:
    std::unique_ptr<WindowManagerPrivate> d;

    void createConnections();
};
}
