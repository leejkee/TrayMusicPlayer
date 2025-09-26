//
// Created by 31305 on 2025/9/26.
//
#include <lyricwidget/lyricview.h>
#include <QTimer>

namespace Tray::Ui
{
LyricView::LyricView(QWidget* parent)
    : QListView(parent)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(INTERVAL);
    connect(m_timer, &QTimer::timeout, this, &LyricView::handleNoUserAction);
}

LyricView::~LyricView() = default;

void LyricView::mousePressEvent(QMouseEvent* event)
{
    m_autoScrollActive = false;
    m_timer->stop();
    QListView::mousePressEvent(event);
}

void LyricView::mouseReleaseEvent(QMouseEvent* event)
{
    QListView::mouseReleaseEvent(event);
    m_timer->start();
}

void LyricView::wheelEvent(QWheelEvent* event)
{
    m_autoScrollActive = false;
    m_timer->stop();
    QListView::wheelEvent(event);
    m_timer->start();
}

void LyricView::handleNoUserAction()
{
    m_autoScrollActive = true;
    m_timer->stop();
}

bool LyricView::isActiveAutoScroll() const
{
    return m_autoScrollActive;
}
}
