//
// Created by cww on 25-4-10.
//
#include <ui/MusicListWidget.h>
#include <panel/BetterButton.h>
#include <ui/Assets.h>

#include <QHBoxLayout>
#include <QScrollArea>
#include <QInputDialog>

namespace UI::MusicListWidget {
MusicListWidget::MusicListWidget(QWidget *parent)
    : QWidget(parent) {
    m_localListButton = new Panel::BetterButton(User::LOCAL_LIST_KEY, this);

    m_expandButton = new Panel::BetterButton(QIcon(SvgRes::UpSVG), this, User::EXPAND_BTN_TEXT);
    m_expandButton->loadStyleSheet(QssRes::BUTTON_EXPAND_QSS);

    m_addButton = new Panel::BetterButton(QIcon(SvgRes::AddSVG), this);
    m_addButton->loadStyleSheet(QssRes::BUTTON_ADD_QSS);

    const auto buttonLayout = new QHBoxLayout;
    const auto spaceH = new QSpacerItem(-1, 0, QSizePolicy::Expanding);
    buttonLayout->addWidget(m_expandButton);

    buttonLayout->addWidget(m_addButton);
    buttonLayout->addItem(spaceH);

    m_buttonWidget = new QWidget(this);
    m_buttonLayout = new QVBoxLayout;
    m_buttonLayout->setSpacing(0);
    m_buttonLayout->setContentsMargins(0, 0, 0, 0);
    m_buttonWidget->setLayout(m_buttonLayout);

    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);

    const auto m_mainLayout = new QVBoxLayout;
    const auto spaceV = new QSpacerItem(0, -1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);
    m_mainLayout->addWidget(m_localListButton);
    m_mainLayout->addItem(buttonLayout);
    m_mainLayout->addWidget(m_buttonWidget);
    m_mainLayout->addItem(spaceV);

    const auto mainWidget = new QWidget(this);
    mainWidget->setLayout(m_mainLayout);
    m_scrollArea->setWidget(mainWidget);
    // m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    const auto layout = new QVBoxLayout;
    layout->addWidget(m_scrollArea);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    setLayout(layout);
    // setFixedWidth(110);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
    // setMaximumWidth(150);
    createConnections();
    initUserListButtons();
}

void MusicListWidget::createConnections() {
    connect(m_localListButton, &Panel::BetterButton::signalButtonClicked, this, &MusicListWidget::handleMusicButtonClicked);
    connect(m_expandButton, &QPushButton::clicked, this, &MusicListWidget::toggleExpand);
    connect(m_addButton, &QPushButton::clicked, this, &MusicListWidget::addButton);
    // connect(this, &MusicListWidget::signalMusicListButtonAdded, &Settings::instance(), &Settings::addUserMusicList);
}


void MusicListWidget::toggleExpand() const {
    const bool isV = m_buttonWidget->isVisible();
    m_buttonWidget->setVisible(!isV);
    if (!isV) {
        m_expandButton->setIcon(QIcon(SvgRes::UpSVG));
    } else {
        m_expandButton->setIcon(QIcon(SvgRes::DownSVG));
    }
}


void MusicListWidget::addButton() {
    qDebug() << "buttonWidget addButton";
    bool ok;
    const QString playlistName = QInputDialog::getText(this,
                                                       "New a music list",
                                                       "Music list name:",
                                                       QLineEdit::Normal,
                                                       "",
                                                       &ok);
    if (ok && !playlistName.isEmpty() && !playlistName.begin()->isDigit()) {
        qDebug() << "playlist: [" << playlistName << "] added.";
        newButton(playlistName);
        Q_EMIT signalMusicListButtonAdded(playlistName);
    }
}

void MusicListWidget::newButton(const QString &playlistName) {
    auto *button = new Panel::BetterButton(playlistName, this);
    m_buttonLayout->addWidget(button);
    // button->setMusicListFromSongs(DBManager::instance().getMusicList(playlistName));
    // if no key, inserts a default-constructed value into the map
    connect(button, &Panel::BetterButton::signalButtonClicked, this, &MusicListWidget::handleMusicButtonClicked);
}


void MusicListWidget::initUserListButtons() {

    // const auto listName = Settings::instance().getUserMusicList();
    // for (const auto &name: listName) {
    //     newButton(name);
    // }
}


void MusicListWidget::handleMusicButtonClicked(const QString &name) {
    qDebug() << "buttonWidget handleMusicButtonClicked";
    Q_EMIT signalMusicListButtonClicked(name);
}

}