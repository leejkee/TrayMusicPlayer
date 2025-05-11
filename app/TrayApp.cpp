//
// Created by cww on 25-2-13.
//

#include "TrayApp.h"
#include <Assets.h>
#include <WindowManager.h>
#include <Core.h>
#include <QApplication>
#include <QCloseEvent>
#include <QMenu>
#include <QMessageBox>
#include <QSystemTrayIcon>

inline void Init_qrc() {
        Q_INIT_RESOURCE(svg);
}

namespace Tray {
    class TrayAppPrivate {
    public:
        QAction *m_minimizeAction;
        QAction *m_maximizeAction;
        QAction *m_restoreAction;
        QAction *m_quitAction;
        QSystemTrayIcon *m_systemTrayIcon;
        QMenu *m_trayIconMenu;

        Ui::WindowManager *m_windowManager;
        Core::Core *m_core;
    };

    TrayApp::TrayApp(QWidget *parent)
        : QMainWindow(parent),
          d(std::make_unique<TrayAppPrivate>()) {
        initMainApplication();
            Init_qrc();
    }

    TrayApp::~TrayApp() {
    }

    void TrayApp::initMainApplication() {
        createTrayIcon();
        d->m_windowManager = new Ui::WindowManager(this);
        d->m_core = new Core::Core(this);
        setCentralWidget(d->m_windowManager);
        createConnections();
            d->m_windowManager->initDefaultSettings(d->m_core->getLocalMusicPaths(), d->m_core->getKeysOfUserPlaylist());
        setMinimumWidth(MAIN_MINIMUM_WIDTH);
        setMinimumHeight(MAIN_MINIMUM_HEIGHT);
    }

    void TrayApp::createConnections() {
        // quit the application
        connect(d->m_quitAction, &QAction::triggered, qApp, &QApplication::quit);

        // resize the main window
        connect(d->m_maximizeAction, &QAction::triggered, this, &TrayApp::showMaximized);
        connect(d->m_minimizeAction, &QAction::triggered, this, &TrayApp::hide);
        connect(d->m_restoreAction, &QAction::triggered, this, &TrayApp::showNormal);

        // core and ui
        // playToggle
        connect(d->m_windowManager, &Ui::WindowManager::signalPlayToggle,
                d->m_core, &Core::Core::playToggle);

        // check music
        connect(d->m_core, &Core::Core::signalCurrentMusicChanged,
                d->m_windowManager, &Ui::WindowManager::updateCurrentMusic);

        // update progress bar pos
        connect(d->m_core, &Core::Core::signalPositionChanged,
                d->m_windowManager, &Ui::WindowManager::updateProgressBarPosition);

        // update playing status
        connect(d->m_core, &Core::Core::signalPlayingStatusChanged,
                d->m_windowManager, &Ui::WindowManager::updatePlayingStatus);

        // pre music
        connect(d->m_windowManager, &Ui::WindowManager::signalPreMusic,
                d->m_core, &Core::Core::preMusic);

        // next music
        connect(d->m_windowManager, &Ui::WindowManager::signalNextMusic,
                d->m_core, &Core::Core::nextMusic);

        // update the icon of volume ctrl button
        connect(d->m_core, &Core::Core::signalIsMuted,
                d->m_windowManager, &Ui::WindowManager::updateVolumeCtrlButtonIcon);

        // change volume
        connect(d->m_windowManager, &Ui::WindowManager::signalSetVolume,
                d->m_core, &Core::Core::setVolume);

        //
        connect(d->m_windowManager, &Ui::WindowManager::signalSetPlayerPosition,
                d->m_core, &Core::Core::setPlayerPosition);

        connect(d->m_core, &Core::Core::signalPlayModeChanged,
                d->m_windowManager, &Ui::WindowManager::updatePlayModeIcon);

        connect(d->m_windowManager, &Ui::WindowManager::signalPlayModeChanged,
                d->m_core, &Core::Core::changePlayMode);

        // change playlist
        connect(d->m_windowManager, &Ui::WindowManager::signalPlaylistChanged,
                d->m_core, &Core::Core::requestPlaylist);

        // switch the current showing list
        connect(d->m_core, &Core::Core::signalMusicListChanged,
                d->m_windowManager, &Ui::WindowManager::updateViewList
        );

        connect(d->m_windowManager, &Ui::WindowManager::signalViewPlayButtonClicked,
                d->m_core, &Core::Core::playToggleWithListAndIndex);

        connect(d->m_windowManager, &Ui::WindowManager::signalPlaylistAdded,
                d->m_core, &Core::Core::newUserList);

        // append local music path
        connect(d->m_windowManager, &Ui::WindowManager::signalLocalMusicDirectoryAdded,
                d->m_core, &Core::Core::appendLocalMusicPath);

        // remove local music path
        connect(d->m_windowManager, &Ui::WindowManager::signalLocalMusicDirectoryRemoved,
                d->m_core, &Core::Core::removeLocalMusicPath);

        connect(d->m_windowManager, &Ui::WindowManager::signalAddSongToList,
                d->m_core, &Core::Core::addMusicToList);
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


    void TrayApp::createTrayIcon() {
        d->m_trayIconMenu = new QMenu(this);
        d->m_systemTrayIcon = new QSystemTrayIcon(this);
        d->m_minimizeAction = new QAction(QCoreApplication::translate("TrayUI", "Minimize"), this);
        d->m_maximizeAction = new QAction(QCoreApplication::translate("TrayUI", "Maximize"), this);
        d->m_restoreAction = new QAction(QCoreApplication::translate("TrayUI", "Restore"), this);
        d->m_quitAction = new QAction(QCoreApplication::translate("TrayUI", "Quit"), this);
        d->m_trayIconMenu->addAction(d->m_minimizeAction);
        d->m_trayIconMenu->addAction(d->m_maximizeAction);
        d->m_trayIconMenu->addAction(d->m_restoreAction);
        d->m_trayIconMenu->addSeparator();
        d->m_trayIconMenu->addAction(d->m_quitAction);
        d->m_systemTrayIcon->setContextMenu(d->m_trayIconMenu);
        const auto icon = QIcon(SvgRes::TrayIconSVG);
        d->m_systemTrayIcon->setIcon(icon);
        this->setWindowIcon(icon);
        d->m_systemTrayIcon->setToolTip("Tray Music");
        d->m_systemTrayIcon->show();
    }
}
