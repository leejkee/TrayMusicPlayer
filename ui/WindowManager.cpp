//
// Created by cww on 25-4-1.
//

#include "WindowManager.h"
#include <PlayerWidget.h>
#include <ViewWidget.h>
#include <MusicListWidget.h>
#include <TopBarWidget.h>
#include <SettingsWidget.h>

#include <QVBoxLayout>
#include <QSplitter>
#include <QGroupBox>
#include <QStackedWidget>


inline void InitMyQRC() {
    Q_INIT_RESOURCE(svg);
    Q_INIT_RESOURCE(qss);
}


namespace Tray::Ui {
    class WindowManagerPrivate {
    public:
        TopBarWidget *m_topBarWidget;
        MusicListWidget *m_musicListWidget;
        ViewWidget *m_viewWidget;
        SettingsWidget *m_settingsWidget;
        PlayerWidget *m_playerWidget;

        QStackedWidget *m_stackedMainWidget;
        QStackedWidget *m_stackedViewWidget;
        QWidget *m_frontWidget;
    };

    WindowManager::WindowManager(QWidget *parent) : QWidget(parent), d(std::make_unique<WindowManagerPrivate>()) {
        InitMyQRC();
        d->m_stackedMainWidget = new QStackedWidget(this);
        d->m_stackedViewWidget = new QStackedWidget(this);
        d->m_frontWidget = new QWidget(this);

        d->m_musicListWidget = new MusicListWidget(this);
        d->m_playerWidget = new PlayerWidget(this);
        d->m_viewWidget = new ViewWidget(this);
        d->m_topBarWidget = new TopBarWidget(this);
        d->m_settingsWidget = new SettingsWidget(this);


        d->m_stackedViewWidget->addWidget(d->m_viewWidget);
        d->m_stackedViewWidget->addWidget(d->m_settingsWidget);

        const auto splitterMusicListAndView = new QSplitter(Qt::Horizontal);
        splitterMusicListAndView->addWidget(d->m_musicListWidget);
        splitterMusicListAndView->addWidget(d->m_stackedViewWidget);

        const auto playerWidgetGroupBox = new QGroupBox;
        const auto playerWidgetGroupBoxLayout = new QHBoxLayout;
        playerWidgetGroupBoxLayout->setSpacing(0);
        playerWidgetGroupBoxLayout->setContentsMargins(0, 0, 0, 0);
        playerWidgetGroupBoxLayout->addWidget(d->m_playerWidget);
        playerWidgetGroupBox->setLayout(playerWidgetGroupBoxLayout);
        playerWidgetGroupBox->setFixedHeight(80);

        const auto frontLayout = new QVBoxLayout(d->m_frontWidget);
        frontLayout->addWidget(d->m_topBarWidget);
        frontLayout->addWidget(splitterMusicListAndView);
        frontLayout->addWidget(playerWidgetGroupBox);

        d->m_stackedMainWidget->addWidget(d->m_frontWidget);

        const auto mainLayout = new QGridLayout(this);
        mainLayout->addWidget(d->m_stackedMainWidget);
        createConnections();
    }

    void WindowManager::initDefaultSettings(const QStringList &localDir, const QStringList &userKeys) {
        d->m_settingsWidget->updateLocalPaths(localDir);
        d->m_musicListWidget->initUserListButtons(userKeys);
        d->m_viewWidget->setUserPlaylistKeys(userKeys);
    }

    void WindowManager::updateCurrentMusic(const int index, const QString &name, const int duration) {
        d->m_viewWidget->updateCurrentIndex(index);
        d->m_playerWidget->updateMusicName(name);
        d->m_playerWidget->updateMusicDuration(duration);
    }

    void WindowManager::updatePlayingStatus(const bool b) {
        d->m_playerWidget->setRotationStatus(b);
        d->m_playerWidget->setPlayButtonIcon(b);
        d->m_viewWidget->updatePlayingStatus(b);
    }

    WindowManager::~WindowManager() {
    }

    void WindowManager::updateProgressBarPosition(const qint64 pos) {
        d->m_playerWidget->updateProgressBarPosition(pos);
    }

    void WindowManager::updateVolumeCtrlButtonIcon(const bool b) {
        d->m_playerWidget->setVolumeCtrlButtonIcon(b);
    }

