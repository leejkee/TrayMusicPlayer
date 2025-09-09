//
// Created by cww on 25-4-26.
//
#pragma once
#include <QLabel>
#include <QTimer>
#include <memory>

namespace Tray::Ui::Panel
{
class MarqueeLabelPrivate;
class MarqueeLabel final : public QLabel
{
    Q_OBJECT

public:
    explicit MarqueeLabel(QWidget* parent = nullptr);

    void setMarqueeText(const QString& text);

protected:
    void paintEvent(QPaintEvent* event) override;

    void resizeEvent(QResizeEvent* event) override;

private slots:
    void updatePosition();

private:
    std::unique_ptr<MarqueeLabelPrivate> d;
    void updateTextWidth();
};
}
