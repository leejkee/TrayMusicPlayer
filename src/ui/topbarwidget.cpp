//
// Created by cww on 25-4-10.
//
#include <topbarwidget/topbarwidget.h>
#include <stylebutton/stylebutton.h>
#include <traysvg.h>
#include <trayqss.h>
#include <QSvgWidget>
#include <QHBoxLayout>

namespace Tray::Ui
{
class TopBarWidgetPrivate
{
public:
    QSvgWidget* m_svgWidget;
    Panel::StyleButton* m_preButton;
    Panel::StyleButton* m_settingsButton;
    bool m_preButtonIsVisible = false;
};

TopBarWidget::TopBarWidget(QWidget* parent)
    : QWidget(parent), d(std::make_unique<TopBarWidgetPrivate>())
{
    d->m_svgWidget = new QSvgWidget(Res::LogoSVG);
    d->m_svgWidget->setFixedSize(130, 25);
    d->m_preButton = new Panel::StyleButton({}
                                            , {30, 30}
                                            , Res::BackPageSVG
                                            , Res::BUTTON_SQUARE_QSS
                                            , this);
    d->m_settingsButton = new Panel::StyleButton({}
                                                 , {30, 30}
                                                 , Res::SettingSVG
                                                 , Res::BUTTON_SQUARE_QSS
                                                 , this);

    auto* spacerH = new QSpacerItem(-1
                                         , 0
                                         , QSizePolicy::Expanding
                                         , QSizePolicy::Minimum);
    auto* layout = new QHBoxLayout(this);
    layout->addWidget(d->m_svgWidget);
    layout->addItem(spacerH);
    layout->addWidget(d->m_preButton);
    layout->addWidget(d->m_settingsButton);
    setFixedHeight(40);

    connect(d->m_preButton
            , &Panel::StyleButton::clicked
            , this
            , [this]()
            {
                Q_EMIT signalPreButtonClicked();
            });
    connect(d->m_settingsButton
            , &Panel::StyleButton::clicked
            , this
            , [this]()
            {
                Q_EMIT signalSettingsButtonClicked();
            });
}

TopBarWidget::~TopBarWidget() = default;

void TopBarWidget::setPreBtnShow(const bool b)
{
    d->m_preButtonIsVisible = b;
    d->m_preButton->setEnabled(b);
}
}
