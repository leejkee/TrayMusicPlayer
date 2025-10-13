//
// Created by cww on 25-4-10.
//
#pragma once
#include <stylebutton/stylebutton.h>
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>

namespace Tray::Ui
{
class TopBarWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit TopBarWidget(QWidget* parent = nullptr);

    ~TopBarWidget() override;

    void setRadius(int radius);

protected:
    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

Q_SIGNALS:
    void signalPreButtonClicked();

    void signalSettingsButtonClicked();

    void signalExitButtonClicked();

private Q_SLOTS:
    void handleWindowMinimize() const;

    void handleWindowMaximize() const;

private:
    static constexpr int BarHeight = 35;
    static constexpr int ButtonWidth = 35;

    QLabel* m_logoLabel;
    Panel::StyleButton* m_preButton;
    QFrame* m_splitFrame;
    Panel::StyleButton* m_settingsButton;
    Panel::StyleButton* m_minimizeButton;
    Panel::StyleButton* m_maximizeButton;
    Panel::StyleButton* m_exitButton;

    bool m_isDragging {false};

    QPoint m_dragPos;
};
}
