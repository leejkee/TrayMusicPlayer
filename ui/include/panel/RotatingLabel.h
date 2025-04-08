#pragma once

#include <QWidget>
#include <QPixmap>
#include <QPropertyAnimation>
namespace UI::Panel {

    class RotatingLabel final : public QWidget {
        Q_OBJECT
        Q_PROPERTY(qreal rotation READ rotation WRITE setRotation NOTIFY rotationChanged)

    public:
        explicit RotatingLabel(QWidget *parent = nullptr);

        void setPixmap(const QPixmap &pixmap);
        void setPixmapWithSmoothScale(const QPixmap &pixmap, const QSize &targetSize);

        qreal rotation() const;
        void setRotation(qreal angle);

        signals:
            void rotationChanged(qreal angle);

    protected:
        void paintEvent(QPaintEvent *event) override;
        QSize sizeHint() const override;

    private:
        QPixmap m_pixmap;
        qreal m_rotation = 0.0;
    };


}
