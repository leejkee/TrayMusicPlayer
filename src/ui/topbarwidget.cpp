//
// Created by cww on 25-4-10.
//
#include <QMouseEvent>
#include <topbarwidget/topbarwidget.h>
#include <traysvg.h>
#include <trayqss.h>

namespace Tray::Ui
{
TopBarWidget::TopBarWidget(QWidget* parent)
    : QWidget(parent)
{
    setObjectName("TopBarWidget");
    m_logoLabel = new QLabel(this);
    m_logoLabel->setPixmap(QPixmap(Res::LogoSVG).scaled(110
                               , 25
                               , Qt::IgnoreAspectRatio
                               , Qt::SmoothTransformation));
    m_preButton = new Panel::StyleButton({}
                                         , {30, 30}
                                         , Res::BackPageSVG
                                         , Res::BUTTON_SQUARE_QSS
                                         , this);
    m_settingsButton = new Panel::StyleButton({}
                                              , {30, 30}
                                              , Res::SettingBtnSVG
                                              , Res::BUTTON_SQUARE_QSS
                                              , this);
    m_minimizeButton = new Panel::StyleButton({}
                                              , {ButtonWidth, BarHeight}
                                              , Res::MinimizeBtnSVG
                                              , Res::BUTTON_WINDOW_QSS
                                              , this);
    m_maximizeButton = new Panel::StyleButton({}
                                              , {ButtonWidth, BarHeight}
                                              , Res::MaximizeBtnSVG
                                              , Res::BUTTON_WINDOW_QSS
                                              , this);
    m_exitButton = new Panel::StyleButton({}
                                          , {ButtonWidth, BarHeight}
                                          , Res::ExitBtnSVG
                                          , Res::BUTTON_WINDOW_QSS
                                          , this);
    m_splitFrame = new QFrame(this);
    m_splitFrame->setFrameShape(QFrame::VLine);
    m_splitFrame->setFrameShadow(QFrame::Sunken);
    auto* spacerH = new QSpacerItem(-1
                                    , 0
                                    , QSizePolicy::Expanding
                                    , QSizePolicy::Minimum);
    auto* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_logoLabel);
    layout->addItem(spacerH);
    layout->addWidget(m_preButton);
    layout->addWidget(m_settingsButton);
    layout->addWidget(m_splitFrame);
    layout->addWidget(m_minimizeButton);
    layout->addWidget(m_maximizeButton);
    layout->addWidget(m_exitButton);
    setFixedHeight(BarHeight);

    connect(m_preButton
            , &Panel::StyleButton::clicked
            , this
            , [this]()
            {
                Q_EMIT signalPreButtonClicked();
            });
    connect(m_settingsButton
            , &Panel::StyleButton::clicked
            , this
            , [this]()
            {
                Q_EMIT signalSettingsButtonClicked();
            });

    connect(m_minimizeButton
            , &QPushButton::clicked
            , this
            , &TopBarWidget::handleWindowMinimize);

    connect(m_maximizeButton
            , &QPushButton::clicked
            , this
            , &TopBarWidget::handleWindowMaximize);

    connect(m_exitButton
            , &QPushButton::clicked
            , this
            , [this]
            {
                Q_EMIT signalExitButtonClicked();
            });
}

TopBarWidget::~TopBarWidget() = default;

void TopBarWidget::setRadius(const int radius)
{
    m_logoLabel->
            setStyleSheet(QString("border-top-left-radius: %1px;").arg(radius));
    m_exitButton->
            setStyleSheet(QString(Res::BUTTON_RADIUS_TEMPLATE_QSS).arg(radius));
    this->setStyleSheet(
                        QString("QWidget#%1 { border-top-left-radius: %2px; border-top-right-radius: %2px; } ")
                       .arg(objectName()).arg(radius)
                       );
}

void TopBarWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isDragging && (event->buttons() & Qt::LeftButton))
    {
        if (QWidget* mainW = this->window())
        {
            if (mainW->isMaximized())
            {
                const int normalWidth = mainW->normalGeometry().width();

                const QPoint globalMousePos = event->globalPosition().toPoint();

                const float widthRatio = (mainW->width() > 0)
                                         ? static_cast<float>(globalMousePos.
                                             x()) / mainW->width()
                                         : 0.5f;

                const int newX = globalMousePos.x() - normalWidth * widthRatio;
                const int newY = globalMousePos.y() - event->position().y();

                mainW->showNormal();
                mainW->move(newX, newY);

                m_dragPos = globalMousePos;
            }
            else
            {
                const auto mousePos = event->globalPosition().toPoint();
                const auto delta = mousePos - m_dragPos;
                mainW->move(mainW->pos() + delta);
                m_dragPos = event->globalPosition().toPoint();
            }
        }
        event->accept();
    }
}

void TopBarWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton)
    {
        return;
    }

    if (m_minimizeButton && m_minimizeButton->geometry().contains(event->pos()))
    {
        return;
    }

    if (m_maximizeButton && m_maximizeButton->geometry().contains(event->pos()))
    {
        return;
    }

    if (m_exitButton && m_exitButton->geometry().contains(event->pos()))
    {
        return;
    }

    if (m_settingsButton && m_settingsButton->geometry().contains(event->pos()))
    {
        return;
    }

    if (m_preButton && m_preButton->geometry().contains(event->pos()))
    {
        return;
    }

    m_isDragging = true;
    m_dragPos = event->globalPosition().toPoint();
    event->accept();
}

void TopBarWidget::mouseReleaseEvent(QMouseEvent* event)
{
    m_isDragging = false;
    QWidget::mouseReleaseEvent(event);
}

void TopBarWidget::handleWindowMinimize() const
{
    if (auto* mainW = this->window(); !mainW->isMinimized())
    {
        mainW->showMinimized();
    }
}

void TopBarWidget::handleWindowMaximize() const
{
    if (auto* mainW = this->window(); !mainW->isMaximized())
    {
        mainW->showMaximized();
    }
    else
    {
        mainW->showNormal();
    }
}
}
