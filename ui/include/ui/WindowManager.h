//
// Created by cww on 25-4-1.
//
#pragma once
#include <QWidget>

#include "PlayerWidget.h"


namespace Core::Player {
    class IPlayer;
}


namespace UI::WindowManager {

    class PlayerWidget::PlayerWidget;

    class WindowManager final : public QWidget {
    public:
        explicit WindowManager(QWidget *parent = Q_NULLPTR);

        ~WindowManager();

    private:
        std::shared_ptr<PlayerWidget::PlayerWidget> m_playerWidget;
        std::shared_ptr<Core::Player::IPlayer> m_player;
    };
}
