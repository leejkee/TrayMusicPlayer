//
// Created by cww on 25-4-1.
//
#include <panel/ProgressBar.h>
#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>
namespace UI::Panel {
    ProgressBar::ProgressBar(QWidget *parent)
    : QWidget(parent)
      , m_sliderP(new QSlider(Qt::Horizontal, this))
      , m_labelLeft(new QLabel(this))
      , m_labelRight(new QLabel(this))
      , m_isUpdatingSlider(false) {

        const auto layout = new QHBoxLayout;
        layout->addWidget(m_labelLeft);
        layout->addWidget(m_sliderP);
        layout->addWidget(m_labelRight);
        setLayout(layout);

        m_labelLeft->setText("00:00");
        m_labelRight->setText("00:00");
    }


    ///
    /// @param position ms
    void ProgressBar::updateSliderPosition(const qint64 position) {
        // secure this function dismiss the signal @valueChanged
        m_isUpdatingSlider = true;
        m_sliderP->setValue(static_cast<int>(position));
        m_isUpdatingSlider = false;
    }

    void ProgressBar::updateLabelL(const qint64 duration) const {
        const int s = duration / 1000;
        const QString t = convertSecondsToTime(s);
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
}
