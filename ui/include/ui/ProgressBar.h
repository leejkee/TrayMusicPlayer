//
// Created by cww on 25-4-1.
//

#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H
#include <QWidget>

class QSlider;
class QLabel;
namespace Ui::Panel {

class ProgressBarWidget final : public QWidget {
    Q_OBJECT
public:
    explicit ProgressBarWidget(QWidget *parent = nullptr);

    QSlider *m_sliderP;
    QLabel *m_labelLeft;
    QLabel *m_labelRight;
    bool m_isUpdatingSlider;

    public Q_SLOTS:
        void updateLabelR();

    void updateSliderPosition(qint64 position);

    void updateLabelL(qint64 duration);
};
}
#endif //PROGRESSBAR_H
