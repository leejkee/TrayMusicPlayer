//
// Created by cww on 25-4-2.
//
#include <volumecontroller/volumecontroller.h>
#include <stylebutton/stylebutton.h>
#include <traysvg.h>
#include <trayqss.h>
#include <styleslider/styleslider.h>
#include <QVBoxLayout>
#include <QLabel>

namespace Tray::Ui::Panel
{
class VolumeControllerPrivate
{
public:
    StyleSlider* m_sliderV;
    QLabel* m_labelVolume;
    StyleButton* m_buttonMute;
};

VolumeController::VolumeController(QWidget* parent)
    : QWidget(parent), d(std::make_unique<VolumeControllerPrivate>())
{
    d->m_sliderV = new StyleSlider(Qt::Vertical, this);
    d->m_sliderV->setRange(0, 100);
    d->m_sliderV->setSingleStep(5);
    d->m_sliderV->setStyleSheet(Res::readQss(Res::VOLUME_CONTROLLER_QSS));

    QHBoxLayout* hSliderLayout = new QHBoxLayout;
    hSliderLayout->addStretch();
    hSliderLayout->addWidget(d->m_sliderV);
    hSliderLayout->addStretch();
    hSliderLayout->setContentsMargins(5, 0, 5, 0);

    d->m_labelVolume = new QLabel(this);
    d->m_labelVolume->setStyleSheet("font-size: 7pt;");
    d->m_labelVolume->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    d->m_buttonMute = new StyleButton({}
                                      , {30, 30}
                                      , Res::VolumeBtnSVG
                                      , Res::BUTTON_NORMAL_QSS
                                      , this);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(d->m_labelVolume);
    layout->addItem(hSliderLayout);
    layout->addWidget(d->m_buttonMute);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    setLayout(layout);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    connect(d->m_sliderV
            , &StyleSlider::signalValueChanged
            , this
            , [this](const int v)
            {
                Q_EMIT signalSetValue(v);
            });
    connect(d->m_buttonMute
            , &QPushButton::clicked
            , this
            , [this]() { Q_EMIT signalSetMute(); });
}

VolumeController::~VolumeController() = default;

void VolumeController::setLabelVolume(const unsigned v)
{
    d->m_labelVolume->setText(QString("%1%").arg(v));
}

void VolumeController::setSliderVolumeValue(const int v)
{
    d->m_sliderV->setValue(v);
    setLabelVolume(v);
}

void VolumeController::setVolumeButtonIcon(const bool isMuted)
{
    if (isMuted)
    {
        d->m_buttonMute->setIcon(QIcon(Res::MuteBtnSVG));
    }
    else
    {
        d->m_buttonMute->setIcon(QIcon(Res::VolumeBtnSVG));
    }
}

QSize VolumeController::sizeHint() const
{
    return {30, 120};
}

}
