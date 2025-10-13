//
// Created by 31305 on 2025/10/10.
//

#include "stylewindow.h"
#include <trayqss.h>
#include <traysvg.h>
#include <QApplication>
#include <QCloseEvent>
#include <QPainter>

inline void Init_qrc()
{
    Q_INIT_RESOURCE(svg);
}

namespace Tray
{
StyleWindow::StyleWindow(const QString& settingsPath, QWidget* parent)
    : QWidget(parent)
{
    setObjectName("StyleWindow");
    Init_qrc();
    initAppIcons();
    m_coreService = new Core::CoreService(settingsPath, this);
    m_topBarWidget = new Ui::TopBarWidget(this);
    m_topBarWidget->setRadius(5);
    const QString preloadKey = m_coreService->getPreloadKey();
    const Ui::WindowManager::WindowInitData initData{
        preloadKey
        , m_coreService->getLocalMusicPaths()
        , m_coreService->getUserListKeys()
        , m_coreService->getTitleList(preloadKey)
        , m_coreService->getDefaultVolume()
    };
    m_windowManager = new Ui::WindowManager(initData, this);
    m_layout = new QVBoxLayout(this);
    m_layout->setSpacing(0);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->addWidget(m_topBarWidget);
    m_layout->addWidget(m_windowManager);
    setMinimumWidth(MAIN_MINIMUM_WIDTH);
    setMinimumHeight(MAIN_MINIMUM_HEIGHT);
    connectCoreWindow();
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, this);
    connect(m_topBarWidget
            , &Ui::TopBarWidget::signalExitButtonClicked
            , this
            , &QWidget::close);
}

void StyleWindow::initAppIcons()
{
    m_trayIconMenu = new QMenu(this);
    m_systemTrayIcon = new QSystemTrayIcon(this);
    m_minimizeAction = new
            QAction(QCoreApplication::translate("TrayUI", "Minimize"), this);
    m_maximizeAction = new
            QAction(QCoreApplication::translate("TrayUI", "Maximize"), this);
    m_restoreAction = new
            QAction(QCoreApplication::translate("TrayUI", "Restore"), this);
    m_quitAction = new QAction(QCoreApplication::translate("TrayUI", "Quit")
                               , this);
    m_trayIconMenu->addAction(m_minimizeAction);
    m_trayIconMenu->addAction(m_maximizeAction);
    m_trayIconMenu->addAction(m_restoreAction);
    m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(m_quitAction);
    m_systemTrayIcon->setContextMenu(m_trayIconMenu);
    const auto icon = QIcon(Res::TrayIconSVG);
    m_systemTrayIcon->setIcon(icon);
    m_systemTrayIcon->setToolTip(WINDOW_TITLE);
    m_systemTrayIcon->show();
    connect(m_quitAction, &QAction::triggered, qApp, &QApplication::quit);
    connect(m_maximizeAction
            , &QAction::triggered
            , this
            , &StyleWindow::showMaximized);
    connect(m_minimizeAction, &QAction::triggered, this, &StyleWindow::hide);
    connect(m_restoreAction
            , &QAction::triggered
            , this
            , &StyleWindow::showNormal);
}

void StyleWindow::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(this->rect(), 5, 5);
}

