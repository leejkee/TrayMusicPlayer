//
// Created by cww on 25-4-1.
//
#pragma once
#include <ICore.h>
#include <QWidget>


namespace UI::SettingsWidget {
    class SettingsWidget;
}


namespace UI::TopBarWidget {
    class TopBarWidget;
}


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


        void updateCurrentMusic(int index, const QString &name, int duration);

        void updatePlayingStatus(bool b);

        ~WindowManager() override;

    private:
        TopBarWidget::TopBarWidget *m_topBarWidget;
        MusicListWidget::MusicListWidget *m_musicListWidget;
        ViewWidget::ViewWidget *m_viewWidget;
        SettingsWidget::SettingsWidget *m_settingsWidget;
        PlayerWidget::PlayerWidget *m_playerWidget;

        QStackedWidget *m_stackedMainWidget;
        QStackedWidget *m_stackedViewWidget;
        QWidget *m_frontWidget;

        Core::ICore *m_core;

        void createConnections();

        void initDefaultSettings();
    };
}
