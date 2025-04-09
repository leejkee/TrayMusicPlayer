//
// Created by cww on 25-4-2.
//
#include <panel/VolumeController.h>
#include <panel/BetterButton.h>
#include <ui/Assets.h>

#include <QSlider>
#include <QVBoxLayout>
#include <QLabel>

namespace UI::Panel {
    VolumeController::VolumeController(QWidget *parent) : QWidget(parent) {
        m_sliderV = new QSlider(this);
        m_sliderV->setRange(0, 100);
        m_sliderV->setSingleStep(5);
        m_sliderV->setValue(10);
        m_sliderV->setStyleSheet(Tools::readQSS(QssRes::VOLUME_SLIDER_QSS));

        QHBoxLayout *hSliderLayout = new QHBoxLayout;
        hSliderLayout->addStretch();
        hSliderLayout->addWidget(m_sliderV);
        hSliderLayout->addStretch();
        hSliderLayout->setContentsMargins(0, 0, 0, 0);

        m_labelVolume = new QLabel(this);
        m_labelVolume->setStyleSheet("font-size: 7pt;");
        m_labelVolume->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        m_buttonMute = new BetterButton(QIcon(SvgRes::VolumeSVG), this);
        m_buttonMute->setIconSize(QSize(10, 10));

        QVBoxLayout *layout = new QVBoxLayout;
        // layout->addWidget(m_sliderV);
        layout->addItem(hSliderLayout);
        layout->addWidget(m_labelVolume);

        layout->addWidget(m_buttonMute);
        layout->setContentsMargins(1, 5, 0, 0);
        layout->setSpacing(0);
        setLayout(layout);
        this->setFixedSize(30, 120);

        setVolume(10);
        connect(m_sliderV, &QSlider::valueChanged, this, &VolumeController::setVolume);
        connect(m_sliderV, &QSlider::valueChanged, this, [this](const int v) {
            Q_EMIT signalSetValue(v);
        });
    }

    void VolumeController::setVolume(const int v) const {
        m_labelVolume->setText(QString("%1%").arg(v));
    }

    void VolumeController::setVolumeButtonIcon(const bool isMuted) const {
        if (isMuted) {
            m_buttonMute->setIcon(QIcon(SvgRes::VolumeMuteSVG));
        } else {
            m_buttonMute->setIcon(QIcon(SvgRes::VolumeSVG));
        }
    }
}
