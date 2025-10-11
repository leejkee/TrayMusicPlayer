//
// Created by cww on 25-4-10.
//
#include <topbarwidget/topbarwidget.h>
#include <stylebutton/stylebutton.h>
#include <traysvg.h>
#include <trayqss.h>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>

namespace Tray::Ui
{
class TopBarWidgetPrivate
{
public:
    QLabel* m_logoLabel;
    Panel::StyleButton* m_preButton;
    QFrame* m_splitFrame;
    Panel::StyleButton* m_settingsButton;
    Panel::StyleButton* m_minimizeButton;
    Panel::StyleButton* m_maximizeButton;
    Panel::StyleButton* m_exitButton;
    bool m_preButtonIsVisible = false;
};

TopBarWidget::TopBarWidget(QWidget* parent)
    : QWidget(parent),
      d(std::make_unique<TopBarWidgetPrivate>())
{
    d->m_logoLabel = new QLabel(this);
    d->m_logoLabel->setPixmap(QPixmap(Res::LogoSVG).scaled(110
                                  , 25
                                  , Qt::IgnoreAspectRatio
                                  , Qt::SmoothTransformation));
    // d->m_logoLabel->setFixedSize(120, 25);

    d->m_preButton = new Panel::StyleButton({}
                                            , {30, 30}
                                            , Res::BackPageSVG
                                            , Res::BUTTON_SQUARE_QSS
                                            , this);
    d->m_settingsButton = new Panel::StyleButton({}
                                                 , {30, 30}
                                                 , Res::SettingBtnSVG
                                                 , Res::BUTTON_SQUARE_QSS
                                                 , this);

    d->m_minimizeButton = new Panel::StyleButton({}
                                                 , {35, 35}
                                                 , Res::MinimizeBtnSVG
                                                 , Res::BUTTON_WINDOW_QSS
                                                 , this);
    d->m_maximizeButton = new Panel::StyleButton({}
                                                 , {35, 35}
                                                 , Res::MaximizeBtnSVG
                                                 , Res::BUTTON_WINDOW_QSS
                                                 , this);
    d->m_exitButton = new Panel::StyleButton({}
                                             , {35, 35}
                                             , Res::ExitBtnSVG
                                             , Res::BUTTON_WINDOW_QSS
                                             , this);
    d->m_splitFrame = new QFrame(this);
    d->m_splitFrame->setFrameShape(QFrame::VLine);
    d->m_splitFrame->setFrameShadow(QFrame::Sunken);
    auto* spacerH = new QSpacerItem(-1
                                    , 0
                                    , QSizePolicy::Expanding
                                    , QSizePolicy::Minimum);
    auto* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(d->m_logoLabel);
    layout->addItem(spacerH);
    layout->addWidget(d->m_preButton);
    layout->addWidget(d->m_settingsButton);
    layout->addWidget(d->m_splitFrame);
    layout->addWidget(d->m_minimizeButton);
    layout->addWidget(d->m_maximizeButton);
    layout->addWidget(d->m_exitButton);
    setFixedHeight(35);

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
