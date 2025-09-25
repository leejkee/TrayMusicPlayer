//
// Created by 31305 on 2025/9/9.
//
#include <windowmanager/windowmanager.h>
#include <playerwidget/playerwidget.h>
#include <topbarwidget/topbarwidget.h>
#include <musiclistwidget/musiclistwidget.h>
#include <viewwidget/viewwidget.h>
#include <lyricwidget/lyricwidget.h>
#include <settingswidget/settingswidget.h>
#include <trayqss.h>
#include <log/log.h>
#include <QSplitter>
#include <QStackedWidget>
#include <QVBoxLayout>

namespace Tray::Ui
{
class WindowManagerPrivate
{
public:
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
    LyricWidget* m_lyricWidget;
    SettingsWidget* m_settingsWidget;
    QFrame* m_lineSplitterUp;
    QFrame* m_lineSplitterDown;
};

WindowManager::WindowManager(const WindowInitData& initData, QWidget* parent)
    : QWidget(parent),
      d(std::make_unique<WindowManagerPrivate>())
{
    d->m_windowManagerLayout = new QVBoxLayout;
    d->m_mainStackedWidget = new QStackedWidget(this);
    d->m_playerWidget = new PlayerWidget(this);
    d->m_primaryWidget = new QWidget(this);
    d->m_primaryWidgetLayout = new QVBoxLayout;
    d->m_topBarWidget = new TopBarWidget(this);
    d->m_musicListWidget = new MusicListWidget(this);
    d->m_viewAndPlaylistsSplitter = new QSplitter(Qt::Horizontal, this);
    d->m_viewAndPlaylistsSplitter->
       setStyleSheet(Res::readQss(Res::VIEW_SPLITTER_QSS));
    d->m_viewStackedWidget = new QStackedWidget(this);
    d->m_viewWidget = new ViewWidget(this);
    d->m_lyricWidget = new LyricWidget(this);
    d->m_settingsWidget = new SettingsWidget(this);
    d->m_lineSplitterUp = new QFrame(this);
    d->m_lineSplitterUp->setFrameShape(QFrame::HLine);
    d->m_lineSplitterDown = new QFrame(this);
    d->m_lineSplitterDown->setFrameShape(QFrame::HLine);
    d->m_lineSplitterUp->setStyleSheet(Res::readQss(Res::HORIZONTAL_LINE_QSS));
    d->m_lineSplitterDown->
       setStyleSheet(Res::readQss(Res::HORIZONTAL_LINE_QSS));

    setLayout(d->m_windowManagerLayout);
    d->m_windowManagerLayout->addWidget(d->m_mainStackedWidget);
    d->m_windowManagerLayout->addWidget(d->m_lineSplitterDown);
    d->m_windowManagerLayout->addWidget(d->m_playerWidget);
    d->m_windowManagerLayout->setContentsMargins(0, 0, 0, 0);
    d->m_windowManagerLayout->setSpacing(0);

    // primaryWidget and lyricWidget
    d->m_mainStackedWidget->addWidget(d->m_primaryWidget);
    d->m_mainStackedWidget->addWidget(d->m_lyricWidget);

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
    initDefaultSettings(initData);
}

WindowManager::~WindowManager() = default;



void WindowManager::createConnections()
{
    /* TopBar */
    connect(d->m_topBarWidget
            , &TopBarWidget::signalSettingsButtonClicked
            , this
            , [this]()
            {
                d->m_viewStackedWidget->setCurrentWidget(d->m_settingsWidget);
            });
    connect(d->m_topBarWidget
            , &TopBarWidget::signalPreButtonClicked
            , this
            , [this]()
            {
                d->m_viewStackedWidget->setCurrentWidget(d->m_viewWidget);
            });

    /* PlayerWidget */
    connect(d->m_playerWidget
            , &PlayerWidget::signalPlayToggle
            , this
            , [this]
            {
                Q_EMIT signalPlayToggle();
            });

    connect(d->m_playerWidget
            , &PlayerWidget::signalPreviousMusic
            , this
            , [this]
            {
                Q_EMIT signalPreMusic();
            });

    connect(d->m_playerWidget
            , &PlayerWidget::signalNextMusic
            , this
            , [this]
            {
                Q_EMIT signalNextMusic();
            });

    connect(d->m_playerWidget
            , &PlayerWidget::signalSetVolume
            , this
            , [this](const int v)
            {
                Q_EMIT signalSetVolume(v);
            });

    connect(d->m_playerWidget
            , &PlayerWidget::signalPlayModeChanged
            , this
            , [this]
            {
                Q_EMIT signalChangePlayMode();
            });

    connect(d->m_viewWidget
            , &ViewWidget::signalViewItemPlayButtonClicked
            , this
            , [this](const QString& listKey, const int index)
            {
                Q_EMIT signalViewPlayButtonClicked(listKey, index);
            });

    connect(d->m_viewWidget
            , &ViewWidget::signalMusicRemovedFromList
            , this
            , [this](const QString& listKey, const QString& title)
            {
                Q_EMIT signalRemoveMusicFromList(listKey, title);
            });

    connect(d->m_viewWidget
            , &ViewWidget::signalMusicAddedToList
            , this
            , [this](const QString& source
                     , const QString& destination
                     , const int index)
            {
                Q_EMIT signalCopyMusicToList(source, destination, index);
            });

    connect(d->m_musicListWidget
            , &MusicListWidget::signalUserPlaylistButtonRemoved
            , this
            , [this](const QString& listKey)
            {
                Q_EMIT signalUserPlaylistButtonRemoved(listKey);
            });

    connect(d->m_musicListWidget
            , &MusicListWidget::signalUserPlaylistButtonAdded
            , this
            , [this](const QString& listKey)
            {
                Q_EMIT signalUserPlaylistButtonAdded(listKey);
            });

    connect(d->m_musicListWidget
            , &MusicListWidget::signalMusicListButtonClicked
            , this
            , [this](const QString& listKey)
            {
                Q_EMIT signalPlaylistButtonClicked(listKey);
            });

    connect(d->m_settingsWidget
            , &SettingsWidget::signalLocalDirAdded
            , this
            , [this](const QString& path)
            {
                Q_EMIT signalLocalMusicDirectoryAdded(path);
            });

    connect(d->m_settingsWidget
            , &SettingsWidget::signalLocalDirRemoved
            , this
            , [this](const QString& path)
            {
                Q_EMIT signalLocalMusicDirectoryRemoved(path);
            });

    connect(d->m_lyricWidget
            , &LyricWidget::signalBackButtonClicked
            , this
            , [this]
            {
                d->m_mainStackedWidget->setCurrentWidget(d->m_primaryWidget);
            });

    connect(d->m_playerWidget
            , &PlayerWidget::signalShowLyricWidget
            , this
            , [this]
            {
                d->m_mainStackedWidget->setCurrentWidget(d->m_lyricWidget);
            });

    connect(d->m_playerWidget, &PlayerWidget::signalSetMusicPosition, this, [this](const qint64 value)
    {
        Q_EMIT signalSetPlayerPosition(value);
    });

    connect(d->m_playerWidget, &PlayerWidget::signalSetMute, this, [this]
    {
        Q_EMIT signalMuteTg();
    });
}

void WindowManager::initDefaultSettings(const WindowInitData& initData)
{
    d->m_settingsWidget->updateLocalPaths(initData.initLocalPaths);
    d->m_musicListWidget->initUserListButtons(initData.initUserKeys);
    d->m_viewWidget->updateUserListKeys(initData.initUserKeys);
    d->m_viewWidget->displayTitleListToView(initData.initKey
                                            , initData.initTitleList);
    d->m_playerWidget->setSliderVolumeValue(initData.initVolume);
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

void WindowManager::updateProgressBarPosition(const qint64 pos)
{
    d->m_playerWidget->updateProgressBarPosition(pos);
}

void WindowManager::updateMuteIcon(const bool isMuted)
{
    d->m_playerWidget->updateMuteIcon(isMuted);
}

void WindowManager::updatePlayModeIcon(const int mode)
{
    d->m_playerWidget->updatePlayModeIcon(mode);
}

void WindowManager::removeUserListButton(const QString& listKey)
{
    d->m_musicListWidget->removeUserButton(listKey);
}

void WindowManager::addUserListButton(const QString& listKey)
{
    d->m_musicListWidget->appendUserButton(listKey);
}

void WindowManager::handlePlaylistDisplayingOnView(
    const QString& listKey
    , const QStringList& titleList)
{
    d->m_viewWidget->displayTitleListToView(listKey, titleList);
}

void WindowManager::handleListCacheUpdated(const QString& listKey
                                           , const QStringList& titleList)
{
    d->m_viewWidget->handleListCacheUpdated(listKey, titleList);
}

void WindowManager::handleUserListKeysUpdated(const QStringList& keys)
{
    d->m_viewWidget->updateUserListKeys(keys);
}

void WindowManager::handleCurrentPlaylistKeyChanged(const QString& listKey)
{
    d->m_viewWidget->syncRenderWithCurrentPlaylist(listKey);
}

void WindowManager::handleLocalPathSettingsUpdated(
    const QStringList& localPaths)
{
    d->m_settingsWidget->updateLocalPaths(localPaths);
}

void WindowManager::updateLyric(const QString& musicTitle, const QStringList& lyricText
                                , const QList<int64_t>& lyricsTiming)
{
    d->m_lyricWidget->updateLyric(musicTitle, lyricText, lyricsTiming);
}

void WindowManager::updateLyricLineIndex(const int index)
{
    d->m_lyricWidget->updateCurrentTiming(index);
}

void WindowManager::updateVolumeValue(const float volume)
{
    d->m_playerWidget->updateVolumeValue(volume);
}
}
