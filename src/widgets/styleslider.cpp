//
// Created by 31305 on 2025/9/25.
//
#include <styleslider/styleslider.h>
#include <trayqss.h>

namespace Tray::Ui::Panel
{
StyleSlider::StyleSlider(const Qt::Orientation orientation, QWidget* parent)
    : QSlider(orientation, parent)
{
    setStyleSheet(Res::readQss(Res::PROGRESS_BAR_QSS));
    connect(this
            , &QSlider::valueChanged
            , this
            , [this](const int value)
            {
                if (m_isUserAction)
                {
                    Q_EMIT signalValueChanged(value);
                }
            });

    connect(this
            , &QSlider::sliderPressed
            , this
            , [this]
            {
                m_isUserAction = true;
            });

    connect(this
            , &QSlider::sliderReleased
            , this
            , [this]
            {
                m_isUserAction = false;
                Q_EMIT signalValueChanged(this->value());
            });
}

void StyleSlider::updateSliderValue(const int value)
{
    if (m_isUserAction)
    {
        return;
    }
    setValue(value);
}
}
