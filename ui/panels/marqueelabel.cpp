//
// Created by cww on 25-4-26.
//
#include "marqueelabel.h"
#include <QPainter>
#include <QTextDocument>

namespace Tray::Ui::Panel {
    MarqueeLabel::MarqueeLabel(QWidget *parent)
        : QLabel(parent) {
        connect(&m_timer, &QTimer::timeout, this, &MarqueeLabel::updatePosition);
        m_timer.start(m_interval);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        m_document = new QTextDocument(this);
    }


    void MarqueeLabel::setMarqueeText(const QString &text) {

        this->setText(text);
        m_document->setHtml(text);
        updateTextWidth();
    }

    void MarqueeLabel::updateTextWidth() {
        // m_textWidth = fontMetrics().horizontalAdvance(text());
        m_textWidth = m_document->size().width();
        update();
    }

    void MarqueeLabel::paintEvent(QPaintEvent *event) {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setPen(palette().windowText().color());
        painter.setFont(font());

        if (m_textWidth > width()) {
            int x = -m_offset;
            while (x < width()) {
                painter.save();
                painter.translate(x, 0);
                m_document->drawContents(&painter);
                painter.restore();
                // painter.drawText(x, y, text());
                x += m_textWidth + m_spacing;
            }
        } else {
            painter.save();
            painter.translate(0, 0);
            m_document->drawContents(&painter);
            painter.restore();
            // painter.drawText(0, y, text());
        }
    }

    void MarqueeLabel::resizeEvent(QResizeEvent *event) {
        QLabel::resizeEvent(event);
        updateTextWidth();
    }

    void MarqueeLabel::updatePosition() {
        if (m_textWidth <= width()) {
            m_offset = 0;
            return;
        }

        m_offset += m_speed;
        if (m_offset > m_textWidth + m_spacing) {
            m_offset = 0;
        }
        update();
    }
}
