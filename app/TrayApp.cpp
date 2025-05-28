//
// Created by cww on 25-2-13.
//

#include "TrayApp.h"
#include "WindowManager.h"
#include <core.h>
#include <TraySVG.h>
#include <QApplication>
#include <QCloseEvent>
#include <QMenu>
#include <QMessageBox>
#include <QSystemTrayIcon>

inline void Init_qrc() {
    Q_INIT_RESOURCE(svg);
    Q_INIT_RESOURCE(qss);
}

namespace Tray {
    class TrayAppPrivate {
    public:
        explicit TrayAppPrivate(TrayApp *w);

        static constexpr int MAIN_MINIMUM_WIDTH = 600;
        static constexpr int MAIN_MINIMUM_HEIGHT = 450;
        QAction *m_minimizeAction;
        QAction *m_maximizeAction;
        QAction *m_restoreAction;
        QAction *m_quitAction;
        QSystemTrayIcon *m_systemTrayIcon;
        QMenu *m_trayIconMenu;
        Ui::WindowManager *m_windowManager;
        Core::Core *m_core;
        QPixmap *m_pixmapBackGround;
        TrayApp *q_ptr;
    };

    TrayAppPrivate::TrayAppPrivate(TrayApp *w): q_ptr(w) {
        m_trayIconMenu = new QMenu(q_ptr);
        m_systemTrayIcon = new QSystemTrayIcon(q_ptr);
        m_minimizeAction = new QAction(QCoreApplication::translate("TrayUI", "Minimize"), q_ptr);
        m_maximizeAction = new QAction(QCoreApplication::translate("TrayUI", "Maximize"), q_ptr);
        m_restoreAction = new QAction(QCoreApplication::translate("TrayUI", "Restore"), q_ptr);
        m_quitAction = new QAction(QCoreApplication::translate("TrayUI", "Quit"), q_ptr);
        m_trayIconMenu->addAction(m_minimizeAction);
        m_trayIconMenu->addAction(m_maximizeAction);
        m_trayIconMenu->addAction(m_restoreAction);
        m_trayIconMenu->addSeparator();
        m_trayIconMenu->addAction(m_quitAction);
        m_systemTrayIcon->setContextMenu(m_trayIconMenu);
        const auto icon = QIcon(Res::TrayIconSVG);
        m_systemTrayIcon->setIcon(icon);
        q_ptr->setWindowIcon(icon);
        m_systemTrayIcon->setToolTip("Tray Music");
        m_systemTrayIcon->show();

        m_core = new Core::Core(q_ptr);
        m_windowManager = new Ui::WindowManager(m_core, q_ptr);
        m_core->initWork();
        q_ptr->setCentralWidget(m_windowManager);
        q_ptr->setMinimumWidth(MAIN_MINIMUM_WIDTH);
        q_ptr->setMinimumHeight(MAIN_MINIMUM_HEIGHT);

        m_pixmapBackGround = new QPixmap();
    }

    TrayApp::TrayApp(QWidget *parent)
        : QMainWindow(parent) {
        Init_qrc();
        d = std::make_unique<TrayAppPrivate>(this);
        createConnections();
    }

    TrayApp::~TrayApp() = default;

    void TrayApp::createConnections() {
        // quit the application
        connect(d->m_quitAction, &QAction::triggered, qApp, &QApplication::quit);

        // resize the main window
        connect(d->m_maximizeAction, &QAction::triggered, this, &TrayApp::showMaximized);
        connect(d->m_minimizeAction, &QAction::triggered, this, &TrayApp::hide);
        connect(d->m_restoreAction, &QAction::triggered, this, &TrayApp::showNormal);
    }

    void TrayApp::closeEvent(QCloseEvent *event) {
        if (!event->spontaneous() || !isVisible())
            return;
        if (d->m_systemTrayIcon->isVisible()) {
            QMessageBox::information(this, tr("Exit the application"),
                                     tr("The program will keep running in the "
                                         "system tray. To terminate the program, "
                                         "choose <b>Quit</b> in the context menu "
                                         "of the system tray entry."));
            this->hide();
            event->ignore();
        }
    }

    void TrayApp::setVisible(const bool visible) {
        d->m_restoreAction->setEnabled(isMaximized() || !visible);
        d->m_minimizeAction->setEnabled(visible);
        d->m_maximizeAction->setEnabled(!isMaximized());
        QMainWindow::setVisible(visible);
    }

}
