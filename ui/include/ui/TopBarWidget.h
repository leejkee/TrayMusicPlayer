//
// Created by cww on 25-4-10.
//
#pragma once
#include <QWidget>


namespace UI::Panel {
    class BetterButton;
}


class QPushButton;
class QSvgWidget;


namespace UI::TopBarWidget {
    class TopBarWidget final : public QWidget {
    public:
        explicit TopBarWidget(QWidget *parent = nullptr);

        void setPreBtnShow(bool b);

        QSvgWidget *m_svgWidget;
        Panel::BetterButton *m_preButton;
        Panel::BetterButton *m_settingsButton;

        bool m_preButtonIsVisible = false;
    };
}
