//
// Created by cww on 25-4-1.
//
#include <playerwidget/playerwidget.h>
#include <traysvg.h>
#include <trayqss.h>
#include <stylebutton/stylebutton.h>
#include <progressbar/progressbar.h>
#include <volumecontroller/volumecontroller.h>
#include <rotatinglabel/rotatinglabel.h>
#include <marqueelabel/marqueelabel.h>

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMenu>
#include <QWidgetAction>


namespace Tray::Ui
{
class PlayerWidgetPrivate
{
public:
    Panel::MarqueeLabel* m_labelMusicName;
    Panel::MarqueeLabel* m_labelArtistName;
    Panel::RotatingLabel* m_labelLogo;
    QHBoxLayout* m_leftLayout;

    // center
    Panel::StyleButton* m_pushButtonPlay;
    Panel::StyleButton* m_pushButtonPre;
    Panel::StyleButton* m_pushButtonNext;
    Panel::StyleButton* m_checkPlayMode;
    Panel::ProgressBar* m_progressWidget;
    QVBoxLayout* m_centerLayout;

    // right
    Panel::VolumeController* m_volumeController;
    Panel::StyleButton* m_pushButtonVolume;
    QMenu* m_menuVolume;
};

PlayerWidget::PlayerWidget(QWidget* parent)
    : QWidget(parent)
{
    initLeftLayout();
    initCenterLayout();
    initVolumeControl();
    initMainLayout();
    createConnections();
}

PlayerWidget::~PlayerWidget() = default;

void PlayerWidget::initLeftLayout()
{
    d->m_labelMusicName = new Panel::MarqueeLabel(this);
    d->m_labelMusicName->setAlignment(Qt::AlignVCenter);
    d->m_labelMusicName->setFixedSize(WIDTH_TITLE_LABEL, HEIGHT_PLAYER_WIDGET);

    const QPixmap pixmap(Res::TrayIconSVG);
    d->m_labelLogo = new Panel::RotatingLabel(pixmap
                                              , QSize(CIRCLE_LOGO_SIZE
                                                  , CIRCLE_LOGO_SIZE));
    d->m_labelLogo->initRotation(0, 360, 15000, -1);
    d->m_labelLogo->setLabelMode(Panel::RotatingLabel::NoRotating);
    d->m_labelLogo->setFixedSize(CIRCLE_LOGO_SIZE + 5, CIRCLE_LOGO_SIZE + 5);

    d->m_leftLayout = new QHBoxLayout;
    d->m_leftLayout->addWidget(d->m_labelLogo);
    d->m_leftLayout->addWidget(d->m_labelMusicName);
}

void PlayerWidget::initCenterLayout()
{
    d->m_pushButtonPlay = new Panel::StyleButton({}
                                                 , {30, 30}
                                                 , Res::PlayIconSVG
                                                 , Res::BUTTON_NORMAL_QSS
                                                 , this);
    d->m_pushButtonPre = new Panel::StyleButton({}
                                                , {30, 30}
                                                , Res::PreIconSVG
                                                , Res::BUTTON_NORMAL_QSS
                                                , this);
    d->m_pushButtonNext = new Panel::StyleButton({}
                                                 , {30, 30}
                                                 , Res::NextIconSVG
                                                 , Res::BUTTON_NORMAL_QSS
                                                 , this);
    d->m_checkPlayMode = new Panel::StyleButton({}
                                                , {30, 30}
                                                , Res::SequentialModeSVG
                                                , Res::BUTTON_NORMAL_QSS
                                                , this);
    d->m_progressWidget = new Panel::ProgressBar(this);

    const auto buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(d->m_pushButtonPre);
    buttonLayout->addWidget(d->m_pushButtonPlay);
    buttonLayout->addWidget(d->m_pushButtonNext);
    buttonLayout->addWidget(d->m_checkPlayMode);

    d->m_centerLayout = new QVBoxLayout;
    d->m_centerLayout->addWidget(d->m_progressWidget);
    d->m_centerLayout->addLayout(buttonLayout);
    d->m_centerLayout->setContentsMargins(0, 0, 0, 0);
}

void PlayerWidget::initVolumeControl()
{
    d->m_volumeController = new Panel::VolumeController(this);
    d->m_pushButtonVolume = new Panel::StyleButton({}
                                                   , {30, 30}
                                                   , Res::VolumeBtnSVG
                                                   , Res::BUTTON_VOLUME_QSS
                                                   , this);

    d->m_menuVolume = new QMenu(this);
    auto* action = new QWidgetAction(this);
    action->setDefaultWidget(d->m_volumeController);
    d->m_menuVolume->addAction(action);
    // d->m_menuVolume->setStyleSheet("QMenu { background-color: white; border: 1px solid gray; border-radius: 6px; }");
    d->m_menuVolume->setStyleSheet(Res::readQss(Res::VOLUME_MENU_QSS));
}

void PlayerWidget::initMainLayout()
{
    const auto mainLayout = new QHBoxLayout;
    const auto spaceH1 = new QSpacerItem(-1
                                         , 0
                                         , QSizePolicy::Expanding
                                         , QSizePolicy::Minimum);
    const auto spaceH2 = new QSpacerItem(-1
                                         , 0
                                         , QSizePolicy::Expanding
                                         , QSizePolicy::Minimum);

    mainLayout->addLayout(d->m_leftLayout);
    mainLayout->addSpacerItem(spaceH1);
    mainLayout->addLayout(d->m_centerLayout);
    mainLayout->addSpacerItem(spaceH2);
    mainLayout->addWidget(d->m_pushButtonVolume);

    setLayout(mainLayout);
}

void PlayerWidget::updateMusicName(const QString& songName)
{
    // update name label
    const auto artist = songName.
                        right(songName.length() - songName.indexOf("-") - 1).
                        trimmed();
    const auto name = songName.left(songName.indexOf("-")).trimmed();
    const QString formattedText = QString(
                                          "<span style='font-size:16px; color:black;'>%1 - </span> "
                                          "<span style='font-size:14px; color:gray; font-weight:bold;'>%2</span>"
                                         ).arg(name, artist);
    d->m_labelMusicName->setMarqueeText(formattedText);
}

void PlayerWidget::updateMusicDuration(const int s)
{
    d->m_progressWidget->updateLabelR(s);
}


void PlayerWidget::createConnections()
{
    // show the volume widget
    connect(d->m_pushButtonVolume
            , &QPushButton::clicked
            , this
            , &PlayerWidget::showVolumeSlider);

    connect(d->m_pushButtonPlay
            , &QPushButton::clicked
            , this
            , [this]()
            {
                Q_EMIT signalPlayToggle();
            });
    connect(d->m_volumeController
            , &Panel::VolumeController::signalSetValue
            , this
            , [this](const int value)
            {
                Q_EMIT signalSetVolume(value);
            });

    connect(d->m_pushButtonNext
            , &QPushButton::clicked
            , this
            , [this]()
            {
                Q_EMIT signalNextMusic();
            });

    connect(d->m_pushButtonPre
            , &QPushButton::clicked
            , this
            , [this]()
            {
                Q_EMIT signalPreviousMusic();
            });

    connect(d->m_progressWidget
            , &Panel::ProgressBar::signalProgressValueChanged
            , this
            , [this](const int value)
            {
                Q_EMIT signalSetMusicPosition(value);
            });

    connect(d->m_checkPlayMode
            , &QPushButton::clicked
            , this
            , [this]()
            {
                Q_EMIT signalPlayModeChanged();
            });

    connect(d->m_volumeController
            , &Panel::VolumeController::signalSetMute
            , this
            , [this]() { Q_EMIT signalSetMute(); });
}

void PlayerWidget::setPlayButtonIcon(const bool playStatus)
{
    if (playStatus)
    {
        d->m_pushButtonPlay->setIcon(QIcon(Res::PauseIconSVG));
    }
    else
    {
        d->m_pushButtonPlay->setIcon(QIcon(Res::PlayIconSVG));
    }
}

void PlayerWidget::setRotationStatus(const bool b)
{
    if (b)
    {
        d->m_labelLogo->setLabelMode(Panel::RotatingLabel::Rotating);
    }
    else
    {
        d->m_labelLogo->setLabelMode(Panel::RotatingLabel::NoRotating);
    }
}

void PlayerWidget::updatePlayModeIcon(const int mode)
{
    switch (mode)
    {
    case 0:
        d->m_checkPlayMode->setIcon(QIcon(Res::SequentialModeSVG));
        break;
    case 1:
        d->m_checkPlayMode->setIcon(QIcon(Res::LoopOneModeSVG));
        break;
    case 2:
        d->m_checkPlayMode->setIcon(QIcon(Res::LoopAllModeSVG));
        break;
    case 3:
        d->m_checkPlayMode->setIcon(QIcon(Res::ShuffleModeSVG));
        break;
    default:
        break;
    }
}

void PlayerWidget::setVolumeCtrlButtonIcon(const bool isMuted)
{
    d->m_pushButtonVolume->setIcon(QIcon(isMuted
                                         ? Res::VolumeMuteSVG
                                         : Res::VolumeBtnSVG));
    d->m_volumeController->setVolumeButtonIcon(isMuted);
}

void PlayerWidget::showVolumeSlider() const
{
    if (d->m_menuVolume->isVisible())
    {
        d->m_menuVolume->hide();

        qDebug() << "Menu hide";
    }
    else
    {
        // coordinate of the top-left pixel widget
        const QPoint pos = d->m_pushButtonVolume->mapToGlobal(QPoint(
         (d->m_pushButtonVolume->width() - d->m_menuVolume->sizeHint().
                                              width()) / 2
         , -d->m_menuVolume->sizeHint().height()));
        d->m_menuVolume->popup(pos);
        qDebug() << "Menu show";
    }
}

void PlayerWidget::updateProgressBarPosition(const qint64 position)
{
    d->m_progressWidget->updateSliderPosition(position);
}

void PlayerWidget::setSliderVolumeValue(const unsigned v)
{
    d->m_volumeController->setSliderVolumeValue(v);
}

void PlayerWidget::updateMuteIcon(const bool isMuted)
{
    d->m_volumeController->setVolumeButtonIcon(isMuted);
}
}
