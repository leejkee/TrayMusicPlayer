//
// Created by cww on 25-4-2.
//
#include <panel/VolumeController.h>
#include <panel/BetterButton.h>
#include <ui/Assets.h>

#include <QSlider>
#include <QVBoxLayout>
#include <QLabel>

namespace UI::Panel{

VolumeController::VolumeController(QWidget *parent) : QWidget(parent) {
    m_sliderV = new QSlider(this);
    m_sliderV->setRange(0, 100);
    m_labelVolume = new QLabel(this);
    m_labelVolume->setText("30%");
    m_labelVolume->setStyleSheet("font-size: 7pt;");
    m_labelVolume->setAlignment(Qt::AlignCenter);
    connect(m_sliderV, &QSlider::valueChanged, m_labelVolume, [=]() {
        m_labelVolume->setText(QString("%1%").arg(m_sliderV->value()));
    });
    m_buttonMute = new BetterButton(QIcon(SvgRes::VolumeSVG), this);
    m_buttonMute->setIconSize(QSize(10, 10));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_sliderV);
    layout->addWidget(m_labelVolume);
    layout->addWidget(m_buttonMute);
    layout->setContentsMargins(5, 5, 0, 0);
    setLayout(layout);
    this->setFixedSize(30, 110);
}

void VolumeController::setDefaultVolume(const float v) const {
    m_sliderV->setValue(static_cast<int>(v * 100));
    m_labelVolume->setText(QString("%1%").arg(m_sliderV->value()));
}
}