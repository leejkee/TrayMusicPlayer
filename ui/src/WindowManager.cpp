//
// Created by cww on 25-4-1.
//

#include <ui/WindowManager.h>
#include <ui/PlayerWidget.h>
#include <ui/ViewWidget.h>
#include <ui/MusicListWidget.h>
#include <ICore.h>
#include <QVBoxLayout>
#include <QStackedWidget>

namespace UI::WindowManager {
    WindowManager::WindowManager(QWidget *parent) : QWidget(parent), m_core(nullptr){
        m_stackedMainWidget = new QStackedWidget(this);
        m_stackedViewWidget = new QStackedWidget(this);

        m_playerWidget = new PlayerWidget::PlayerWidget(this);
        m_viewWidget = new ViewWidget::ViewWidget(this);
        m_musicListWidget = new MusicListWidget::MusicListWidget(this);
        m_core = Core::ICore::create(this);

        const auto mainLayout = new QVBoxLayout(this);
        mainLayout->addWidget(m_viewWidget);
        mainLayout->addWidget(m_playerWidget);
        createConnections();
        m_core->initDefaultSettings();
    }

    WindowManager::~WindowManager() {
    }

    void WindowManager::createConnections() {
        connect(m_playerWidget, &PlayerWidget::PlayerWidget::signalPlayToggle, m_core, &Core::ICore::playToggle);
        connect(m_core, &Core::ICore::signalCurrentMusicNameChanged, m_playerWidget, &PlayerWidget::PlayerWidget::setSongName);
        connect(m_core, &Core::ICore::signalPlayingChanged, m_playerWidget, &PlayerWidget::PlayerWidget::setPlayButtonIcon);
        connect(m_core, &Core::ICore::signalPlayingChanged, m_playerWidget, &PlayerWidget::PlayerWidget::setRotationStatus);
        connect(m_playerWidget, &PlayerWidget::PlayerWidget::signalNextMusic, m_core, &Core::ICore::nextMusic);
        connect(m_playerWidget, &PlayerWidget::PlayerWidget::signalPreviousMusic, m_core, &Core::ICore::preMusic);
        connect(m_playerWidget, &PlayerWidget::PlayerWidget::signalSetVolume, m_core, &Core::ICore::setVolume);
        connect(m_core, &Core::ICore::signalIsMuted, m_playerWidget, &PlayerWidget::PlayerWidget::setVolumeCtrlButtonIcon);
    }
}
