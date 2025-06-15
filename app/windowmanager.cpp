//
// Created by cww on 25-4-1.
//
#include "windowmanager.h"
#include <core.h>
#include <musiclistwidget.h>
#include <playerwidget.h>
#include <topbarwidget.h>
#include <viewwidget.h>
#include <settingswidget.h>

#include <QGroupBox>
#include <QSplitter>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <trayqss.h>


inline void InitMyQRC() {
    Q_INIT_RESOURCE(svg);
    Q_INIT_RESOURCE(qss);
}

namespace Tray::Ui {
class WindowManagerPrivate {
public:
    TopBarWidget* m_topBarWidget;
    MusicListWidget* m_musicListWidget;
    ViewWidget* m_viewWidget;
    SettingsWidget* m_settingsWidget;

    PlayerWidget* m_playerWidget;

    QStackedWidget* m_stackedMainWidget;
    QStackedWidget* m_stackedViewWidget;
    QWidget* m_frontWidget;

    Core::Core* q_core;
};

WindowManager::WindowManager(Core::Core* core, QWidget* parent) :
    QWidget(parent), d(std::make_unique<WindowManagerPrivate>()) {
    InitMyQRC();
    d->q_core = core;
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

void WindowManager::initDefaultSettings(const QStringList& localDir,
                                        const QStringList& userKeys,
                                        const unsigned volume) {
    d->m_settingsWidget->updateLocalPaths(localDir);
    d->m_musicListWidget->initUserListButtons(userKeys);
    d->m_viewWidget->setUserPlaylistKeys(userKeys);
    d->m_playerWidget->setSliderVolumeValue(volume);
}

void WindowManager::updateCurrentMusic(const int index, const QString& name,
                                       const int duration) {
    d->m_viewWidget->updateCurrentIndex(index);
    d->m_playerWidget->updateMusicName(name);
    d->m_playerWidget->updateMusicDuration(duration);
}

void WindowManager::updatePlayingStatus(const bool b) {
    d->m_playerWidget->setRotationStatus(b);
    d->m_playerWidget->setPlayButtonIcon(b);
    d->m_viewWidget->updatePlayingStatus(b);
}

WindowManager::~WindowManager() {}

void WindowManager::updateProgressBarPosition(const qint64 pos) {
    d->m_playerWidget->updateProgressBarPosition(pos);
}

void WindowManager::updateVolumeCtrlButtonIcon(const bool b) {
    d->m_playerWidget->setVolumeCtrlButtonIcon(b);
}

void WindowManager::updatePlayModeIcon(const int mode) {
    d->m_playerWidget->updatePlayModeIcon(mode);
}

void WindowManager::showCurrentTitleListToView(const QString& name,
                                               const QStringList& titleList) {
    d->m_viewWidget->showCurrentTitleListToView(name, titleList);
}

void WindowManager::updateUserPlaylistKeys(const QStringList& list) {
    d->m_viewWidget->setUserPlaylistKeys(list);
}

void WindowManager::updateCurrentViewList(const QString& key,
                                          const QStringList& titleList) {
    d->m_viewWidget->refreshCurrentMusicList(key, titleList);
}

void WindowManager::updateCurrentPlaylistKey(const QString& key) {
    d->m_viewWidget->syncRenderWithCurrentPlaylist(key);
}

void WindowManager::updateSettingsLocalPaths(const QStringList& paths) {
    d->m_settingsWidget->updateLocalPaths(paths);
}

void WindowManager::removeUserPlaylistButton(const QString& key) {
    d->m_musicListWidget->removeUserButton(key);
}

void WindowManager::addUserPlaylistButton(const QString& key) {
    d->m_musicListWidget->appendUserButton(key);
}

void WindowManager::createConnections() {

    // 1
    connect(d->m_playerWidget, &PlayerWidget::signalPlayToggle, d->q_core,
            &Core::Core::playToggle);

    // 2
    connect(d->q_core, &Core::Core::signalNotifyUiCurrentMusicChanged, this,
            &WindowManager::updateCurrentMusic);

    // 3
    connect(d->q_core, &Core::Core::signalPositionChanged, d->m_playerWidget,
            &PlayerWidget::updateProgressBarPosition);

    // 4
    connect(d->q_core, &Core::Core::signalNotifyUiPlayingStatusChanged, this,
            &WindowManager::updatePlayingStatus);

    // 5
    connect(d->m_playerWidget, &PlayerWidget::signalPreviousMusic, d->q_core,
            &Core::Core::preMusic);

    // 6
    connect(d->m_playerWidget, &PlayerWidget::signalNextMusic, d->q_core,
            &Core::Core::nextMusic);

    // 7
    connect(d->q_core, &Core::Core::signalNotifyUiUpdateMuteIcon, d->m_playerWidget,
            &PlayerWidget::updateMuteIcon);

    // 8
    connect(d->m_playerWidget, &PlayerWidget::signalSetVolume, d->q_core,
            &Core::Core::setVolume);

    // 9
    connect(d->m_playerWidget, &PlayerWidget::signalSetMusicPosition, d->q_core,
            &Core::Core::setPlayerPosition);

    // 10
    connect(d->q_core, &Core::Core::signalNotifyUiPlayModeChanged,
            d->m_playerWidget, &PlayerWidget::updatePlayModeIcon);

    // 11
    connect(d->m_playerWidget, &PlayerWidget::signalPlayModeChanged, d->q_core,
            &Core::Core::changePlayMode);

    // 12
    connect(d->q_core, &Core::Core::signalInitUiDefaultSettings, this,
            &WindowManager::initDefaultSettings);

    // 13
    connect(d->m_viewWidget, &ViewWidget::signalViewItemPlayButtonClicked,
            d->q_core, &Core::Core::playToggleWithListAndIndex);

    // 14
    connect(d->m_viewWidget, &ViewWidget::signalMusicRemovedFromList, d->q_core,
            &Core::Core::removeMusicFromList);

    // 15
    connect(d->m_viewWidget, &ViewWidget::signalMusicAddedToList, d->q_core,
            &Core::Core::addMusicToList);

    // 16
    connect(d->m_musicListWidget,
            &MusicListWidget::signalUserPlaylistButtonAdded, d->q_core,
            &Core::Core::addUserPlaylist);

    // 17
    connect(d->m_musicListWidget,
            &MusicListWidget::signalUserPlaylistButtonRemoved, d->q_core,
            &Core::Core::removeUserPlaylist);

    // 18
    connect(d->q_core, &Core::Core::signalNotifyUiToRemoveUserPlaylist,
            d->m_musicListWidget, &MusicListWidget::removeUserButton);

    // 19
    connect(d->q_core, &Core::Core::signalNotifyUiToAddUserPlaylist,
            d->m_musicListWidget, &MusicListWidget::appendUserButton);

    // 20
    connect(d->m_musicListWidget,
            &MusicListWidget::signalMusicListButtonClicked, d->q_core,
            &Core::Core::requestPlaylist);

    // 21
    connect(d->q_core, &Core::Core::signalSendUiCurrentTitleList,
            d->m_viewWidget, &ViewWidget::showCurrentTitleListToView);

    // 22
    connect(d->q_core, &Core::Core::signalNotifyUiCacheModified,
            d->m_viewWidget, &ViewWidget::refreshCurrentMusicList);

    // // 23
    // connect(d->m_settingsWidget, &SettingsWidget::signalLocalDirAdded,
    //         d->q_core, &Core::Core::appendLocalMusicPath);
    //
    // // 24
    // connect(d->m_settingsWidget, &SettingsWidget::signalLocalDirRemoved,
    //         d->q_core, &Core::Core::removeLocalMusicPath);
    //
    // // 25
    // connect(d->q_core, &Core::Core::signalNotifyUiToUpdateLocalPaths,
    //         d->m_settingsWidget, &SettingsWidget::updateLocalPaths);

    // 26
    connect(d->q_core, &Core::Core::signalNotifyUiUserKeySetsChanged,
            d->m_viewWidget, &ViewWidget::setUserPlaylistKeys);

    // 27
    connect(d->q_core, &Core::Core::signalNotifyUiCurrentListKeyChanged,
            d->m_viewWidget, &ViewWidget::syncRenderWithCurrentPlaylist);

    // 28
    connect(d->m_playerWidget, &PlayerWidget::signalSetMute, d->q_core,
            &Core::Core::setMute);

    connect(d->m_topBarWidget, &TopBarWidget::signalPreButtonClicked, this,
            [this]() { d->m_stackedViewWidget->setCurrentIndex(0); });

    connect(d->m_topBarWidget, &TopBarWidget::signalSettingsButtonClicked, this,
            [this]() { d->m_stackedViewWidget->setCurrentIndex(1); });
}
} // namespace Tray::Ui
