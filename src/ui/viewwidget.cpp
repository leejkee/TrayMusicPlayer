//
// Created by cww on 25-4-10.
//
#include <viewwidget/viewdatamodel.h>
#include <viewwidget/viewdelegate.h>
#include <viewwidget/viewwidget.h>
#include <stylebutton/stylebutton.h>
#include <log/log.h>
#include <traysvg.h>
#include <trayqss.h>
#include <QLabel>
#include <QHBoxLayout>
#include <QMenu>
#include <QListView>

namespace Tray::Ui
{
class ViewWidgetPrivate
{
public:
    constexpr static int PLAYALL_BTN_HEIGHT = 30;
    constexpr static int PLAYALL_BTN_WIDTH = 0;
    constexpr static int SIZE_TITLE_FONT = 14;
    const static inline auto PLAY_ALL_KEY = QStringLiteral("Play All");
    const static inline auto ADD_TO_LIST = QStringLiteral("Add to");
    const static inline auto REMOVE_FROM_LIST = QStringLiteral("Remove from");
    const static inline auto LOCAL_LIST_KEY = QStringLiteral("Local");

    QLabel* m_labelName;
    QListView* m_playlistView;
    Panel::StyleButton* m_playAllButton;
    ViewDataModel* m_dataModel;
    ViewDelegate* m_viewDelegate;
    QStringList m_userPlaylistKeys;
    QString m_displayPlaylistKey;
    QString m_currentPlaylistKey;
};

ViewWidget::ViewWidget(QWidget* parent)
    : QWidget(parent), d(std::make_unique<ViewWidgetPrivate>())
{
    d->m_labelName = new QLabel(this);
    d->m_labelName->setAlignment(Qt::AlignCenter);
    QFont font = d->m_labelName->font();
    font.setPointSize(ViewWidgetPrivate::SIZE_TITLE_FONT);
    font.setBold(true);
    d->m_labelName->setFont(font);

    d->m_playAllButton = new Panel::StyleButton(
                                                ViewWidgetPrivate::PLAY_ALL_KEY
                                                , {
                                                    ViewWidgetPrivate::PLAYALL_BTN_HEIGHT
                                                    , ViewWidgetPrivate::PLAYALL_BTN_WIDTH
                                                }
                                                , Res::PlayBtnSVG
                                                , Res::BUTTON_NORMAL_QSS
                                                , this);

    auto* spaceH = new QSpacerItem(-1
                                        , 0
                                        , QSizePolicy::Expanding
                                        , QSizePolicy::Minimum);
    auto* layoutH = new QHBoxLayout;
    layoutH->addWidget(d->m_playAllButton);
    layoutH->addSpacerItem(spaceH);

    d->m_dataModel = new ViewDataModel({}, this);
    // init fun
    d->m_playlistView = new QListView(this);
    d->m_viewDelegate = new ViewDelegate(d->m_playlistView);
    d->m_playlistView->setModel(d->m_dataModel);
    d->m_playlistView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    d->m_playlistView->setContextMenuPolicy(Qt::CustomContextMenu);
    setListViewQss(Res::LIST_VIEW_QSS);
    d->m_playlistView->viewport()->setMouseTracking(true);
    d->m_playlistView->setItemDelegate(d->m_viewDelegate);
    auto* Layout = new QVBoxLayout;
    Layout->addWidget(d->m_labelName);
    Layout->addItem(layoutH);
    Layout->addWidget(d->m_playlistView);
    setLayout(Layout);
    createConnections();
}

ViewWidget::~ViewWidget() = default;

void ViewWidget::createConnections()
{
    connect(d->m_playlistView
            , &QListView::customContextMenuRequested
            , this
            , &ViewWidget::showContextMenu);

    connect(d->m_playlistView
            , &QListView::doubleClicked
            , this
            , [this](const QModelIndex& index)
            {
                Q_EMIT signalViewItemPlayButtonClicked(d->m_displayPlaylistKey, index.row());
            });

    connect(d->m_playAllButton
            , &QPushButton::clicked
            , this
            , [this]()
            {
                Q_EMIT signalViewItemPlayButtonClicked(d->m_displayPlaylistKey, 0);
            });

    connect(d->m_viewDelegate
            , &ViewDelegate::signalViewItemPlayButtonClicked
            , this
            , [this](const int index)
            {
                Q_EMIT signalViewItemPlayButtonClicked(d->m_displayPlaylistKey, index);
            });

    connect(d->m_viewDelegate
            , &ViewDelegate::signalViewItemOptionsMenu
            , this
            , &ViewWidget::handleMenuPop);
}

void ViewWidget::setListViewQss(const QString& qssPath)
{
    if (qssPath.isEmpty())
    {
        LOG_WARNING("Empty qssPath");
    }
    if (QFile file(qssPath); file.open(QFile::ReadOnly))
    {
        const QString qss = QString::fromUtf8(file.readAll()).trimmed();
        file.close();
        d->m_playlistView->setStyleSheet(qss);
        LOG_INFO(QString("QSS [%1] has been loaded.").arg(qssPath));
    }
    else
    {
        LOG_WARNING(QString("Load QSS for ListView [%1] failed.").arg(qssPath));
    }
}

void ViewWidget::handleMenuPop(const QPoint& pos, const QModelIndex& index)
{
    auto* optionsMenu = new QMenu(this);
    connect(optionsMenu, &QMenu::aboutToHide, optionsMenu, &QMenu::deleteLater);

    if (d->m_displayPlaylistKey != ViewWidgetPrivate::LOCAL_LIST_KEY)
    {
        const QAction* delAction = optionsMenu->
                addAction(ViewWidgetPrivate::REMOVE_FROM_LIST);
        connect(delAction
                , &QAction::triggered
                , this
                , [this, index]()
                {
                    Q_EMIT signalMusicRemovedFromList(d->m_displayPlaylistKey
                                                      , index.data(Qt::UserRole + 1)
                                                     .toString());
                });
    }

    QAction* addToPlaylistAction = optionsMenu->
            addAction(ViewWidgetPrivate::ADD_TO_LIST);
    auto* addToOtherPlaylistMenu = new QMenu(optionsMenu);
    for (const QString& playlistName : d->m_userPlaylistKeys)
    {
        if (playlistName == d->m_displayPlaylistKey)
        {
            continue;
        }
        const auto* action = addToOtherPlaylistMenu->addAction(playlistName);
        connect(action
                , &QAction::triggered
                , this
                , [this, playlistName, index]()
                {
                    Q_EMIT signalMusicAddedToList(d->m_displayPlaylistKey
                                                  , playlistName
                                                  , index.row());
                });
    }
    addToPlaylistAction->setMenu(addToOtherPlaylistMenu);
    optionsMenu->exec(d->m_playlistView->viewport()->mapToGlobal(pos));
}

void ViewWidget::updateUserListKeys(const QStringList& keys)
{
    d->m_userPlaylistKeys = keys;
}

void ViewWidget::updateCurrentIndex(const int row)
{
    const auto index = d->m_dataModel->index(row, 0);
    d->m_playlistView->selectionModel()->select(index
                                                , QItemSelectionModel::ClearAndSelect);

    const QRect viewportRect = d->m_playlistView->viewport()->rect();
    const QModelIndex firstVisible = d->m_playlistView->
                                        indexAt(viewportRect.topLeft());
    const QModelIndex lastVisible = d->m_playlistView->
                                       indexAt(viewportRect.bottomLeft());
    if (row < firstVisible.row() + 1 || row > lastVisible.row() - 1)
    {
        d->m_playlistView->scrollTo(index, QAbstractItemView::EnsureVisible);
    }
    d->m_viewDelegate->updatePreviousIndex(row);
}

void ViewWidget::showContextMenu(const QPoint& pos)
{
    const QModelIndex index = d->m_playlistView->indexAt(pos);
    if (!index.isValid())
    {
        return;
    }
    handleMenuPop(pos, index);
}


void ViewWidget::displayTitleListToView(const QString& name
                                            , const QStringList& nameList)
{
    d->m_displayPlaylistKey = name;
    d->m_labelName->setText(name);
    d->m_dataModel->setMusicList(nameList);
    syncRenderMatchStatus();
}

void ViewWidget::handleListCacheUpdated(const QString& key
                                         , const QStringList& nameList)
{
    if (d->m_displayPlaylistKey == key)
    {
        // the List "key" is shown now
        d->m_dataModel->setMusicList(nameList);
    }
}

void ViewWidget::updatePlayingStatus(const bool b)
{
    d->m_viewDelegate->updatePlayingStatus(b);
}

void ViewWidget::syncRenderWithCurrentPlaylist(const QString& key)
{
    d->m_currentPlaylistKey = key;
    syncRenderMatchStatus();
}

void ViewWidget::syncRenderMatchStatus()
{
    d->m_viewDelegate->
       setCurrentPlaylistMatched(d->m_currentPlaylistKey == d->m_displayPlaylistKey);
}

}
