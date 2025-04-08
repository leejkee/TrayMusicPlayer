//
// Created by cww on 25-4-9.
//

#include <panel/RotatingLabel.h>
#include <QPainter>
namespace UI::Panel{

    RotatingLabel::RotatingLabel(QWidget *parent)
        : QWidget(parent) {
        setAttribute(Qt::WA_TranslucentBackground);
    }

    void RotatingLabel::setPixmap(const QPixmap &pixmap) {
        m_pixmap = pixmap;
        update();
    }

    void RotatingLabel::setPixmapWithSmoothScale(const QPixmap &pixmap, const QSize &targetSize) {
        m_pixmap = pixmap.scaled(targetSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        update();
    }

    qreal RotatingLabel::rotation() const {
        return m_rotation;
    }

    void RotatingLabel::setRotation(qreal angle) {
        if (!qFuzzyCompare(m_rotation, angle)) {
            m_rotation = angle;
            update();
            emit rotationChanged(angle);
        }
    }

    void RotatingLabel::paintEvent(QPaintEvent *) {
        if (m_pixmap.isNull())
            return;

        QPainter painter(this);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
        painter.setRenderHint(QPainter::Antialiasing, true);

        const QSize pixSize = m_pixmap.size();
        const QPoint center = rect().center();

        painter.translate(center);
        painter.rotate(m_rotation);
        painter.translate(-pixSize.width() / 2, -pixSize.height() / 2);
        painter.drawPixmap(0, 0, m_pixmap);
    }

    QSize RotatingLabel::sizeHint() const {
        return m_pixmap.size().isEmpty() ? QSize(100, 100) : m_pixmap.size();
    }



}
