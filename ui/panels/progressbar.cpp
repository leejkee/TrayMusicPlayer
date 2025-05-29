//
// Created by cww on 25-4-1.
//
#include "progressbar.h"
#include <qslider.h>
#include <trayqss.h>
#include <uiconfig.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>

namespace Tray::Ui::Panel {
ProgressBar::ProgressBar(QWidget* parent) :
    QWidget(parent), m_sliderP(new QSlider(Qt::Horizontal, this)),
    m_labelLeft(new QLabel(this)), m_labelRight(new QLabel(this)) {
    const auto layout = new QHBoxLayout;
    layout->setSpacing(2);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_labelLeft);
    layout->addWidget(m_sliderP);
    layout->addWidget(m_labelRight);
    setLayout(layout);

    m_labelLeft->setText("00:00");
    m_labelRight->setText("00:00");
    m_sliderP->setStyleSheet(readQSS(Res::PROGRESS_BAR_QSS));
    setFixedWidth(PROGRESS_BAR_WIDTH);

    connect(m_sliderP, &QSlider::sliderMoved, this, [this](const int value) {
        Q_EMIT signalProgressValueChanged(value);
    });
}

void ProgressBar::updateSliderPosition(const qint64 position) {
    m_sliderP->setValue(static_cast<int>(position));
    const qint64 s = position / 1000;
    const QString t = convertSecondsToTime(static_cast<int>(s));
    m_labelLeft->setText(t);
}

// update when music changed
void ProgressBar::updateLabelR(const int seconds) const {
    m_sliderP->setMaximum(seconds * 1000);
    m_labelRight->setText(convertSecondsToTime(seconds));
}

QString ProgressBar::convertSecondsToTime(const int seconds) {
    const int m = seconds / 60;
    const int s = seconds % 60;
    return QString::asprintf("%02d:%02d", m, s);
}
} // namespace Tray::Ui::Panel
