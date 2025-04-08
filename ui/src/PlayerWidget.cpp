//
// Created by cww on 25-4-1.
//
#include <ui/PlayerWidget.h>
#include <ui/Assets.h>
#include <panel/BetterButton.h>
#include <panel/ProgressBar.h>
#include <panel/VolumeController.h>

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMenu>
#include <QWidgetAction>


namespace UI::PlayerWidget {
    PlayerWidget::PlayerWidget(QWidget *parent) : QWidget(parent) {
        initLeft();
        initCenter();
        initRight();

        const auto layout = new QHBoxLayout(this);
        layout->addLayout(m_leftLayout);
        layout->addLayout(m_centerLayout);
        layout->addWidget(m_pushButtonVolume);
        createConnections();
    }

    void PlayerWidget::initLeft() {
        m_labelMusicName = new QLabel(this);
        m_labelLogo = new QLabel(this);
        m_labelLogo->setPixmap(SvgRes::TrayIconSVG);
        m_leftLayout = new QHBoxLayout;
        m_leftLayout->setSpacing(0);
        m_leftLayout->setContentsMargins(0, 0, 0, 0);
        m_leftLayout->addWidget(m_labelLogo);
        m_leftLayout->addWidget(m_labelMusicName);
    }

    void PlayerWidget::initCenter() {
        m_pushButtonPlay = new Panel::BetterButton(QIcon(SvgRes::PlayIconSVG), this);
        m_pushButtonPre = new Panel::BetterButton(QIcon(SvgRes::PreIconSVG), this);
        m_pushButtonNext = new Panel::BetterButton(QIcon(SvgRes::NextIconSVG), this);
        m_progressWidget = new Panel::ProgressBar(this);
        m_centerLayout = new QVBoxLayout;
        const auto buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(m_pushButtonPre);
        buttonLayout->addWidget(m_pushButtonPlay);
        buttonLayout->addWidget(m_pushButtonNext);

        m_centerLayout->addWidget(m_progressWidget);
        m_centerLayout->addLayout(buttonLayout);
    }

    void PlayerWidget::initRight() {

        // VolumeCtrl Section Begin
        m_volumeController = new Panel::VolumeController(this);
        m_pushButtonVolume = new Panel::BetterButton(QIcon(SvgRes::VolumeSVG), this);
        m_menuVolume = new QMenu(this);
        auto *action = new QWidgetAction(this);
        action->setDefaultWidget(m_volumeController);
        m_menuVolume->addAction(action);
        // VolumeCtrl Section End
    }

    void PlayerWidget::setSongName(const QString &songName) const {
        m_labelMusicName->setText(songName);
    }

    void PlayerWidget::createConnections() {
        // update the music name label
        // connect(&PlayList::instance(), &PlayList::currentMusicNameChanged, m_labelMusicFileName, &QLabel::setText);

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

        // check music
        // connect(m_pushButtonNext, &QPushButton::clicked, &PlayList::instance(), &PlayList::nextMusic);
        // connect(m_pushButtonPre, &QPushButton::clicked, &PlayList::instance(), &PlayList::previousMusic);
    }

    void PlayerWidget::setPlayButtonIcon(const bool playStatus) const {
        if (playStatus) {
            m_pushButtonPlay->setIcon(QIcon(SvgRes::PauseIconSVG));
        } else {
            m_pushButtonPlay->setIcon(QIcon(SvgRes::PlayIconSVG));
        }
    }

    void PlayerWidget::setVolumeCtrlButtonIcon(const bool b) const {
        m_pushButtonVolume->setIcon(QIcon(b ? SvgRes::VolumeMuteSVG : SvgRes::VolumeSVG));
        m_volumeController->setVolumeButtonIcon(b);
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
