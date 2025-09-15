//
// Created by cww on 25-4-26.
//
#include <marqueelabel/marqueelabel.h>
#include <QPainter>
#include <QTextDocument>

namespace Tray::Ui::Panel
{
class MarqueeLabelPrivate
{
public:
    QTimer m_timer;
    static constexpr int SPEED = 1;
    static constexpr int INTERVAL = 30;
    static constexpr int SPACING = 30;
    qreal m_offset = 0;
    qreal m_textWidth = 0;
    QTextDocument* m_document{};
};

MarqueeLabel::MarqueeLabel(QWidget* parent)
    : QLabel(parent),
      d(std::make_unique<MarqueeLabelPrivate>())
{
    connect(&d->m_timer, &QTimer::timeout, this, &MarqueeLabel::updatePosition);
    d->m_timer.start(MarqueeLabelPrivate::INTERVAL);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    d->m_document = new QTextDocument(this);
}

MarqueeLabel::~MarqueeLabel() = default;

void MarqueeLabel::setMarqueeText(const QString& text)
{
    this->setText(text);
    d->m_document->setHtml(text);
    updateTextWidth();
}

void MarqueeLabel::updateTextWidth()
{
    // d->m_textWidth = fontMetrics().horizontalAdvance(text());
    d->m_textWidth = d->m_document->size().width();
    update();
}

void MarqueeLabel::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(palette().windowText().color());
    painter.setFont(font());

    if (d->m_textWidth > width())
    {
        qreal x = -d->m_offset;
        while (x < width())
        {
            painter.save();
            painter.translate(x, 0);
            d->m_document->drawContents(&painter);
            painter.restore();
            // painter.drawText(x, y, text());
            x += d->m_textWidth + MarqueeLabelPrivate::SPACING;
        }
    }
    else
    {
        painter.save();
        painter.translate(0, 0);
        d->m_document->drawContents(&painter);
        painter.restore();
        // painter.drawText(0, y, text());
    }
}

void MarqueeLabel::resizeEvent(QResizeEvent* event)
{
    QLabel::resizeEvent(event);
    updateTextWidth();
}

void MarqueeLabel::updatePosition()
{
    if (d->m_textWidth <= width())
    {
        d->m_offset = 0;
        return;
    }

    d->m_offset += MarqueeLabelPrivate::SPEED;
    if (d->m_offset > d->m_textWidth + MarqueeLabelPrivate::SPACING)
    {
        d->m_offset = 0;
    }
    update();
}
}
