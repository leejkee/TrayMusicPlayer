//
// Created by cww on 25-4-10.
//
#include <topbarwidget.h>
#include <../widgets/include/stylebutton/stylebutton.h>
#include <traysvg.h>
#include <trayqss.h>
#include <QSvgWidget>
#include <QHBoxLayout>

namespace Tray::Ui {

TopBarWidget::TopBarWidget(QWidget *parent) : QWidget(parent) {

    m_svgWidget = new QSvgWidget(Res::LogoSVG);
    m_svgWidget->setFixedSize(130, 25);
    m_preButton = new Panel::StyleButton({{}, Res::BackPageSVG, Res::BUTTON_SQUARE_QSS, 30, 30}, this);
    m_settingsButton = new Panel::StyleButton({{}, Res::SettingSVG, Res::BUTTON_SQUARE_QSS, 30, 30},  this);

    const auto spacerH = new QSpacerItem(-1, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    const auto layout = new QHBoxLayout(this);
    layout->addWidget(m_svgWidget);
    layout->addItem(spacerH);
    layout->addWidget(m_preButton);
    layout->addWidget(m_settingsButton);
    setFixedHeight(40);

    connect(m_preButton, &Panel::StyleButton::clicked, this, [this]() {
        Q_EMIT signalPreButtonClicked();
    });
    connect(m_settingsButton, &Panel::StyleButton::clicked, this, [this]() {
        Q_EMIT signalSettingsButtonClicked();
    });
}

void TopBarWidget::setPreBtnShow(const bool b) {
    m_preButtonIsVisible = b;
    m_preButton->setEnabled(b);
}
}