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


namespace UI::Panel {
    class ProgressBar;
    class BetterButton;
    class VolumeController;
    class RotatingLabel;
}


namespace UI::PlayerWidget {
    class PlayerWidget final : public QWidget {
        Q_OBJECT

    public:
        explicit PlayerWidget(QWidget *parent = nullptr);



    public Q_SLOTS:
        void setPlayButtonIcon(bool playStatus) const;

        void setVolumeCtrlButtonIcon(bool b) const;

        void setSongName(const QString &songName) const;

        void setRotationStatus(bool) const;

    Q_SIGNALS:
        void signalNextMusic();

        void signalPreviousMusic();

        void signalPlayToggle();

        void signalSetVolume(int);

    private:
        // left
        QLabel *m_labelMusicName;
        Panel::RotatingLabel *m_labelLogo;
        QVBoxLayout *m_leftLayout;
        QPropertyAnimation *m_animation;

        // center
        Panel::BetterButton *m_pushButtonPlay;
        Panel::BetterButton *m_pushButtonPre;
        Panel::BetterButton *m_pushButtonNext;
        Panel::ProgressBar *m_progressWidget;
        QVBoxLayout *m_centerLayout;

        // right
        Panel::VolumeController *m_volumeController;
        Panel::BetterButton *m_pushButtonVolume;
        QMenu *m_menuVolume;

        void initLeft();

        void initRight();

        void initCenter();

        void createConnections();

        void showVolumeSlider() const;
    };
}