    void WindowManager::updatePlayModeIcon(const int mode) {
        d->m_playerWidget->updatePlayModeIcon(mode);
    }

    void WindowManager::showPlaylistOnView(const QString &name, const QStringList &titleList) {
        d->m_viewWidget->showMusicList(name, titleList);
    }

    void WindowManager::updateUserPlaylistKeys(const QStringList &list) {
        d->m_viewWidget->setUserPlaylistKeys(list);
    }

    void WindowManager::updateCurrentViewList(const QString &key, const QStringList &titleList) {
        d->m_viewWidget->updateViewList(key, titleList);
    }

    void WindowManager::updateCurrentPlaylist(const QString &key) {
        d->m_viewWidget->updateStatusRenderCurrentPlaylist(key);
    }

    // settings section begin
    void WindowManager::updateSettingsLocalPaths(const QStringList &paths) {
        d->m_settingsWidget->updateLocalPaths(paths);
    }

    // settings section end
    void WindowManager::createConnections() {
        connect(d->m_playerWidget, &PlayerWidget::signalPlayToggle,
                this, [this] {
                    Q_EMIT signalPlayToggle();
                });

        connect(d->m_playerWidget, &PlayerWidget::signalNextMusic,
                this, [this]() { Q_EMIT signalNextMusic(); });

        connect(d->m_playerWidget, &PlayerWidget::signalPreviousMusic,
                this, [this]() { Q_EMIT signalPreMusic(); });

        connect(d->m_playerWidget, &PlayerWidget::signalSetVolume,
                this, [this](const int v) {
                    Q_EMIT signalSetVolume(v);
                });

        connect(d->m_playerWidget, &PlayerWidget::signalSetMusicPosition,
                this, [this](const qint64 pos) { Q_EMIT signalSetPlayerPosition(pos); });

        connect(d->m_playerWidget, &PlayerWidget::signalPlayModeChanged,
                this, [this]() { Q_EMIT signalPlayModeChanged(); });

        connect(d->m_musicListWidget, &MusicListWidget::signalMusicListButtonClicked,
                this, [this](const QString &list) { Q_EMIT signalPlaylistButtonClicked(list); });

        connect(d->m_musicListWidget, &MusicListWidget::signalPlaylistButtonDeleted,
                this, [this](const QString &key) { Q_EMIT signalPlaylistDeleted(key); });
        // add button
        connect(d->m_musicListWidget, &MusicListWidget::signalMusicListButtonAdded,
                this, [this](const QString &key) {
                    Q_EMIT signalPlaylistAdded(key);
                });

        connect(d->m_topBarWidget, &TopBarWidget::signalPreButtonClicked,
                this, [this]() {
                    d->m_stackedViewWidget->setCurrentIndex(0);
                });
        connect(d->m_topBarWidget, &TopBarWidget::signalSettingsButtonClicked,
                this, [this]() {
                    d->m_stackedViewWidget->setCurrentIndex(1);
                });


        // 4 signal from settingsWidget
        connect(d->m_settingsWidget, &SettingsWidget::signalLocalDirAdded,
                this, [this](const QString &dir) { Q_EMIT signalLocalMusicDirectoryAdded(dir); });

        connect(d->m_settingsWidget, &SettingsWidget::signalLocalDirRemoved,
                this, [this](const QString &dir) { Q_EMIT signalLocalMusicDirectoryRemoved(dir); });
        // 4 signal from settingsWidget


        connect(d->m_viewWidget, &ViewWidget::signalViewItemAddToList,
                this, [this](const QString &s, const QString &d, const int i) {
                    Q_EMIT signalAddSongToList(s, d, i);
                });

        connect(d->m_viewWidget, &ViewWidget::signalViewItemDel,
                this, [this](const QString &key, const QString &title) {
                    Q_EMIT signalDelSongFromList(key, title);
                });
        /// ViewWidget: ItemPlayButton -> Core: play music with index
        connect(d->m_viewWidget, &ViewWidget::signalViewItemPlayButtonClicked,
                this, [this](const QString &key, const int index) {
                    Q_EMIT signalViewPlayButtonClicked(key, index);
                });
    }
}
