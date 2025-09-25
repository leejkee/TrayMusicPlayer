//
// Created by 31305 on 2025/9/25.
//
#pragma once
#include <QSlider>
namespace Tray::Ui::Panel
{
class StyleSlider final : public QSlider{
    Q_OBJECT

public:
    explicit StyleSlider(Qt::Orientation orientation, QWidget *parent = nullptr);

Q_SIGNALS:
    void signalValueChanged(int value);

public Q_SLOTS:
    void updateSliderValue(int value);

private:
    bool m_isUserAction = false;
};

}
