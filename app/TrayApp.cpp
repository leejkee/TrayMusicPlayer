//
// Created by cww on 25-2-13.
//

#include "TrayApp.h"
#include <TraySVG.h>
#include <WindowManager.h>
#include <Core.h>
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

        m_windowManager = new Ui::WindowManager(q_ptr);
        m_core = new Core::Core(q_ptr);
        q_ptr->setCentralWidget(m_windowManager);
        m_windowManager->initDefaultSettings(m_core->getLocalMusicPaths(), m_core->getKeysOfUserPlaylist());
        q_ptr->setMinimumWidth(MAIN_MINIMUM_WIDTH);
        q_ptr->setMinimumHeight(MAIN_MINIMUM_HEIGHT);
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

        // core and ui
        // playToggle
        connect(d->m_windowManager, &Ui::WindowManager::signalPlayToggle,
                d->m_core, &Core::Core::playToggle);

        // core: playlist --> ui: (viewWidget, playerWidget)
        connect(d->m_core, &Core::Core::signalNotifyUiCurrentMusicChanged,
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

        // progressbar
        connect(d->m_windowManager, &Ui::WindowManager::signalSetPlayerPosition,
                d->m_core, &Core::Core::setPlayerPosition);

        connect(d->m_core, &Core::Core::signalPlayModeChanged,
                d->m_windowManager, &Ui::WindowManager::updatePlayModeIcon);

        connect(d->m_windowManager, &Ui::WindowManager::signalPlayModeChanged,
                d->m_core, &Core::Core::changePlayMode);



        /// ViewWidget section
        connect(d->m_windowManager, &Ui::WindowManager::signalViewPlayButtonClicked,
                d->m_core, &Core::Core::playToggleWithListAndIndex);

        connect(d->m_windowManager, &Ui::WindowManager::signalMusicRemovedFromList,
                d->m_core, &Core::Core::removeMusicFromList);

        connect(d->m_windowManager, &Ui::WindowManager::signalMusicAddedToList,
                d->m_core, &Core::Core::addMusicToList);
        /// ViewWidget section

        /// User playlist add/remove section
        // ui: musicListWidget --> core: listCache -> db -> settings
        connect(d->m_windowManager, &Ui::WindowManager::signalUserPlaylistButtonAdded,
                d->m_core, &Core::Core::addUserPlaylist);
        connect(d->m_windowManager, &Ui::WindowManager::signalUserPlaylistButtonRemoved,
                d->m_core, &Core::Core::removeUserPlaylist);
        // core: listCache --> ui: musicListWidget
        connect(d->m_core, &Core::Core::signalNotifyUiToRemoveUserPlaylist,
                d->m_windowManager, &Ui::WindowManager::removeUserPlaylistButton);
        connect(d->m_core, &Core::Core::signalNotifyUiToAddUserPlaylist,
                d->m_windowManager, &Ui::WindowManager::addUserPlaylistButton);
        /// User playlist add/remove section


        /// Switch playlist to view section
        // ui: musicListWidget --> core: listCache
        connect(d->m_windowManager, &Ui::WindowManager::signalPlaylistButtonClicked,
                d->m_core, &Core::Core::requestPlaylist);
        // core: listCache --> ui: viewWidget
        connect(d->m_core, &Core::Core::signalSendUiCurrentTitleList,
                d->m_windowManager, &Ui::WindowManager::showCurrentTitleListToView);
        /// Switch playlist to view section


        /// Local music paths section
        // ui: settingsWidget --> core：settings --> listCache
        connect(d->m_windowManager, &Ui::WindowManager::signalLocalMusicDirectoryAdded,
                d->m_core, &Core::Core::appendLocalMusicPath);
        connect(d->m_windowManager, &Ui::WindowManager::signalLocalMusicDirectoryRemoved,
                d->m_core, &Core::Core::removeLocalMusicPath);
        // core: settings --> ui: settingsWidget
        connect(d->m_core, &Core::Core::signalNotifyUiToUpdateLocalPaths,
                d->m_windowManager, &Ui::WindowManager::updateSettingsLocalPaths);
        /// Local music paths section

        connect(d->m_core, &Core::Core::signalUserPlaylistSetsChanged,
                d->m_windowManager, &Ui::WindowManager::updateUserPlaylistKeys);

            // todo
        connect(d->m_core, &Core::Core::signalPlaylistModified
                , d->m_windowManager, &Ui::WindowManager::updateCurrentViewList);

        connect(d->m_core, &Core::Core::signalCurrentPlaylistKeyChanged, d->m_windowManager,
                &Ui::WindowManager::updateCurrentPlaylist);
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
