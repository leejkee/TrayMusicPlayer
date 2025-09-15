//
// Created by cww on 25-4-1.
//
#pragma once
#include <QWidget>
#include <memory>

namespace Tray::Ui {
class PlayerWidgetPrivate;
class PlayerWidget final : public QWidget {
    Q_OBJECT

public:
    constexpr static int CIRCLE_LOGO_SIZE = 45;

    constexpr static int HEIGHT_PLAYER_WIDGET = 45;

    constexpr static int WIDTH_TITLE_LABEL = 120;

    explicit PlayerWidget(QWidget* parent = nullptr);

    ~PlayerWidget() override;

    void setSliderVolumeValue(int v);

public Q_SLOTS:
    void setPlayButtonIcon(bool playStatus);

    void setVolumeCtrlButtonIcon(bool isMuted);

    void updateMusicName(const QString& songName);

    void updateMusicDuration(int s);

    void setRotationStatus(bool);

    void updatePlayModeIcon(int mode);

    void updateProgressBarPosition(qint64);

    void updateMuteIcon(bool isMuted);

Q_SIGNALS:
    void signalNextMusic();

    void signalPreviousMusic();

    void signalPlayToggle();

    void signalSetVolume(int);

    void signalSetMusicPosition(qint64);

    void signalPlayModeChanged();

    void signalSetMute();

private:
    std::unique_ptr<PlayerWidgetPrivate> d;

    void initLeftLayout();

    void initCenterLayout();

    void initVolumeControl();

    void initMainLayout();

    void createConnections();

    void showVolumeSlider() const;
};
} // namespace Tray::Ui
