//
// Created by cww on 25-4-1.
//
#pragma once

#include <QWidget>


class QGroupBox;
class QLabel;
class QSlider;
class QPushButton;


namespace UI::PlayerWidget {

    class VolumeWidget final : public QWidget {
    public:
        explicit VolumeWidget(QWidget *parent = nullptr);

        void loadDefaultSetting(float v) const;

        QSlider *m_sliderV;
        QLabel *m_labelVolume;
        QPushButton *m_buttonMute;
    };



    class PlayerWidget final : public QWidget {
        Q_OBJECT

    public:
        explicit PlayerWidget(QWidget *parent = nullptr);


        static QString convertSecondsToTime(int seconds);

        // left
        QLabel *m_labelMusicFileName;

        // center
        QPushButton *m_pushButtonPlay;
        QPushButton *m_pushButtonPre;
        QPushButton *m_pushButtonNext;
        ProgressBarWidget *m_progressWidget;

        // right
        VolumeWidget *m_volumeWidget;
        QPushButton *m_pushButtonVolume;
        QMenu *m_menuVolume;

    private:
        void createConnections();

        void setButtonVisible(bool b);

        void showVolumeSlider();

    Q_SIGNALS:
        void nextMusic();

        void previousMusic();

        void playToggle();

    public Q_SLOTS:
        void setPlayButtonIcon(bool playStatus);

        void setVolumeCtrlButtonIcon(int volume);

        void setSongName(const QString &songName);
    };
}
