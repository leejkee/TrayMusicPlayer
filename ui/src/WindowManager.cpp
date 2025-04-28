//
// Created by cww on 25-4-1.
//

#include <ICore.h>
#include <ui/WindowManager.h>
#include <ui/PlayerWidget.h>
#include <ui/ViewWidget.h>
#include <ui/MusicListWidget.h>
#include <ui/TopBarWidget.h>
#include <ui/SettingsWidget.h>

#include <QVBoxLayout>
#include <QSplitter>
#include <QStackedWidget>
#include <QGroupBox>


namespace UI::WindowManager {
    WindowManager::WindowManager(QWidget *parent) : QWidget(parent), m_core(nullptr) {
        m_core = Core::ICore::create(this);

        m_stackedMainWidget = new QStackedWidget(this);
        m_stackedViewWidget = new QStackedWidget(this);
        m_frontWidget = new QWidget(this);

        m_musicListWidget = new MusicListWidget::MusicListWidget(this);
        m_playerWidget = new PlayerWidget::PlayerWidget(this);
        m_viewWidget = new ViewWidget::ViewWidget(this);
        m_topBarWidget = new TopBarWidget::TopBarWidget(this);
        m_settingsWidget = new SettingsWidget::SettingsWidget(this);


        m_stackedViewWidget->addWidget(m_viewWidget);
        m_stackedViewWidget->addWidget(m_settingsWidget);

        const auto splitterMusicListAndView = new QSplitter(Qt::Horizontal);
        splitterMusicListAndView->addWidget(m_musicListWidget);
        splitterMusicListAndView->addWidget(m_stackedViewWidget);

        const auto playerWidgetGroupBox = new QGroupBox;
        const auto m_groupBoxLayout = new QHBoxLayout;
        m_groupBoxLayout->setSpacing(0);
        m_groupBoxLayout->setContentsMargins(0, 0, 0, 0);
        m_groupBoxLayout->addWidget(m_playerWidget);
        playerWidgetGroupBox->setLayout(m_groupBoxLayout);
        playerWidgetGroupBox->setFixedHeight(80);

        const auto frontLayout = new QVBoxLayout(m_frontWidget);
        frontLayout->addWidget(m_topBarWidget);
        frontLayout->addWidget(splitterMusicListAndView);
        frontLayout->addWidget(playerWidgetGroupBox);

        m_stackedMainWidget->addWidget(m_frontWidget);

        const auto mainLayout = new QGridLayout(this);
        mainLayout->addWidget(m_stackedMainWidget);

        createConnections();
        m_core->initDefaultSettings();
        this->initDefaultSettings();
    }

    void WindowManager::initDefaultSettings() {
        m_settingsWidget->updateLocalPaths(m_core->getLocalMusicPaths());
        m_musicListWidget->initUserListButtons(m_core->getKeysOfUserPlaylist());
        m_viewWidget->initUserPlaylistKeys(m_core->getKeysOfUserPlaylist());
    }

    void WindowManager::updateCurrentMusic(const int index, const QString &name, const int duration) {
        m_viewWidget->updateCurrentIndex(index);
        m_playerWidget->updateMusicName(name);
        m_playerWidget->updateMusicDuration(duration);
    }

    void WindowManager::updatePlayingStatus(const bool b) {
        m_playerWidget->setRotationStatus(b);
        m_playerWidget->setPlayButtonIcon(b);
        m_viewWidget->updatePlayingStatus(b);
    }

    WindowManager::~WindowManager() {
    }

    void WindowManager::createConnections() {
        connect(m_playerWidget, &PlayerWidget::PlayerWidget::signalPlayToggle,
                m_core, &Core::ICore::playToggle);

        connect(m_core, &Core::ICore::signalCurrentMusicChanged,
                this, &WindowManager::updateCurrentMusic);

        connect(m_core, &Core::ICore::signalPositionChanged,
                m_playerWidget, &PlayerWidget::PlayerWidget::updateProgressBarPosition);

        connect(m_core, &Core::ICore::signalPlayingChanged,
                this, &WindowManager::updatePlayingStatus);

        connect(m_playerWidget, &PlayerWidget::PlayerWidget::signalNextMusic,
                m_core, &Core::ICore::nextMusic);

        connect(m_playerWidget, &PlayerWidget::PlayerWidget::signalPreviousMusic,
                m_core, &Core::ICore::preMusic);

        connect(m_playerWidget, &PlayerWidget::PlayerWidget::signalSetVolume,
                m_core, &Core::ICore::setVolume);

        connect(m_core, &Core::ICore::signalIsMuted,
                m_playerWidget, &PlayerWidget::PlayerWidget::setVolumeCtrlButtonIcon);

        connect(m_playerWidget, &PlayerWidget::PlayerWidget::signalSetMusicPosition,
                m_core, &Core::ICore::setMusicPosition);

        connect(m_core, &Core::ICore::signalPlayModeChanged,
                m_playerWidget, &PlayerWidget::PlayerWidget::updatePlayModeIcon);

        connect(m_playerWidget, &PlayerWidget::PlayerWidget::signalPlayModeChanged,
                m_core, &Core::ICore::changePlayMode);

        connect(m_musicListWidget, &MusicListWidget::MusicListWidget::signalMusicListButtonClicked,
                m_core, &Core::ICore::requestMusicListByName);

        connect(m_core, &Core::ICore::signalMusicListChanged,
                m_viewWidget, &ViewWidget::ViewWidget::showMusicList);

        /// ViewWidget: ItemPlayButton -> Core: play music with index
        connect(m_viewWidget, &ViewWidget::ViewWidget::signalViewItemPlayButtonClicked,
                m_core, &Core::ICore::playToggleWithListAndIndex);

        connect(m_topBarWidget, &TopBarWidget::TopBarWidget::signalPreButtonClicked,
                this, [this]() {
                    m_stackedViewWidget->setCurrentIndex(0);
                });
        connect(m_topBarWidget, &TopBarWidget::TopBarWidget::signalSettingsButtonClicked,
                this, [this]() {
                    m_stackedViewWidget->setCurrentIndex(1);
                });

        // add button
        connect(m_musicListWidget, &MusicListWidget::MusicListWidget::signalMusicListButtonAdded,
                m_core, &Core::ICore::addUserList);


        connect(m_settingsWidget, &SettingsWidget::SettingsWidget::signalAddButton,
                m_core, &Core::ICore::addLocalMusicPath);

        connect(m_settingsWidget, &SettingsWidget::SettingsWidget::signalRemoveButton,
                m_core, &Core::ICore::removeLocalMusicPath);


        connect(m_core, &Core::ICore::signalLocalPathsChanged, this, [this]() {
            m_settingsWidget->updateLocalPaths(m_core->getLocalMusicPaths());
        });

        connect(m_viewWidget, &ViewWidget::ViewWidget::signalViewItemAddToList,
                m_core, &Core::ICore::addMusicToList);
    }
}
