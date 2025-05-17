//
// Created by cww on 25-4-10.
//
#include "ViewWidget.h"
#include "DataModel.h"
#include "ViewDelegate.h"
#include <BetterButton.h>
#include <UiConfig.h>
#include <TraySVG.h>
#include <TrayQSS.h>
#include <QLabel>
#include <QHBoxLayout>
#include <QMenu>
#include <QListView>

namespace Tray::Ui {
    ViewWidget::ViewWidget(QWidget *parent): QWidget(parent) {
        m_labelName = new QLabel(this);
        m_playAllButton = new Panel::BetterButton(PLAY_ALL_KEY, QIcon(Res::PlayIconSVG), this);

        const auto spaceH = new QSpacerItem(-1, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
        const auto layoutH = new QHBoxLayout;
        layoutH->addWidget(m_playAllButton);
        layoutH->addSpacerItem(spaceH);

        m_dataModel = new DataModel({}, this);
        // init fun
        m_playListView = new QListView(this);
        m_viewDelegate = new ViewDelegate(m_playListView);
        m_playListView->setModel(m_dataModel);
        m_playListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_playListView->setContextMenuPolicy(Qt::CustomContextMenu);
        m_playListView->setStyleSheet(readQSS(Res::LIST_VIEW_QSS));
        m_playListView->viewport()->setMouseTracking(true);

        // Delegate Func
        m_playListView->setItemDelegate(m_viewDelegate);

        auto *Layout = new QVBoxLayout;
        Layout->addWidget(m_labelName);
        Layout->addItem(layoutH);
        Layout->addWidget(m_playListView);
        this->setLayout(Layout);
        createConnections();
    }

    void ViewWidget::createConnections() {
        connect(m_playListView, &QListView::customContextMenuRequested,
                this, &ViewWidget::showContextMenu);

        connect(m_playListView, &QListView::doubleClicked,
                this, [this](const QModelIndex &index) {
                    Q_EMIT signalViewItemPlayButtonClicked(m_labelName->text(), index.row());
                });

        connect(m_playAllButton, &QPushButton::clicked,
                this, [this]() {
                    Q_EMIT signalViewItemPlayButtonClicked(m_labelName->text(), 0);
                });

        connect(m_viewDelegate, &ViewDelegate::signalViewItemPlayButtonClicked,
                this, &ViewWidget::handleViewItemPlayButton);

        connect(m_viewDelegate, &ViewDelegate::signalViewItemOptionsMenu,
                this, &ViewWidget::handleMenuPop);
    }


    void ViewWidget::handleMenuPop(const QPoint &pos, const QModelIndex &index) {
        QMenu *optionsMenu = new QMenu(this);
        connect(optionsMenu, &QMenu::aboutToHide, optionsMenu, &QMenu::deleteLater);

        const QAction *delAction = optionsMenu->addAction(tr("Delete from this playlist"));
        connect(delAction, &QAction::triggered, this, [this, index]() {
            Q_EMIT signalViewItemDel(m_labelName->text(), index.data(Qt::UserRole + 1).toString());
        });

        QAction *addToPlaylistAction = optionsMenu->addAction(tr("Add to playlist"));
        QMenu *addToOtherPlaylistMenu = new QMenu();
        for (const QString& playlistName : m_userPlaylistKeys) {
            auto *action = new QAction(playlistName);
            connect(action, &QAction::triggered, this, [this, playlistName, index]() {
                Q_EMIT signalViewItemAddToList(m_labelName->text(), playlistName, index.row());
            });
            addToOtherPlaylistMenu->addAction(action);
        }
        addToPlaylistAction->setMenu(addToOtherPlaylistMenu);
        optionsMenu->exec(m_playListView->viewport()->mapToGlobal(pos));
    }

    void ViewWidget::setUserPlaylistKeys(const QStringList &keys) {
        m_userPlaylistKeys = keys;
    }

    void ViewWidget::handleViewItemPlayButton(const int index) {
        Q_EMIT signalViewItemPlayButtonClicked(m_labelName->text(), index);
    }

    void ViewWidget::updateCurrentIndex(const int row) {
        const auto index = m_dataModel->index(row, 0);
        m_playListView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);

        const QRect viewportRect = m_playListView->viewport()->rect();
        const QModelIndex firstVisible = m_playListView->indexAt(viewportRect.topLeft());
        const QModelIndex lastVisible = m_playListView->indexAt(viewportRect.bottomLeft());
        if (row < firstVisible.row() + 1 || row > lastVisible.row() - 1) {
            m_playListView->scrollTo(index, QAbstractItemView::EnsureVisible);
        }

        m_viewDelegate->updatePreviousIndex(row);
    }

    void ViewWidget::showContextMenu(const QPoint &pos) {
        const QModelIndex index = m_playListView->indexAt(pos);
        if (!index.isValid()) {
            return;
        }
        handleMenuPop(pos, index);
    }

    // switch view
    void ViewWidget::showMusicList(const QString &name, const QStringList &nameList) {
        setListTitle(name);
        m_dataModel->setMusicList(nameList);
    }

    void ViewWidget::updateViewList(const QString &key, const QStringList &nameList) {
        if (m_labelName->text() == key) {
            m_dataModel->setMusicList(nameList);
        }
    }

    void ViewWidget::updateCurrentView(const QString &key, const QStringList &nameList) {
        if (m_labelName->text() == key) {
            m_dataModel->setMusicList(nameList);
        }

    }

    void ViewWidget::setListTitle(const QString &title) {
        m_labelName->setText(title);
        m_labelName->setAlignment(Qt::AlignCenter); // 左对齐，垂直居中

        QFont font = m_labelName->font();
        font.setPointSize(SIZE_TITLE_FONT);
        font.setBold(true);
        m_labelName->setFont(font);
    }

    // todo
    void ViewWidget::refreshForLocalMusic() {
        if (m_labelName->text() == LOCAL_LIST_KEY) {
            // showMusicList(User::LOCAL_LIST_KEY);
        }
    }

    void ViewWidget::updatePlayingStatus(const bool b) {
        m_viewDelegate->updatePlayingStatus(b);
    }
}
