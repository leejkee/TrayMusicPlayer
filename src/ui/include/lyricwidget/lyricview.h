//
// Created by 31305 on 2025/9/26.
//
#pragma once
#include <QListView>

namespace Tray::Ui
{
class LyricView final: public QListView
{
    Q_OBJECT
public:
    explicit LyricView(QWidget* parent = nullptr);
    ~LyricView() override;

    bool isActiveAutoScroll() const;

Q_SIGNALS:
    void signalScrollToCenter();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    QTimer* m_timer;
    bool m_autoScrollActive{true};
    static constexpr int INTERVAL{5000}; // ms
    void handleNoUserAction();
};

}
