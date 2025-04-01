//
// Created by cww on 25-4-1.
//
#pragma once
#include <ui/PlayerWidget.h>
#include <QWidget>


namespace Core::Player {
    class IPlayer;
}

namespace UI::WindowManager {

    class WindowManager final : public QWidget {
    public:
        explicit WindowManager(QWidget *parent = Q_NULLPTR);

        ~WindowManager();

    private:
        PlayerWidget::PlayerWidget  *m_playerWidget;
        Core::Player::IPlayer *m_player;
    };
}
