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
#include <trayqss.h>
#include <uitools.h>

#include <QGroupBox>
#include <QListWidget>
#include <QSplitter>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QFrame>


inline void InitMyQRC()
{
    Q_INIT_RESOURCE(svg);
    Q_INIT_RESOURCE(qss);
}

namespace Tray::Ui
{
class WindowManagerPrivate
{
public:
    explicit WindowManagerPrivate(Core::Core* core, WindowManager* w);
    // Manages the playerWidget and a QStackedWidget that switches between the lyricWidget and the primary content widget.
    QVBoxLayout* m_windowManagerLayout;
    // Handles the primary display and the lyric display, allowing switching between them.
    QStackedWidget* m_mainStackedWidget;
    // Widget that contains the controller panels
    PlayerWidget* m_playerWidget;
    // Contains the topWidget, MusicListWidget, ViewWidget, SettingsWidget
    QWidget* m_primaryWidget;
    // LyricWidget *m_lyricWidget;
    QVBoxLayout* m_primaryWidgetLayout;
    TopBarWidget* m_topBarWidget;
    MusicListWidget* m_musicListWidget;
    QSplitter* m_viewAndPlaylistsSplitter;
    QStackedWidget* m_viewStackedWidget;
    ViewWidget* m_viewWidget;
    SettingsWidget* m_settingsWidget;
    QFrame* m_lineSplitterUp;
    QFrame* m_lineSplitterDown;

