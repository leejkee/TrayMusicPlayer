//
// Created by cww on 25-4-1.
//
#include <ui/PlayerWidget.h>
#include <ui/Assets.h>
#include <panel/BetterButton.h>
#include <panel/ProgressBar.h>
#include <panel/VolumeController.h>
#include <panel/RotatingLabel.h>

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMenu>
#include <QWidgetAction>


namespace UI::PlayerWidget {
    PlayerWidget::PlayerWidget(QWidget *parent) : QWidget(parent) {
        initWidget();
        createConnections();
    }

    void PlayerWidget::initWidget() {
        initLeftLayout();
        initCenterLayout();
        initVolumeControl();
        initMainLayout();
    }

    void PlayerWidget::initLeftLayout() {
        m_labelMusicName = new QLabel(this);
        m_labelMusicName->setAlignment(Qt::AlignVCenter);
        m_labelMusicName->setFixedSize(WIDTH_TITLE_LABEL, HEIGHT_PLAYER_WIDGET - 10);

        const QPixmap pixmap(SvgRes::TrayIconSVG);
        m_labelLogo = new Panel::RotatingLabel(pixmap, QSize(CIRCLE_LOGO_SIZE, CIRCLE_LOGO_SIZE));
        m_labelLogo->initRotation(0, 360, 15000, -1);
        m_labelLogo->setLabelMode(Panel::RotatingLabel::NoRotating);

        m_leftLayout = new QHBoxLayout;
        m_leftLayout->addWidget(m_labelLogo);
        m_leftLayout->addWidget(m_labelMusicName);
    }

    void PlayerWidget::initCenterLayout() {
        m_pushButtonPlay = new Panel::BetterButton(QIcon(SvgRes::PlayIconSVG), this);
        m_pushButtonPre = new Panel::BetterButton(QIcon(SvgRes::PreIconSVG), this);
        m_pushButtonNext = new Panel::BetterButton(QIcon(SvgRes::NextIconSVG), this);
        m_progressWidget = new Panel::ProgressBar(this);

        const auto buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(m_pushButtonPre);
        buttonLayout->addWidget(m_pushButtonPlay);
        buttonLayout->addWidget(m_pushButtonNext);

        m_centerLayout = new QVBoxLayout;
        m_centerLayout->addWidget(m_progressWidget);
        m_centerLayout->addLayout(buttonLayout);
        m_centerLayout->setContentsMargins(0, 0, 0, 0);
    }

    void PlayerWidget::initVolumeControl() {
        m_volumeController = new Panel::VolumeController(this);
        m_pushButtonVolume = new Panel::BetterButton(QIcon(SvgRes::VolumeBtnSVG), this);
        m_pushButtonVolume->loadStyleSheet(QssRes::BUTTON_VOLUME_QSS);

        m_menuVolume = new QMenu(this);
        auto *action = new QWidgetAction(this);
        action->setDefaultWidget(m_volumeController);
        m_menuVolume->addAction(action);
    }

    void PlayerWidget::initMainLayout() {
        const auto mainLayout = new QHBoxLayout;
        const auto spaceH1 = new QSpacerItem(-1, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
        const auto spaceH2 = new QSpacerItem(-1, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        mainLayout->addLayout(m_leftLayout);
        mainLayout->addSpacerItem(spaceH1);
        mainLayout->addLayout(m_centerLayout);
        mainLayout->addSpacerItem(spaceH2);
        mainLayout->addWidget(m_pushButtonVolume);

        setLayout(mainLayout);
    }


    void PlayerWidget::setSongName(const QString &songName) {
        const auto artist = songName.right(songName.length() - songName.indexOf("-") - 1).trimmed();
        const auto name = songName.left(songName.indexOf("-")).trimmed();
        const QString formattedText = QString("%1<br><font size='-1'>%2</font>")
                .arg(name, artist);
        m_labelMusicName->setText(formattedText);
    }

    void PlayerWidget::createConnections() {
        // show the volume widget
        connect(m_pushButtonVolume, &QPushButton::clicked, this, &PlayerWidget::showVolumeSlider);

        connect(m_pushButtonPlay, &QPushButton::clicked, this, [this]() {
            Q_EMIT signalPlayToggle();
        });
        connect(m_volumeController, &Panel::VolumeController::signalSetValue, this, [this](const int value) {
            Q_EMIT signalSetVolume(value);
        });

        connect(m_pushButtonNext, &QPushButton::clicked, this, [this]() {
            Q_EMIT signalNextMusic();
        });

        connect(m_pushButtonPre, &QPushButton::clicked, this, [this]() {
            Q_EMIT signalPreviousMusic();
        });
    }

    void PlayerWidget::setPlayButtonIcon(const bool playStatus) {
        if (playStatus) {
            m_pushButtonPlay->setIcon(QIcon(SvgRes::PauseIconSVG));
        } else {
            m_pushButtonPlay->setIcon(QIcon(SvgRes::PlayIconSVG));
        }
    }

    void PlayerWidget::setRotationStatus(const bool b) {
        if (b) {
            m_labelLogo->setLabelMode(Panel::RotatingLabel::Rotating);
        } else {
            m_labelLogo->setLabelMode(Panel::RotatingLabel::NoRotating);
        }
    }

    void PlayerWidget::setVolumeCtrlButtonIcon(const bool isMuted) {
        m_pushButtonVolume->setIcon(QIcon(isMuted ? SvgRes::VolumeMuteSVG : SvgRes::VolumeBtnSVG));
        m_volumeController->setVolumeButtonIcon(isMuted);
    }

    void PlayerWidget::showVolumeSlider() const {
        if (m_menuVolume->isVisible()) {
            m_menuVolume->hide();
            qDebug() << "Menu hide";
        } else {
            QPoint pos = m_pushButtonVolume->mapToGlobal(QPoint(0, 0));
            pos.setY(pos.y() - m_menuVolume->sizeHint().height());
            pos.setX(pos.x() - 5);
            m_menuVolume->popup(pos);
            qDebug() << "Menu show";
        }
    }
}
