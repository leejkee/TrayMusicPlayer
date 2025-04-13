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
        m_stackedMainWidget = new QStackedWidget(this);
        m_stackedViewWidget = new QStackedWidget(this);
        m_frontWidget = new QWidget(this);

        m_musicListWidget = new MusicListWidget::MusicListWidget(this);
        m_playerWidget = new PlayerWidget::PlayerWidget(this);
        m_viewWidget = new ViewWidget::ViewWidget(this);
        m_topBarWidget = new TopBarWidget::TopBarWidget(this);
        m_settingsWidget = new SettingsWidget::SettingsWidget(this);

        m_core = Core::ICore::create(this);

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
    }

    WindowManager::~WindowManager() {
    }

    void WindowManager::createConnections() {
        connect(m_playerWidget, &PlayerWidget::PlayerWidget::signalPlayToggle,
                m_core, &Core::ICore::playToggle);

        connect(m_core, &Core::ICore::signalCurrentMusicNameChanged,
                m_playerWidget, &PlayerWidget::PlayerWidget::updateMusicName);

        connect(m_core, &Core::ICore::signalCurrentMusicDurationChanged,
                m_playerWidget, &PlayerWidget::PlayerWidget::updateMusicDuration);

        connect(m_core, &Core::ICore::signalPositionChanged,
                m_playerWidget, &PlayerWidget::PlayerWidget::updateProgressBarPosition);

        connect(m_core, &Core::ICore::signalPlayingChanged,
                m_playerWidget, &PlayerWidget::PlayerWidget::setPlayButtonIcon);

        connect(m_core, &Core::ICore::signalPlayingChanged,
                m_playerWidget, &PlayerWidget::PlayerWidget::setRotationStatus);

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

        connect(m_musicListWidget, &MusicListWidget::MusicListWidget::signalMusicListButtonClicked,
                m_viewWidget, &ViewWidget::ViewWidget::setViewTitle);
    }
}
