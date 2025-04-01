//
// Created by cww on 25-4-1.
//
#include <ui/PlayerWidget.h>
#include <ui/Assets.h>
#include <ui/BetterButton.h>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMenu>
#include <QWidgetAction>
#include <QSlider>

namespace UI::PlayerWidget {
PlayerWidget::PlayerWidget(QWidget *parent) : QWidget(parent) {
    m_pushButtonPlay = new BetterButton(QIcon(SvgRes::PlayIconSVG), this);
    m_pushButtonPlay->setFixedSize(30, 30);
    m_labelMusicFileName = new QLabel;
    m_pushButtonPre = new BetterButton(QIcon(SvgRes::PreIconSVG), this);
    m_pushButtonPre->setFixedSize(30, 30);
    m_pushButtonNext = new BetterButton(QIcon(SvgRes::NextIconSVG), this);
    m_pushButtonNext->setFixedSize(30, 30);
    setButtonVisible(true);

    // VolumeCtrl Section Begin
    m_volumeWidget = new VolumeWidget(this);

    // m_volumeWidget->loadDefaultSetting(0);
    m_pushButtonVolume = new BetterButton(QIcon(SvgRes::VolumeSVG),this);
    m_pushButtonVolume->setFixedSize(30, 30);
    m_menuVolume = new QMenu(this);
    auto *action = new QWidgetAction(this);
    action->setDefaultWidget(m_volumeWidget);
    m_menuVolume->addAction(action);
    // VolumeCtrl Section End

    m_progressWidget = new ProgressBarWidget(this);
    auto *Layout = new QVBoxLayout;
    auto *spaceH = new QSpacerItem(-1, 0, QSizePolicy::Expanding);
    Layout->addWidget(m_labelMusicFileName);
    Layout->addWidget(m_progressWidget);
    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(m_pushButtonPre);
    buttonLayout->addWidget(m_pushButtonPlay);
    buttonLayout->addWidget(m_pushButtonNext);
    buttonLayout->addItem(spaceH);
    buttonLayout->addWidget(m_pushButtonVolume);
    Layout->addLayout(buttonLayout);
    this->setLayout(Layout);
    // this->setFixedHeight(100);
    createConnections();
}

    void PlayerWidget::setSongName(const QString &songName) {
        m_labelMusicFileName->setText(songName);
}

void PlayerWidget::createConnections() {
    // update the music name label
    // connect(&PlayList::instance(), &PlayList::currentMusicNameChanged, m_labelMusicFileName, &QLabel::setText);

    // show the volume widget
    connect(m_pushButtonVolume, &QPushButton::clicked, this, &PlayerWidget::showVolumeSlider);

    // check music
    // connect(m_pushButtonNext, &QPushButton::clicked, &PlayList::instance(), &PlayList::nextMusic);
    // connect(m_pushButtonPre, &QPushButton::clicked, &PlayList::instance(), &PlayList::previousMusic);
}

void PlayerWidget::setPlayButtonIcon(const bool playStatus) {
    if (playStatus) {
        m_pushButtonPlay->setIcon(QIcon(SvgRes::PauseIconSVG));
    } else {
        m_pushButtonPlay->setIcon(QIcon(SvgRes::PlayIconSVG));
    }
}

void PlayerWidget::setVolumeCtrlButtonIcon(const int volume) {
    if (volume != 0) {
        m_pushButtonVolume->setIcon(QIcon(SvgRes::VolumeSVG));
        m_volumeWidget->m_buttonMute->setIcon(QIcon(SvgRes::VolumeSVG));
    } else {
        m_pushButtonVolume->setIcon(QIcon(SvgRes::VolumeMuteSVG));
        m_volumeWidget->m_buttonMute->setIcon(QIcon(SvgRes::VolumeMuteSVG));
    }
}

void PlayerWidget::setButtonVisible(const bool b) {
    m_pushButtonPlay->setEnabled(b);
    m_pushButtonNext->setEnabled(b);
    m_pushButtonPre->setEnabled(b);
}

QString PlayerWidget::convertSecondsToTime(const int seconds) {
    const int m = seconds / 60;
    const int s = seconds % 60;
    return QString::asprintf("%02d:%02d", m, s);
}

VolumeWidget::VolumeWidget(QWidget *parent) : QWidget(parent) {
    m_sliderV = new QSlider(this);
    m_sliderV->setRange(0, 100);
    m_labelVolume = new QLabel(this);
    m_labelVolume->setText("30%");
    m_labelVolume->setStyleSheet("font-size: 7pt;");
    m_labelVolume->setAlignment(Qt::AlignCenter);
    connect(m_sliderV, &QSlider::valueChanged, m_labelVolume, [=]() {
        m_labelVolume->setText(QString("%1%").arg(m_sliderV->value()));
    });
    m_buttonMute = new BetterButton(QIcon(SvgRes::VolumeSVG), this);
    m_buttonMute->setIconSize(QSize(10, 10));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_sliderV);
    layout->addWidget(m_labelVolume);
    layout->addWidget(m_buttonMute);
    layout->setContentsMargins(5, 5, 0, 0);
    setLayout(layout);
    this->setFixedSize(30, 110);
}

void VolumeWidget::loadDefaultSetting(const float v) const {
    m_sliderV->setValue(static_cast<int>(v * 100));
    m_labelVolume->setText(QString("%1%").arg(m_sliderV->value()));
}

void PlayerWidget::showVolumeSlider() {
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

ProgressBarWidget::ProgressBarWidget(QWidget *parent)
    : QWidget(parent)
      , m_sliderP(new QSlider(Qt::Horizontal, this))
      , m_labelLeft(new QLabel(this))
      , m_labelRight(new QLabel(this))
      , m_isUpdatingSlider(false) {
    m_labelLeft->setText("00:00");
    m_labelRight->setText("00:00");
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(m_labelLeft);
    layout->addWidget(m_sliderP);
    layout->addWidget(m_labelRight);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);
    setLayout(layout);

    // refresh firstly

    // init fun
    // updateLabelR();
}


///
/// @param position ms
void ProgressBarWidget::updateSliderPosition(const qint64 position) {
    // secure this function dismiss the signal @valueChanged
    m_isUpdatingSlider = true;
    m_sliderP->setValue(static_cast<int>(position));
    m_isUpdatingSlider = false;
}

void ProgressBarWidget::updateLabelL(const qint64 duration) {
    const int s = duration / 1000;
    const QString t = PlayerWidget::convertSecondsToTime(s);
    m_labelLeft->setText(t);
}


// update when music changed
void ProgressBarWidget::updateLabelR() {
    const int s = PlayList::instance().getCurrentMusicDuration();
    m_sliderP->setMaximum(s * 1000);
    m_labelRight->setText(PlayerWidget::convertSecondsToTime(s));
}
}
