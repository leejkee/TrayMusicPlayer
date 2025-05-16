//
// Created by cww on 25-4-1.
//
#pragma once
#include <QWidget>
#include <memory>

class QStackedWidget;


namespace Tray::Ui {
    class SettingsWidget;
    class TopBarWidget;
    class MusicListWidget;
    class ViewWidget;
    class PlayerWidget;
    class WindowManagerPrivate;

    class WindowManager final : public QWidget {
        Q_OBJECT

    public:
        explicit WindowManager(QWidget *parent = nullptr);

        ~WindowManager() override;

        void initDefaultSettings(const QStringList &localDir, const QStringList &userKeys);

    Q_SIGNALS:
        void signalPlayToggle();

        void signalNextMusic();

        void signalPreMusic();

        void signalSetVolume(int);

        void signalSetPlayerPosition(qint64);

        void signalPlayModeChanged();

        void signalPlaylistChanged(const QString &);

        void signalViewPlayButtonClicked(const QString &, int);

        void signalPlaylistAdded(const QString &);

        void signalLocalMusicDirectoryAdded(const QString &);

        void signalLocalMusicDirectoryRemoved(const QString &);

        void signalAddSongToList(const QString &, const QString &, int);

        void signalDelSongFromList(const QString &, const QString &);

    public Q_SLOTS:
        void updateCurrentMusic(int index, const QString &name, int duration);

        void updateProgressBarPosition(qint64 pos);

        void updateVolumeCtrlButtonIcon(bool b);

        void updatePlayModeIcon(int mode);

        void updateViewList(const QString &name, const QStringList &titleList);

        void updatePlayingStatus(bool b);

        void updateUserPlaylistKeys(const QStringList &list);

    private:
        std::unique_ptr<WindowManagerPrivate> d;

        void createConnections();
    };
}
