//
// Created by cww on 25-4-2.
//
#include "VolumeController.h"
#include <BetterButton.h>
#include <UiConfig.h>
#include <TraySVG.h>
#include <TrayQSS.h>
#include <QSlider>
#include <QVBoxLayout>
#include <QLabel>

namespace Tray::Ui::Panel {
    VolumeController::VolumeController(QWidget *parent) : QWidget(parent) {
        m_sliderV = new QSlider(this);
        m_sliderV->setRange(0, 100);
        m_sliderV->setSingleStep(5);
        m_sliderV->setStyleSheet(readQSS(Res::VOLUME_SLIDER_QSS));

        QHBoxLayout *hSliderLayout = new QHBoxLayout;
        hSliderLayout->addStretch();
        hSliderLayout->addWidget(m_sliderV);
        hSliderLayout->addStretch();
        hSliderLayout->setContentsMargins(5, 0, 5, 0);

        m_labelVolume = new QLabel(this);
        m_labelVolume->setStyleSheet("font-size: 7pt;");
        m_labelVolume->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        m_buttonMute = new BetterButton(QIcon(Res::VolumeBtnSVG), this);
        m_buttonMute->setIconSize(QSize(10, 10));

        QVBoxLayout *layout = new QVBoxLayout;
        // layout->addWidget(m_sliderV);
        layout->addItem(hSliderLayout);
        layout->addWidget(m_labelVolume);

        layout->addWidget(m_buttonMute);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        setLayout(layout);
        // this->setFixedSize(30, 120);
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        connect(m_sliderV, &QSlider::valueChanged, this, &VolumeController::setLabelVolume);
        connect(m_sliderV, &QSlider::valueChanged, this, [this](const int v) {
            Q_EMIT signalSetValue(v);
        });
    }

    void VolumeController::setLabelVolume(const int v) {
        m_labelVolume->setText(QString("%1%").arg(v));
    }

    void VolumeController::setSliderVolumeValue(const unsigned v) {
        m_sliderV->setValue(v);
    }

    void VolumeController::setVolumeButtonIcon(const bool isMuted)  {
        if (isMuted) {
            m_buttonMute->setIcon(QIcon(Res::VolumeMuteSVG));
        } else {
            m_buttonMute->setIcon(QIcon(Res::VolumeBtnSVG));
        }
    }
    QSize VolumeController::sizeHint() const {
        return {30, 120};
    }
}
