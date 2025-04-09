//
// Created by cww on 25-4-2.
//

#ifndef VOLUMECONTROLLER_H
#define VOLUMECONTROLLER_H

#include <QWidget>

class QSlider;
class QLabel;
class QPushButton;


namespace UI::Panel {
    class BetterButton;

    class VolumeController final : public QWidget {
        Q_OBJECT

    public:
        explicit VolumeController(QWidget *parent = nullptr);

        void setVolume(int v) const;


    Q_SIGNALS:
        void signalSetValue(int);

    public Q_SLOTS:
        void setVolumeButtonIcon(bool) const;

    private:
        QSlider *m_sliderV;
        QLabel *m_labelVolume;
        BetterButton *m_buttonMute;

    };
}

#endif //VOLUMECONTROLLER_H
