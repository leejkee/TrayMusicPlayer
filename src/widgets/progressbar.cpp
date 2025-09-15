//
// Created by cww on 25-4-1.
//
#include <progressbar/progressbar.h>
#include <trayqss.h>
#include <QSlider>
#include <QHBoxLayout>
#include <QLabel>

namespace Tray::Ui::Panel
{
class ProgressBarPrivate
{
public:
    static constexpr int PROGRESS_BAR_WIDTH = 180;
    QSlider* m_sliderP;
    QLabel* m_labelLeft;
    QLabel* m_labelRight;
};

ProgressBar::~ProgressBar() = default;

ProgressBar::ProgressBar(QWidget* parent)
    : QWidget(parent),
      d(std::make_unique<ProgressBarPrivate>())
{
    d->m_sliderP = new QSlider(Qt::Horizontal, this);
    d->m_labelLeft = new QLabel(this);
    d->m_labelRight = new QLabel(this);
    const auto layout = new QHBoxLayout;
    layout->setSpacing(2);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(d->m_labelLeft);
    layout->addWidget(d->m_sliderP);
    layout->addWidget(d->m_labelRight);
    setLayout(layout);

    d->m_labelLeft->setText("00:00");
    d->m_labelRight->setText("00:00");
    setFixedWidth(ProgressBarPrivate::PROGRESS_BAR_WIDTH);

    connect(d->m_sliderP
            , &QSlider::sliderMoved
            , this
            , [this](const int value)
            {
                Q_EMIT signalProgressValueChanged(value);
            });
}

void ProgressBar::updateSliderPosition(const qint64 position)
{
    d->m_sliderP->setValue(static_cast<int>(position));
    const qint64 s = position / 1000;
    const QString t = convertSecondsToTime(static_cast<int>(s));
    d->m_labelLeft->setText(t);
}

// update when music changed
void ProgressBar::updateLabelR(const int seconds) const
{
    d->m_sliderP->setMaximum(seconds * 1000);
    d->m_labelRight->setText(convertSecondsToTime(seconds));
}

QString ProgressBar::convertSecondsToTime(const int seconds)
{
    const int m = seconds / 60;
    const int s = seconds % 60;
    return QString::asprintf("%02d:%02d", m, s);
}
} // namespace Tray::Ui::Panel
