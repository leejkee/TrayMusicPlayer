//
// Created by cww on 25-4-1.
//
#pragma once

#include <QVBoxLayout>
#include <QWidget>

class QHBoxLayout;
class QGroupBox;
class QLabel;
class QSlider;
class QPushButton;
class QPropertyAnimation;

namespace Tray::Ui::Panel {
class ProgressBar;
class BetterButton;
class VolumeController;
class RotatingLabel;
class MarqueeLabel;
} // namespace Tray::Ui::Panel

namespace Tray::Ui {
class PlayerWidget final : public QWidget {
    Q_OBJECT

public:
    constexpr static int CIRCLE_LOGO_SIZE = 45;

    constexpr static int HEIGHT_PLAYER_WIDGET = 45;

    constexpr static int WIDTH_TITLE_LABEL = 120;

    explicit PlayerWidget(QWidget* parent = nullptr);

    void setSliderVolumeValue(unsigned v);

public Q_SLOTS:
    void setPlayButtonIcon(bool playStatus);

    void setVolumeCtrlButtonIcon(bool isMuted);

    void updateMusicName(const QString& songName);

    void updateMusicDuration(int s);

    void setRotationStatus(bool);

    void updatePlayModeIcon(int mode);

    void updateProgressBarPosition(qint64);

    void updateMuteIcon(const bool isMuted);

Q_SIGNALS:
    void signalNextMusic();

    void signalPreviousMusic();

    void signalPlayToggle();

    void signalSetVolume(int);

    void signalSetMusicPosition(qint64);

    void signalPlayModeChanged();

    void signalSetMute();

private:
    // left
    Panel::MarqueeLabel* m_labelMusicName;
    Panel::MarqueeLabel* m_labelArtistName;
    Panel::RotatingLabel* m_labelLogo;
    QHBoxLayout* m_leftLayout;

    // center
    Panel::BetterButton* m_pushButtonPlay;
    Panel::BetterButton* m_pushButtonPre;
    Panel::BetterButton* m_pushButtonNext;
    Panel::BetterButton* m_checkPlayMode;
    Panel::ProgressBar* m_progressWidget;
    QVBoxLayout* m_centerLayout;

    // right
    Panel::VolumeController* m_volumeController;
    Panel::BetterButton* m_pushButtonVolume;
    QMenu* m_menuVolume;

    void initWidget();

    void initLeftLayout();

    void initCenterLayout();

    void initVolumeControl();

    void initMainLayout();

    void createConnections();

    void showVolumeSlider() const;
};
} // namespace Tray::Ui
