//
// Created by cww on 25-4-1.
//

#include <../include/ui/WindowManager.h>
#include <../include/ui/PlayerWidget.h>
#include <IPlayer.h>

namespace UI::WindowManager {
    WindowManager::WindowManager(QWidget *parent) : QWidget(parent) {
        m_player = std::make_shared<Core::Player::IPlayer>();
        m_playerWidget = std::make_shared<PlayerWidget::PlayerWidget>();
    }
}
