//
// Created by cww on 25-4-10.
//
#include <musiclistwidget.h>
#include <uitools.h>
#include <betterbutton.h>
#include <traysvg.h>
#include <trayqss.h>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QInputDialog>
#include <QMenu>

namespace Tray::Ui {
    MusicListWidget::MusicListWidget(QWidget *parent)
        : QWidget(parent) {
        m_localListButton = new Panel::BetterButton({LOCAL_LIST_KEY, Res::MusicListSVG, Res::BUTTON_NORMAL_QSS, 30, 0 }, this);
        m_expandButton = new Panel::BetterButton({EXPAND_BTN_TEXT, Res::UpSVG, Res::BUTTON_EXPAND_QSS, 30, 60}, this);
        m_addButton = new Panel::BetterButton({{}, Res::AddSVG, Res::BUTTON_ADD_QSS, 0, 0}, this);
        const auto buttonLayout = new QHBoxLayout;
        const auto spaceH = new QSpacerItem(-1, 0, QSizePolicy::Expanding);
        buttonLayout->addWidget(m_expandButton);
        buttonLayout->addItem(spaceH);
        buttonLayout->addWidget(m_addButton);
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
        setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        createConnections();
    }

    void MusicListWidget::createConnections() {
        connect(m_localListButton, &Panel::BetterButton::signalButtonClicked, this,
                &MusicListWidget::handleMusicButtonClicked);
        connect(m_expandButton, &QPushButton::clicked, this, &MusicListWidget::toggleExpand);
        connect(m_addButton, &QPushButton::clicked, this, &MusicListWidget::newUserPlaylist);
    }


    void MusicListWidget::toggleExpand() const {
        const bool isV = m_buttonWidget->isVisible();
        m_buttonWidget->setVisible(!isV);
        if (!isV) {
            m_expandButton->setIcon(QIcon(Res::UpSVG));
        } else {
            m_expandButton->setIcon(QIcon(Res::DownSVG));
        }
    }

    void MusicListWidget::newUserPlaylist() {
        bool ok;
        const QString playlistName = QInputDialog::getText(this,
                                                           "New a music list",
                                                           "Music list name:",
                                                           QLineEdit::Normal,
                                                           "",
                                                           &ok);
        if (ok && !playlistName.isEmpty() && !playlistName.begin()->isDigit()) {
            qDebug() << "MusicListWidget: [" << playlistName << "] added.";
            Q_EMIT signalUserPlaylistButtonAdded(playlistName);
        }
    }

    void MusicListWidget::appendUserButton(const QString &playlistName) {
        auto *button = new Panel::BetterButton({playlistName, Res::MusicListSVG, Res::BUTTON_NORMAL_QSS, 30, 0}, this);
        m_UserButtonHash[playlistName] = button;
        m_buttonLayout->addWidget(button);
        button->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(button, &Panel::BetterButton::signalButtonClicked, this, &MusicListWidget::handleMusicButtonClicked);
        connect(button, &Panel::BetterButton::customContextMenuRequested, this, &MusicListWidget::handleContextMenu);
    }

    void MusicListWidget::handleContextMenu(const QPoint &pos) {
        auto *button = qobject_cast<Panel::BetterButton *>(sender());
        auto *listMenu = new QMenu(this);
        connect(listMenu, &QMenu::aboutToHide, listMenu, &QMenu::deleteLater);
        const QAction *deleteAction = listMenu->addAction("Delete");
        connect(deleteAction, &QAction::triggered, this, [this, button]() {
            Q_EMIT signalUserPlaylistButtonRemoved(button->text());
        });
        listMenu->exec(button->mapToGlobal(pos));
    }

    void MusicListWidget::removeUserButton(const QString &playlistName) {
        if (QPushButton* buttonToRemove = m_UserButtonHash.take(playlistName)) {
            m_buttonLayout->removeWidget(buttonToRemove);
            delete buttonToRemove;
        }
    }

    void MusicListWidget::initUserListButtons(const QStringList &playlistNames) {
        for (const auto &name: playlistNames) {
            appendUserButton(name);
        }
    }

    void MusicListWidget::handleMusicButtonClicked(const QString &name) {
        qDebug() << "buttonWidget handleMusicButtonClicked";
        Q_EMIT signalMusicListButtonClicked(name);
    }
}
