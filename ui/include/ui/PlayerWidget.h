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


namespace UI::Panel {
    class ProgressBar;
    class BetterButton;
    class VolumeController;
}


namespace UI::PlayerWidget {
    class PlayerWidget final : public QWidget {
        Q_OBJECT

    public:
        explicit PlayerWidget(QWidget *parent = nullptr);

    private:
        // left
        QLabel *m_labelMusicName;
        QLabel *m_labelLogo;
        QHBoxLayout *m_leftLayout;

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


    public Q_SLOTS:
        void setPlayButtonIcon(bool playStatus) const;

        void setVolumeCtrlButtonIcon(bool b) const;

        void setSongName(const QString &songName) const;

    Q_SIGNALS:
        void signalNextMusic();

        void signalPreviousMusic();

        void signalPlayToggle();

        void signalSetVolume(int);
    };
}
