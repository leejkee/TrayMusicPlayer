//
// Created by cww on 25-4-26.
//
#pragma once

#include <QLabel>
#include <QTimer>


namespace UI::Panel {
    class MarqueeLabel final : public QLabel {
        Q_OBJECT

    public:
        explicit MarqueeLabel(QWidget *parent = nullptr);

        void setMarqueeText(const QString &text); // 重写 setText

    protected:
        void paintEvent(QPaintEvent *event) override;

        void resizeEvent(QResizeEvent *event) override;

    private slots:
        void updatePosition();

    private:
        void updateTextWidth();

        QTimer m_timer;
        const int m_speed = 1;
        const int m_interval = 30; // update every 30ms
        const int m_spacing = 30;
        int m_offset = 0;    // 当前绘制偏移
        int m_textWidth = 0; // 当前文字宽度
        QTextDocument *m_document;
    };
}
