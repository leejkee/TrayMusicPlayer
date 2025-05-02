//
// Created by cww on 25-2-13.
//
#pragma once
#include <QMainWindow>

class QSystemTrayIcon;

namespace UI::MainWindow {
    class MainWindow final : public QMainWindow {
        Q_OBJECT

    public:
        static constexpr int MAIN_MINIMUM_WIDTH = 600;
        static constexpr int MAIN_MINIMUM_HEIGHT = 450;

        explicit MainWindow(QWidget *parent = nullptr);

        ~MainWindow() override;

    protected:
        void closeEvent(QCloseEvent *event) override;

        void setVisible(bool visible) override;

    private:
        void initMainApplication();

        void createConnections();

        void createTrayIcon();

        QAction *m_minimizeAction;
        QAction *m_maximizeAction;
        QAction *m_restoreAction;
        QAction *m_quitAction;
        QSystemTrayIcon *m_systemTrayIcon;
        QMenu *m_trayIconMenu;

        QWidget *m_windowManager;
    };
}