    Core::Core* q_core;
    WindowManager* q_ptr;
};

WindowManagerPrivate::WindowManagerPrivate(Core::Core* core, WindowManager* w)
    : q_core(core),
      q_ptr(w)
{
    m_windowManagerLayout = new QVBoxLayout;
    m_mainStackedWidget = new QStackedWidget(q_ptr);
    m_playerWidget = new PlayerWidget(q_ptr);
    m_primaryWidget = new QWidget(q_ptr);
    m_primaryWidgetLayout = new QVBoxLayout;
    m_topBarWidget = new TopBarWidget(q_ptr);
    m_musicListWidget = new MusicListWidget(q_ptr);
    m_viewAndPlaylistsSplitter = new QSplitter(Qt::Horizontal, q_ptr);
    m_viewAndPlaylistsSplitter->setStyleSheet(readQSS(Res::VIEW_SPLITTER_QSS));
    m_viewStackedWidget = new QStackedWidget(q_ptr);
    m_viewWidget = new ViewWidget(q_ptr);
    m_settingsWidget = new SettingsWidget(q_ptr);
    m_lineSplitterUp = new QFrame(q_ptr);
    m_lineSplitterUp->setFrameShape(QFrame::HLine);
    m_lineSplitterDown = new QFrame(q_ptr);
    m_lineSplitterDown->setFrameShape(QFrame::HLine);
    m_lineSplitterUp->setStyleSheet(readQSS(Res::HERIZONTAL_LINE_QSS));
    m_lineSplitterDown->setStyleSheet(readQSS(Res::HERIZONTAL_LINE_QSS));
}

WindowManager::WindowManager(Core::Core* core, QWidget* parent)
    : QWidget(parent)
{
    InitMyQRC();
    d = std::make_unique<WindowManagerPrivate>(core, this);
    setLayout(d->m_windowManagerLayout);
    d->m_windowManagerLayout->addWidget(d->m_mainStackedWidget);
    d->m_windowManagerLayout->addWidget(d->m_lineSplitterDown);
    d->m_windowManagerLayout->addWidget(d->m_playerWidget);
    d->m_windowManagerLayout->setContentsMargins(0, 0, 0, 0);
    d->m_windowManagerLayout->setSpacing(0);

    d->m_mainStackedWidget->addWidget(d->m_primaryWidget);

    d->m_primaryWidget->setLayout(d->m_primaryWidgetLayout);
    d->m_primaryWidgetLayout->addWidget(d->m_topBarWidget);
    d->m_primaryWidgetLayout->addWidget(d->m_lineSplitterUp);
    d->m_primaryWidgetLayout->addWidget(d->m_viewAndPlaylistsSplitter);
    d->m_primaryWidgetLayout->setContentsMargins(0, 0, 0, 0);
    d->m_primaryWidgetLayout->setSpacing(0);

    d->m_viewAndPlaylistsSplitter->addWidget(d->m_musicListWidget);
    d->m_viewAndPlaylistsSplitter->addWidget(d->m_viewStackedWidget);

    d->m_viewStackedWidget->addWidget(d->m_viewWidget);
    d->m_viewStackedWidget->addWidget(d->m_settingsWidget);

    createConnections();
}

void WindowManager::initDefaultSettings(const QStringList& localDir
                                        , const QStringList& userKeys
                                        , const unsigned volume)
{
    d->m_settingsWidget->updateLocalPaths(localDir);
    d->m_musicListWidget->initUserListButtons(userKeys);
    d->m_viewWidget->setUserPlaylistKeys(userKeys);
    d->m_playerWidget->setSliderVolumeValue(volume);
}

void WindowManager::updateCurrentMusic(const int index
                                       , const QString& name
                                       , const int duration)
{
    d->m_viewWidget->updateCurrentIndex(index);
    d->m_playerWidget->updateMusicName(name);
    d->m_playerWidget->updateMusicDuration(duration);
}

void WindowManager::updatePlayingStatus(const bool b)
{
    d->m_playerWidget->setRotationStatus(b);
    d->m_playerWidget->setPlayButtonIcon(b);
    d->m_viewWidget->updatePlayingStatus(b);
}

WindowManager::~WindowManager() = default;

void WindowManager::createConnections()
{
    // 1
    connect(d->m_playerWidget
            , &PlayerWidget::signalPlayToggle
            , d->q_core
            , &Core::Core::playToggle);

    // 2
    connect(d->q_core
            , &Core::Core::signalNotifyUiCurrentMusicChanged
            , this
            , &WindowManager::updateCurrentMusic);

    // 3
    connect(d->q_core
            , &Core::Core::signalPositionChanged
            , d->m_playerWidget
            , &PlayerWidget::updateProgressBarPosition);

    // 4
    connect(d->q_core
            , &Core::Core::signalNotifyUiPlayingStatusChanged
            , this
            , &WindowManager::updatePlayingStatus);

    // 5
    connect(d->m_playerWidget
            , &PlayerWidget::signalPreviousMusic
            , d->q_core
            , &Core::Core::preMusic);

    // 6
    connect(d->m_playerWidget
            , &PlayerWidget::signalNextMusic
            , d->q_core
            , &Core::Core::nextMusic);

    // 7
    connect(d->q_core
            , &Core::Core::signalNotifyUiUpdateMuteIcon
            , d->m_playerWidget
            , &PlayerWidget::updateMuteIcon);

    // 8
    connect(d->m_playerWidget
            , &PlayerWidget::signalSetVolume
            , d->q_core
            , &Core::Core::setVolume);

    // 9
    connect(d->m_playerWidget
            , &PlayerWidget::signalSetMusicPosition
            , d->q_core
            , &Core::Core::setPlayerPosition);

    // 10
    connect(d->q_core
            , &Core::Core::signalNotifyUiPlayModeChanged
            , d->m_playerWidget
            , &PlayerWidget::updatePlayModeIcon);

    // 11
    connect(d->m_playerWidget
            , &PlayerWidget::signalPlayModeChanged
            , d->q_core
            , &Core::Core::changePlayMode);

    // 12
    connect(d->q_core
            , &Core::Core::signalInitUiDefaultSettings
            , this
            , &WindowManager::initDefaultSettings);

    // 13
    connect(d->m_viewWidget
            , &ViewWidget::signalViewItemPlayButtonClicked
            , d->q_core
            , &Core::Core::playToggleWithListAndIndex);

    // 14
    connect(d->m_viewWidget
            , &ViewWidget::signalMusicRemovedFromList
            , d->q_core
            , &Core::Core::removeMusicFromList);

    // 15
    connect(d->m_viewWidget
            , &ViewWidget::signalMusicAddedToList
            , d->q_core
            , &Core::Core::addMusicToList);

    // 16
    connect(d->m_musicListWidget
            , &MusicListWidget::signalUserPlaylistButtonAdded
            , d->q_core
            , &Core::Core::addUserPlaylist);

    // 17
    connect(d->m_musicListWidget
            , &MusicListWidget::signalUserPlaylistButtonRemoved
            , d->q_core
            , &Core::Core::removeUserPlaylist);

    // 18
    connect(d->q_core
            , &Core::Core::signalNotifyUiToRemoveUserPlaylist
            , d->m_musicListWidget
            , &MusicListWidget::removeUserButton);

    // 19
    connect(d->q_core
            , &Core::Core::signalNotifyUiToAddUserPlaylist
            , d->m_musicListWidget
            , &MusicListWidget::appendUserButton);

    // 20
    connect(d->m_musicListWidget
            , &MusicListWidget::signalMusicListButtonClicked
            , d->q_core
            , &Core::Core::requestPlaylist);

    // 21
    connect(d->q_core
            , &Core::Core::signalSendUiCurrentTitleList
            , d->m_viewWidget
            , &ViewWidget::showCurrentTitleListToView);

    // 22
    connect(d->q_core
            , &Core::Core::signalNotifyUiCacheModified
            , d->m_viewWidget
            , &ViewWidget::refreshCurrentMusicList);

    // 23
    connect(d->m_settingsWidget
            , &SettingsWidget::signalLocalDirAdded
            , d->q_core
            , &Core::Core::appendLocalMusicPath);

    // 24
    connect(d->m_settingsWidget
            , &SettingsWidget::signalLocalDirRemoved
            , d->q_core
            , &Core::Core::removeLocalMusicPath);

    // 25
    connect(d->q_core
            , &Core::Core::signalNotifyUiToUpdateLocalPaths
            , d->m_settingsWidget
            , &SettingsWidget::updateLocalPaths);

    // 26
    connect(d->q_core
            , &Core::Core::signalNotifyUiUserKeySetsChanged
            , d->m_viewWidget
            , &ViewWidget::setUserPlaylistKeys);

    // 27
    connect(d->q_core
            , &Core::Core::signalNotifyUiCurrentListKeyChanged
            , d->m_viewWidget
            , &ViewWidget::syncRenderWithCurrentPlaylist);

    // 28
    connect(d->m_playerWidget
            , &PlayerWidget::signalSetMute
            , d->q_core
            , &Core::Core::setMute);

    // 29
    connect(d->m_topBarWidget
            , &TopBarWidget::signalPreButtonClicked
            , this
            , [this]()
            {
                d->m_viewStackedWidget->setCurrentWidget(d->m_viewWidget);
            });

    // 30
    connect(d->m_topBarWidget
            , &TopBarWidget::signalSettingsButtonClicked
            , this
            , [this]()
            {
                d->m_viewStackedWidget->setCurrentWidget(d->m_settingsWidget);
            });
}

} // namespace Tray::Ui
