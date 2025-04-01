//
// Created by cww on 25-4-1.
//

#include <ui/WindowManager.h>
#include <ui/PlayerWidget.h>
#include <IPlayer.h>
#include <QVBoxLayout>

namespace UI::WindowManager {
    WindowManager::WindowManager(QWidget *parent) : QWidget(parent), m_player(nullptr){;
        m_playerWidget = new PlayerWidget::PlayerWidget(this);
        const auto layout = new QVBoxLayout();
        layout->addWidget(m_playerWidget);
        setLayout(layout);
    }

    WindowManager::~WindowManager() {
        delete m_player;
    }

}
