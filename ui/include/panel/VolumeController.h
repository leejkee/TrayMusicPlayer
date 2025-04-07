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
    public:
        explicit VolumeController(QWidget *parent = nullptr);


    private:
        QSlider *m_sliderV;
        QLabel *m_labelVolume;
        BetterButton *m_buttonMute;
        void setDefaultVolume(float v) const;
    };
}

#endif //VOLUMECONTROLLER_H
