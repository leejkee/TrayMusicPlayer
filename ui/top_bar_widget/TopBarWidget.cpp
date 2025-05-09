//
// Created by cww on 25-4-10.
//
#include "TopBarWidget.h"
#include <BetterButton.h>
#include <Assets.h>
#include <QSvgWidget>
#include <QHBoxLayout>
namespace Tray::Ui {

TopBarWidget::TopBarWidget(QWidget *parent) : QWidget(parent) {

    m_svgWidget = new QSvgWidget(SvgRes::LogoSVG);
    m_svgWidget->setFixedSize(130, 25);
    m_preButton = new Panel::BetterButton(QIcon(SvgRes::BackPageSVG), this);
    m_preButton->loadStyleSheet(QssRes::BUTTON_SQUARE_QSS);
    m_settingsButton = new Panel::BetterButton(QIcon(SvgRes::SettingSVG),  this);
    m_settingsButton->loadStyleSheet(QssRes::BUTTON_SQUARE_QSS);

    const auto spacerH = new QSpacerItem(-1, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    const auto layout = new QHBoxLayout(this);
    layout->addWidget(m_svgWidget);
    layout->addItem(spacerH);
    layout->addWidget(m_preButton);
    layout->addWidget(m_settingsButton);
    setFixedHeight(40);

    connect(m_preButton, &Panel::BetterButton::clicked, this, [this]() {
        Q_EMIT signalPreButtonClicked();
    });
    connect(m_settingsButton, &Panel::BetterButton::clicked, this, [this]() {
        Q_EMIT signalSettingsButtonClicked();
    });
}

void TopBarWidget::setPreBtnShow(const bool b) {
    m_preButtonIsVisible = b;
    m_preButton->setEnabled(b);
}
}