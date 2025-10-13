//
// Created by 31305 on 2025/10/10.
//
#pragma once
#include <topbarwidget/topbarwidget.h>
#include <coreservice/coreservice.h>
#include <windowmanager/windowmanager.h>
#include <QSystemTrayIcon>
#include <QMenu>

namespace Tray
{
class StyleWindow final : public QWidget
{
    Q_OBJECT
public:
    explicit StyleWindow(const QString& settingsPath, QWidget *parent = nullptr);

protected:
    void setVisible(bool visible) override;
    void closeEvent(QCloseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    Ui::TopBarWidget *m_topBarWidget;
    Ui::WindowManager *m_windowManager;
    Core::CoreService *m_coreService;
    QVBoxLayout *m_layout;

    QAction* m_minimizeAction;
    QAction* m_maximizeAction;
    QAction* m_restoreAction;
    QAction* m_quitAction;
    QSystemTrayIcon* m_systemTrayIcon;
    QMenu* m_trayIconMenu;
    QPixmap m_pixmapBackGround;

    static constexpr int MAIN_MINIMUM_WIDTH = 600;
    static constexpr int MAIN_MINIMUM_HEIGHT = 450;
    static inline const auto WINDOW_TITLE = QStringLiteral("Tray Music");

    void initAppIcons();
    void connectCoreWindow();
};


}