void StyleWindow::connectCoreWindow()
{
    connect(m_topBarWidget
            , &Ui::TopBarWidget::signalSettingsButtonClicked
            , m_windowManager
            , &Ui::WindowManager::showSettingsWidget);

    connect(m_topBarWidget
            , &Ui::TopBarWidget::signalPreButtonClicked
            , m_windowManager
            , &Ui::WindowManager::showViewWidget);

    connect(m_windowManager
            , &Ui::WindowManager::signalPlayToggle
            , m_coreService
            , &Core::CoreService::playToggle);

    connect(m_windowManager
            , &Ui::WindowManager::signalNextMusic
            , m_coreService
            , &Core::CoreService::nextMusic);

    connect(m_windowManager
            , &Ui::WindowManager::signalPreMusic
            , m_coreService
            , &Core::CoreService::preMusic);

    connect(m_windowManager
            , &Ui::WindowManager::signalSetVolume
            , m_coreService
            , &Core::CoreService::setVolume);

    connect(m_windowManager
            , &Ui::WindowManager::signalSetPlayerPosition
            , m_coreService
            , &Core::CoreService::setPlayerPosition);

    connect(m_windowManager
            , &Ui::WindowManager::signalChangePlayMode
            , m_coreService
            , &Core::CoreService::changePlayMode);

    connect(m_windowManager
            , &Ui::WindowManager::signalPlaylistButtonClicked
            , m_coreService
            , &Core::CoreService::handleDisplayPlaylist);

    connect(m_windowManager
            , &Ui::WindowManager::signalViewPlayButtonClicked
            , m_coreService
            , &Core::CoreService::handlePlaylistItemSelection);

    connect(m_windowManager
            , &Ui::WindowManager::signalUserPlaylistButtonAdded
            , m_coreService
            , &Core::CoreService::addUserPlaylist);

    connect(m_windowManager
            , &Ui::WindowManager::signalUserPlaylistButtonRemoved
            , m_coreService
            , &Core::CoreService::removeUserPlaylist);

    connect(m_windowManager
            , &Ui::WindowManager::signalLocalMusicDirectoryAdded
            , m_coreService
            , &Core::CoreService::appendLocalMusicPath);

    connect(m_windowManager
            , &Ui::WindowManager::signalLocalMusicDirectoryRemoved
            , m_coreService
            , &Core::CoreService::removeLocalMusicPath);

    connect(m_windowManager
            , &Ui::WindowManager::signalCopyMusicToList
            , m_coreService
            , &Core::CoreService::copyMusicToList);

    connect(m_windowManager
            , &Ui::WindowManager::signalRemoveMusicFromList
            , m_coreService
            , &Core::CoreService::removeMusicFromList);

    connect(m_coreService
            , &Core::CoreService::signalCurrentTitleListChanged
            , m_windowManager
            , &Ui::WindowManager::handlePlaylistDisplayingOnView);

    connect(m_coreService
            , &Core::CoreService::signalCurrentMusicSourceChanged
            , m_windowManager
            , &Ui::WindowManager::updateCurrentMusic);

    connect(m_coreService
            , &Core::CoreService::signalPlayingStatusChanged
            , m_windowManager
            , &Ui::WindowManager::updatePlayingStatus);

    connect(m_coreService
            , &Core::CoreService::signalPlayerPositionChanged
            , m_windowManager
            , &Ui::WindowManager::updateProgressBarPosition);

    connect(m_coreService
            , &Core::CoreService::signalPlayModeChanged
            , m_windowManager
            , &Ui::WindowManager::updatePlayModeIcon);

    connect(m_coreService
            , &Core::CoreService::signalUserPlaylistAdded
            , m_windowManager
            , &Ui::WindowManager::addUserListButton);

    connect(m_coreService
            , &Core::CoreService::signalUserPlaylistRemoved
            , m_windowManager
            , &Ui::WindowManager::removeUserListButton);

    connect(m_coreService
            , &Core::CoreService::signalUserKeysChanged
            , m_windowManager
            , &Ui::WindowManager::handleUserListKeysUpdated);

    connect(m_coreService
            , &Core::CoreService::signalListCacheChanged
            , m_windowManager
            , &Ui::WindowManager::handleListCacheUpdated);

    connect(m_coreService
            , &Core::CoreService::signalLocalPathsChanged
            , m_windowManager
            , &Ui::WindowManager::handleLocalPathSettingsUpdated);

    connect(m_coreService
            , &Core::CoreService::signalCurrentListChanged
            , m_windowManager
            , &Ui::WindowManager::handleCurrentPlaylistKeyChanged);

    connect(m_coreService
            , &Core::CoreService::signalLyricChanged
            , m_windowManager
            , &Ui::WindowManager::updateLyric);

    connect(m_coreService
            , &Core::CoreService::signalLyricLineIndexChanged
            , m_windowManager
            , &Ui::WindowManager::updateLyricLineIndex);

    connect(m_windowManager
            , &Ui::WindowManager::signalMuteTg
            , m_coreService
            , &Core::CoreService::muteToggle);

    connect(m_coreService
            , &Core::CoreService::signalMuteChanged
            , m_windowManager
            , &Ui::WindowManager::updateMuteIcon);

    connect(m_coreService
            , &Core::CoreService::signalVolumeChanged
            , m_windowManager
            , &Ui::WindowManager::updateVolumeValue);

    connect(m_coreService
            , &Core::CoreService::signalDurationChanged
            , m_windowManager
            , &Ui::WindowManager::updateMusicDuration);
}

void StyleWindow::setVisible(const bool visible)
{
    m_restoreAction->setEnabled(isMaximized() || !visible);
    m_minimizeAction->setEnabled(visible);
    m_maximizeAction->setEnabled(!isMaximized());
    QWidget::setVisible(visible);
}

void StyleWindow::closeEvent(QCloseEvent* event)
{
    if (!event->spontaneous() || !isVisible())
        return;
    if (m_systemTrayIcon->isVisible())
    {
        this->hide();
        event->ignore();
    }
}
}
