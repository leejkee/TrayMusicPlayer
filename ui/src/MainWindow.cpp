//
// Created by cww on 25-2-13.
//

#include <ui/MainWindow.h>
#include <ui/Assets.h>
#include <ui/WindowManager.h>

#include <QApplication>
#include <QCloseEvent>
#include <QMenu>
#include <QMessageBox>
#include <QSystemTrayIcon>

inline void InitMyQRC() {
    Q_INIT_RESOURCE(svg);
    Q_INIT_RESOURCE(qss);
}

namespace UI::MainWindow {
    MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent),
          m_minimizeAction(nullptr),
          m_maximizeAction(nullptr),
          m_restoreAction(nullptr),
          m_quitAction(nullptr),
          m_systemTrayIcon(nullptr),
          m_trayIconMenu(nullptr),
        m_windowManager(nullptr){
        InitMyQRC();
        initMainApplication();
    }

    MainWindow::~MainWindow() = default;

    void MainWindow::initMainApplication() {
        createTrayIcon();
        m_windowManager = new WindowManager::WindowManager(this);
        setCentralWidget(m_windowManager);
        createConnections();
        resize(600, 400);
    }

    void MainWindow::createConnections() {
        // quit the application
        connect(m_quitAction, &QAction::triggered, qApp, &QApplication::quit);

        // resize the main window
        connect(m_maximizeAction, &QAction::triggered, this, &MainWindow::showMaximized);
        connect(m_minimizeAction, &QAction::triggered, this, &MainWindow::hide);
        connect(m_restoreAction, &QAction::triggered, this, &MainWindow::showNormal);
    }

    void MainWindow::closeEvent(QCloseEvent *event) {
        if (!event->spontaneous() || !isVisible())
            return;
        if (m_systemTrayIcon->isVisible()) {
            QMessageBox::information(this, tr("Tray"),
                                     tr("The program will keep running in the "
                                         "system tray. To terminate the program, "
                                         "choose <b>Quit</b> in the context menu "
                                         "of the system tray entry."));
            this->hide();
            event->ignore();
        }
    }

    void MainWindow::setVisible(const bool visible) {
        m_restoreAction->setEnabled(isMaximized() || !visible);
        m_minimizeAction->setEnabled(visible);
        m_maximizeAction->setEnabled(!isMaximized());
        QMainWindow::setVisible(visible);
    }


    void MainWindow::createTrayIcon() {
        m_trayIconMenu = new QMenu(this);
        m_systemTrayIcon = new QSystemTrayIcon(this);
        m_minimizeAction = new QAction(QCoreApplication::translate("TrayUI", "Minimize"), this);
        m_maximizeAction = new QAction(QCoreApplication::translate("TrayUI", "Maximize"), this);
        m_restoreAction = new QAction(QCoreApplication::translate("TrayUI", "Restore"), this);
        m_quitAction = new QAction(QCoreApplication::translate("TrayUI", "Quit"), this);
        m_trayIconMenu->addAction(m_minimizeAction);
        m_trayIconMenu->addAction(m_maximizeAction);
        m_trayIconMenu->addAction(m_restoreAction);
        m_trayIconMenu->addSeparator();
        m_trayIconMenu->addAction(m_quitAction);
        m_systemTrayIcon->setContextMenu(m_trayIconMenu);
        const auto icon = QIcon(SvgRes::TrayIconSVG);
        m_systemTrayIcon->setIcon(icon);
        this->setWindowIcon(icon);
        m_systemTrayIcon->setToolTip("Tray Music");
        m_systemTrayIcon->show();
    }
}
