//
// Created by cww on 25-4-9.
//

#include "rotatinglabel.h"
#include <QPainter>


namespace Tray::Ui::Panel {
    RotatingLabel::RotatingLabel(const QPixmap &logo, const QSize &targetSize, const RotatingLabelMode mode,
                                 QWidget *parent)
        : QWidget(parent) {
        m_rotationAnimation = new QPropertyAnimation(this, "rotation", this);
        setPixmapWithSmoothScale(logo, targetSize);
        setAttribute(Qt::WA_TranslucentBackground);
        setMinimumSize(50, 50);
    }

    RotatingLabel::RotatingLabel(QWidget *parent) : QWidget(parent) {
        m_rotationAnimation = new QPropertyAnimation(this, "rotation", this);
        setAttribute(Qt::WA_TranslucentBackground);
    }


    void RotatingLabel::initRotation(const QVariant &startV, const QVariant &endV, const int msecs,
                                     const int loopCount) {
        m_rotationAnimation->setStartValue(startV);
        m_rotationAnimation->setEndValue(endV);
        m_rotationAnimation->setDuration(msecs);
        m_rotationAnimation->setLoopCount(loopCount);
    }

    void RotatingLabel::setLabelMode(const RotatingLabelMode mode) {
        switch (mode) {
            case Rotating:
                m_rotationAnimation->start();
                break;
            case NoRotating:
                m_rotationAnimation->stop();
                break;
            default:
                break;
        }
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

    void RotatingLabel::setRotation(const qreal angle) {
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
        painter.translate(-static_cast<qreal>(pixSize.width()) / 2, -static_cast<qreal>(pixSize.height()) / 2);
        painter.drawPixmap(0, 0, m_pixmap);
    }

    QSize RotatingLabel::sizeHint() const {
        // return m_pixmap.size().isEmpty() ? QSize(100, 100) : m_pixmap.size();
        return QSize(50, 50);
    }
}
