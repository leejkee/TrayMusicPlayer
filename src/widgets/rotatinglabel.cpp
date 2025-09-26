//
// Created by cww on 25-4-9.
//

#include <QMouseEvent>
#include <rotatinglabel/rotatinglabel.h>
#include <QPainter>
#include <QPropertyAnimation>

namespace Tray::Ui::Panel
{
class RotatingLabelPrivate
{
public:
    QPixmap m_pixmap{};
    qreal m_rotation = 0.0;
    QPropertyAnimation* m_rotationAnimation;
};

RotatingLabel::RotatingLabel(const QPixmap& logo
                             , const QSize& targetSize
                             , const RotatingLabelMode mode
                             , QWidget* parent)
    : QWidget(parent), d(std::make_unique<RotatingLabelPrivate>())
{
    d->m_rotationAnimation = new QPropertyAnimation(this, "rotation", this);
    setPixmapWithSmoothScale(logo, targetSize);
    setAttribute(Qt::WA_TranslucentBackground);
    setMinimumSize(50, 50);
    setLabelMode(mode);
}

RotatingLabel::~RotatingLabel() = default;

RotatingLabel::RotatingLabel(QWidget* parent)
    : QWidget(parent)
{
    d->m_rotationAnimation = new QPropertyAnimation(this, "rotation", this);
    setAttribute(Qt::WA_TranslucentBackground);
}


void RotatingLabel::initRotation(const QVariant& startV
                                 , const QVariant& endV
                                 , const int msecs
                                 , const int loopCount)
{
    d->m_rotationAnimation->setStartValue(startV);
    d->m_rotationAnimation->setEndValue(endV);
    d->m_rotationAnimation->setDuration(msecs);
    d->m_rotationAnimation->setLoopCount(loopCount);
}

void RotatingLabel::setLabelMode(const RotatingLabelMode mode)
{
    switch (mode)
    {
    case Rotating:
        d->m_rotationAnimation->start();
        break;
    case NoRotating:
        d->m_rotationAnimation->stop();
        break;
    default:
        break;
    }
}

void RotatingLabel::setPixmap(const QPixmap& pixmap)
{
    d->m_pixmap = pixmap;
    update();
}

void RotatingLabel::setPixmapWithSmoothScale(const QPixmap& pixmap
                                             , const QSize& targetSize)
{
    d->m_pixmap = pixmap.scaled(targetSize
                                , Qt::IgnoreAspectRatio
                                , Qt::SmoothTransformation);
    update();
}

qreal RotatingLabel::rotation() const
{
    return d->m_rotation;
}

void RotatingLabel::setRotation(const qreal angle)
{
    if (!qFuzzyCompare(d->m_rotation, angle))
    {
        d->m_rotation = angle;
        update();
        emit rotationChanged(angle);
    }
}

void RotatingLabel::paintEvent(QPaintEvent*)
{
    if (d->m_pixmap.isNull())
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const QSize pixSize = d->m_pixmap.size();
    const QPoint center = rect().center();

    painter.translate(center);
    painter.rotate(d->m_rotation);
    painter.translate(-static_cast<qreal>(pixSize.width()) / 2
                      , -static_cast<qreal>(pixSize.height()) / 2);
    painter.drawPixmap(0, 0, d->m_pixmap);
}

QSize RotatingLabel::sizeHint() const
{
    // return m_pixmap.size().isEmpty() ? QSize(100, 100) : m_pixmap.size();
    return {50, 50};
}

void RotatingLabel::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        Q_EMIT signalClicked();
    }
    QWidget::mousePressEvent(event);
}
}
