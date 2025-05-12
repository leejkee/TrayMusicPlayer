//
// Created by cww on 25-4-1.
//

#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H
#include <QWidget>

class QSlider;
class QLabel;


namespace Tray::Ui::Panel {
    constexpr int PROGRESS_BAR_WIDTH = 180;

    class ProgressBar final : public QWidget {
        Q_OBJECT

    public:
        explicit ProgressBar(QWidget *parent = nullptr);

    Q_SIGNALS:
        void signalProgressValueChanged(int value);

    public Q_SLOTS:
        void updateLabelR(int seconds) const;

        /// \brief Updates the position of the progress slider and the timestamp label.
        ///
        /// This function is called when the playback position changes.
        /// It performs two main actions:
        /// 1. Updates the position of the progress slider to match the current playback time.
        /// 2. Updates the timestamp label (typically shown to the left of the slider) to display the current time.
        ///
        /// \param position The current playback position in milliseconds.
        void updateSliderPosition(qint64 position);

    private:
        QSlider *m_sliderP;
        QLabel *m_labelLeft;
        QLabel *m_labelRight;
        bool m_isUpdatingSlider;

        static QString convertSecondsToTime(int seconds);
    };
}
#endif //PROGRESSBAR_H
