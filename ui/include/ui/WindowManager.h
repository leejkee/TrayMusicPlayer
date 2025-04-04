//
// Created by cww on 25-4-1.
//
#pragma once
#include <ICore.h>
#include <ui/PlayerWidget.h>
#include <QWidget>


namespace Core {
    class ICore;
}

namespace UI::WindowManager {

    class WindowManager final : public QWidget {
    public:
        explicit WindowManager(QWidget *parent = Q_NULLPTR);

        ~WindowManager() override;

    private:
        PlayerWidget::PlayerWidget  *m_playerWidget;
        Core::ICore *m_core;

        void createConnections();
    };
}
