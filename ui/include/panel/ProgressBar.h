//
// Created by cww on 25-4-1.
//

#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H
#include <QWidget>

class QSlider;
class QLabel;

namespace UI::Panel {
    class ProgressBar final : public QWidget {
        Q_OBJECT

    public:
        explicit ProgressBar(QWidget *parent = nullptr);

        QSlider *m_sliderP;
        QLabel *m_labelLeft;
        QLabel *m_labelRight;
        bool m_isUpdatingSlider;

    public Q_SLOTS:
        void updateLabelR(int seconds) const;

        void updateSliderPosition(qint64 position);

        void updateLabelL(qint64 duration) const;
    private:
        static QString convertSecondsToTime(const int seconds);
    };
}
#endif //PROGRESSBAR_H
