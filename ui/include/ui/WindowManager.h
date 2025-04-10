//
// Created by cww on 25-4-1.
//
#pragma once
#include <ICore.h>
#include <QWidget>


namespace UI::MusicListWidget {
    class MusicListWidget;
}

namespace UI::ViewWidget {
    class ViewWidget;
}

namespace UI::PlayerWidget {
    class PlayerWidget;
}

namespace Core {
    class ICore;
}

class QStackedWidget;
namespace UI::WindowManager {

    class WindowManager final : public QWidget {
    public:
        explicit WindowManager(QWidget *parent = Q_NULLPTR);

        ~WindowManager() override;

    private:
        PlayerWidget::PlayerWidget  *m_playerWidget;
        ViewWidget::ViewWidget *m_viewWidget;
        MusicListWidget::MusicListWidget *m_musicListWidget;
        Core::ICore *m_core;

        QStackedWidget *m_stackedMainWidget;
        QStackedWidget *m_stackedViewWidget;



        void createConnections();
    };
}
