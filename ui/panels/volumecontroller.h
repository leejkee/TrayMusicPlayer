//
// Created by cww on 25-4-2.
//

#pragma once

#include <QWidget>

class QSlider;
class QLabel;
class QPushButton;


namespace Tray::Ui::Panel {
    class BetterButton;

    class VolumeController final : public QWidget {
        Q_OBJECT

    public:
        explicit VolumeController(QWidget *parent = nullptr);

        void setSliderVolumeValue(unsigned v);

    protected:
        QSize sizeHint() const override;


    Q_SIGNALS:
        void signalSetValue(int);

        void signalSetMute();

    public Q_SLOTS:
        void setVolumeButtonIcon(bool);


    private:
        QSlider *m_sliderV;
        QLabel *m_labelVolume;
        BetterButton *m_buttonMute;

        void setLabelVolume(unsigned v);
    };
}

