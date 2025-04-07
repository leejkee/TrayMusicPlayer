//
// Created by cww on 25-4-1.
//

#include <ui/WindowManager.h>
#include <ui/PlayerWidget.h>
#include <ICore.h>
#include <QVBoxLayout>


namespace UI::WindowManager {
    WindowManager::WindowManager(QWidget *parent) : QWidget(parent), m_core(nullptr){;
        m_playerWidget = new PlayerWidget::PlayerWidget(this);
        m_core = Core::ICore::create(this);
        const auto layout = new QVBoxLayout();
        layout->addWidget(m_playerWidget);
        setLayout(layout);
        createConnections();
    }

    WindowManager::~WindowManager() {
    }

    void WindowManager::createConnections() {
        connect(m_playerWidget, &PlayerWidget::PlayerWidget::playToggle, m_core, &Core::ICore::playToggle);
        connect(m_core, &Core::ICore::signalCurrentMusicNameChanged, m_playerWidget, &PlayerWidget::PlayerWidget::setSongName);
        connect(m_core, &Core::ICore::signalPlayingChanged, m_playerWidget, &PlayerWidget::PlayerWidget::setPlayButtonIcon);
        connect(m_playerWidget, &PlayerWidget::PlayerWidget::nextMusic, m_core, &Core::ICore::nextMusic);
        connect(m_playerWidget, &PlayerWidget::PlayerWidget::previousMusic, m_core, &Core::ICore::preMusic);
        connect(m_playerWidget, &PlayerWidget::PlayerWidget::set)
    }
}
