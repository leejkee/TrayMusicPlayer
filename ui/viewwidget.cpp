//
// Created by cww on 25-4-10.
//
#include "datamodel.h"
#include "viewdelegate.h"
#include <viewwidget.h>
#include <betterbutton.h>
#include <uiconfig.h>
#include <traysvg.h>
#include <trayqss.h>
#include <QLabel>
#include <QHBoxLayout>
#include <QMenu>
#include <QListView>


namespace Tray::Ui {
class ViewWidgetPrivate {
public:
    explicit ViewWidgetPrivate(ViewWidget *w);

    constexpr static int PLAYALL_BTN_HEIGHT = 30;
    constexpr static int PLAYALL_BTN_WIDTH = 0;
    constexpr static int SIZE_TITLE_FONT = 14;

    QLabel *m_labelName;
    QListView *m_playListView;
    Panel::BetterButton *m_playAllButton;
    DataModel *m_dataModel;
    ViewDelegate *m_viewDelegate;
    QStringList m_userPlaylistKeys;
    QString m_currentPlaylistKey;

    QWidget *q_ptr;
};

ViewWidgetPrivate::ViewWidgetPrivate(ViewWidget *w) : q_ptr(w) {
    m_labelName = new QLabel(q_ptr);
    m_playAllButton = new Panel::BetterButton(
        {PLAY_ALL_KEY, Res::PlayIconSVG, Res::BUTTON_NORMAL_QSS, PLAYALL_BTN_HEIGHT, PLAYALL_BTN_WIDTH}, q_ptr);

    const auto spaceH = new QSpacerItem(-1, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    const auto layoutH = new QHBoxLayout;
    layoutH->addWidget(m_playAllButton);
    layoutH->addSpacerItem(spaceH);

    m_dataModel = new DataModel({}, q_ptr);
    // init fun
    m_playListView = new QListView(q_ptr);
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
    q_ptr->setLayout(Layout);
}

ViewWidget::~ViewWidget() = default;

ViewWidget::ViewWidget(QWidget *parent): QWidget(parent) {
    d = std::make_unique<ViewWidgetPrivate>(this);
    createConnections();
}

void ViewWidget::createConnections() {
    connect(d->m_playListView, &QListView::customContextMenuRequested,
            this, &ViewWidget::showContextMenu);

    connect(d->m_playListView, &QListView::doubleClicked,
            this, [this](const QModelIndex &index) {
                Q_EMIT signalViewItemPlayButtonClicked(d->m_labelName->text(), index.row());
            });

    connect(d->m_playAllButton, &QPushButton::clicked,
            this, [this]() {
                Q_EMIT signalViewItemPlayButtonClicked(d->m_labelName->text(), 0);
            });

    connect(d->m_viewDelegate, &ViewDelegate::signalViewItemPlayButtonClicked,
            this, [this](const int index) { Q_EMIT signalViewItemPlayButtonClicked(d->m_labelName->text(), index); });

    connect(d->m_viewDelegate, &ViewDelegate::signalViewItemOptionsMenu,
            this, &ViewWidget::handleMenuPop);
}


void ViewWidget::handleMenuPop(const QPoint &pos, const QModelIndex &index) {
    auto *optionsMenu = new QMenu(this);
    connect(optionsMenu, &QMenu::aboutToHide, optionsMenu, &QMenu::deleteLater);

    const QAction *delAction = optionsMenu->addAction(tr("Delete from this playlist"));
    connect(delAction, &QAction::triggered, this, [this, index]() {
        Q_EMIT signalMusicRemovedFromList(d->m_labelName->text(), index.data(Qt::UserRole + 1).toString());
    });

    QAction *addToPlaylistAction = optionsMenu->addAction(tr("Add to playlist"));
    auto *addToOtherPlaylistMenu = new QMenu(optionsMenu);
    for (const QString &playlistName: d->m_userPlaylistKeys) {
        const auto *action = addToOtherPlaylistMenu->addAction(playlistName);
        connect(action, &QAction::triggered, this, [this, playlistName, index]() {
            Q_EMIT signalMusicAddedToList(d->m_labelName->text(), playlistName, index.row());
        });
    }
    addToPlaylistAction->setMenu(addToOtherPlaylistMenu);
    optionsMenu->exec(d->m_playListView->viewport()->mapToGlobal(pos));
}

void ViewWidget::setUserPlaylistKeys(const QStringList &keys) {
    d->m_userPlaylistKeys = keys;
}

void ViewWidget::updateCurrentIndex(const int row) {
    const auto index = d->m_dataModel->index(row, 0);
    d->m_playListView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);

    const QRect viewportRect = d->m_playListView->viewport()->rect();
    const QModelIndex firstVisible = d->m_playListView->indexAt(viewportRect.topLeft());
    const QModelIndex lastVisible = d->m_playListView->indexAt(viewportRect.bottomLeft());
    if (row < firstVisible.row() + 1 || row > lastVisible.row() - 1) {
        d->m_playListView->scrollTo(index, QAbstractItemView::EnsureVisible);
    }
    d->m_viewDelegate->updatePreviousIndex(row);
}

void ViewWidget::showContextMenu(const QPoint &pos) {
    const QModelIndex index = d->m_playListView->indexAt(pos);
    if (!index.isValid()) {
        return;
    }
    handleMenuPop(pos, index);
}


void ViewWidget::showCurrentTitleListToView(const QString &name, const QStringList &nameList) {
    setListTitle(name);
    d->m_dataModel->setMusicList(nameList);
    syncRenderMatchStatus();
}

void ViewWidget::refreshCurrentMusicList(const QString &key, const QStringList &nameList) {
    if (d->m_labelName->text() == key) {
        d->m_dataModel->setMusicList(nameList);
    }
}

void ViewWidget::setListTitle(const QString &title) {
    d->m_labelName->setText(title);
    d->m_labelName->setAlignment(Qt::AlignCenter);

    QFont font = d->m_labelName->font();
    font.setPointSize(ViewWidgetPrivate::SIZE_TITLE_FONT);
    font.setBold(true);
    d->m_labelName->setFont(font);
}

void ViewWidget::updatePlayingStatus(const bool b) {
    d->m_viewDelegate->updatePlayingStatus(b);
}

void ViewWidget::syncRenderWithCurrentPlaylist(const QString &key) {
    d->m_currentPlaylistKey = key;
    syncRenderMatchStatus();
}

void ViewWidget::syncRenderMatchStatus() {
    d->m_viewDelegate->setCurrentPlaylistMatched(d->m_labelName->text() == d->m_currentPlaylistKey);
}
}
