//
// Created by cww on 25-2-13.
//
#include "trayapp.h"
#include <coreservice/coreservice.h>
#include <windowmanager/windowmanager.h>
#include <traysvg.h>
#include <trayqss.h>
#include <QApplication>
#include <QCloseEvent>
#include <QMenu>
#include <QMessageBox>
#include <QSystemTrayIcon>

inline void Init_qrc()
{
    Q_INIT_RESOURCE(svg);
    Q_INIT_RESOURCE(qss);
}

namespace Tray
{
class TrayAppPrivate
{
public:
    static constexpr int MAIN_MINIMUM_WIDTH = 600;
    static constexpr int MAIN_MINIMUM_HEIGHT = 450;
    static inline const auto WINDOW_TITLE = QStringLiteral("Tray Music");
    QAction* m_minimizeAction;
    QAction* m_maximizeAction;
    QAction* m_restoreAction;
    QAction* m_quitAction;
    QSystemTrayIcon* m_systemTrayIcon;
    QMenu* m_trayIconMenu;
    Ui::WindowManager* m_windowManager;
    Core::CoreService* m_core;
    QPixmap m_pixmapBackGround;
};


TrayApp::TrayApp(QWidget* parent)
    : QMainWindow(parent),
      d(std::make_unique<TrayAppPrivate>())
{
    Init_qrc();
    d->m_trayIconMenu = new QMenu(this);
    d->m_systemTrayIcon = new QSystemTrayIcon(this);
    d->m_minimizeAction = new
            QAction(QCoreApplication::translate("TrayUI", "Minimize"), this);
    d->m_maximizeAction = new
            QAction(QCoreApplication::translate("TrayUI", "Maximize"), this);
    d->m_restoreAction = new
            QAction(QCoreApplication::translate("TrayUI", "Restore"), this);
    d->m_quitAction = new QAction(QCoreApplication::translate("TrayUI", "Quit")
                                  , this);
    d->m_trayIconMenu->addAction(d->m_minimizeAction);
    d->m_trayIconMenu->addAction(d->m_maximizeAction);
    d->m_trayIconMenu->addAction(d->m_restoreAction);
    d->m_trayIconMenu->addSeparator();
    d->m_trayIconMenu->addAction(d->m_quitAction);
    d->m_systemTrayIcon->setContextMenu(d->m_trayIconMenu);
    const auto icon = QIcon(Res::TrayIconSVG);
    d->m_systemTrayIcon->setIcon(icon);
    setWindowIcon(icon);
    d->m_systemTrayIcon->setToolTip(TrayAppPrivate::WINDOW_TITLE);
    d->m_systemTrayIcon->show();

    d->m_core = new Core::CoreService(this);
    const QString preloadKey = d->m_core->getPreloadKey();
    const Ui::WindowManager::WindowInitData initData{
        preloadKey
        , d->m_core->getLocalMusicPaths()
        , d->m_core->getUserListKeys()
        , d->m_core->getTitleList(preloadKey)
        , d->m_core->getDefaultVolume()
    };
    d->m_windowManager = new Ui::WindowManager(initData, this);

    setCentralWidget(d->m_windowManager);
    setMinimumWidth(TrayAppPrivate::MAIN_MINIMUM_WIDTH);
    setMinimumHeight(TrayAppPrivate::MAIN_MINIMUM_HEIGHT);
    setWindowTitle(TrayAppPrivate::WINDOW_TITLE);

    initConnections();
    connectCoreWindow();
}

TrayApp::~TrayApp() = default;

void TrayApp::initConnections()
{
    // quit the application
    connect(d->m_quitAction, &QAction::triggered, qApp, &QApplication::quit);

    // resize the main window
    connect(d->m_maximizeAction
            , &QAction::triggered
            , this
            , &TrayApp::showMaximized);
    connect(d->m_minimizeAction, &QAction::triggered, this, &TrayApp::hide);
    connect(d->m_restoreAction
            , &QAction::triggered
            , this
            , &TrayApp::showNormal);
}

void TrayApp::connectCoreWindow()
{
}

void TrayApp::closeEvent(QCloseEvent* event)
{
    if (!event->spontaneous() || !isVisible())
        return;
    if (d->m_systemTrayIcon->isVisible())
    {
        QMessageBox::information(this
                                 , tr("Exit the application")
                                 , tr("The program will keep running in the "
                                      "system tray. To terminate the program, "
                                      "choose <b>Quit</b> in the context menu "
                                      "of the system tray entry."));
        this->hide();
        event->ignore();
    }
}

void TrayApp::setVisible(const bool visible)
{
    d->m_restoreAction->setEnabled(isMaximized() || !visible);
    d->m_minimizeAction->setEnabled(visible);
    d->m_maximizeAction->setEnabled(!isMaximized());
    QMainWindow::setVisible(visible);
}
}
