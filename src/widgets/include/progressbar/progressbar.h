//
// Created by cww on 25-4-1.
//

#pragma once
#include <QWidget>

namespace Tray::Ui::Panel
{
class ProgressBarPrivate;

class ProgressBar final : public QWidget
{
    Q_OBJECT

public:
    explicit ProgressBar(QWidget* parent = nullptr);

    ~ProgressBar();

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
    std::unique_ptr<ProgressBarPrivate> d;

    static QString convertSecondsToTime(int seconds);
};
}
