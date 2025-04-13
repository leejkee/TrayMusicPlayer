//
// Created by cww on 25-4-10.
//
#include <ui/ViewWidget.h>
#include <panel/BetterButton.h>
#include <panel/DataModel.h>
#include <panel/ViewDelegate.h>
#include <ui/Assets.h>
#include <QListView>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QMenu>


namespace UI::ViewWidget {
    void ViewWidget::createConnections() {
        connect(m_playListView, &QListView::customContextMenuRequested, this, &ViewWidget::showContextMenu);
        connect(m_playListView, &QListView::doubleClicked, this, &ViewWidget::viewDoubleClick);
        // connect(&PlayList::instance(), &PlayList::currentMusicIndexChanged, this, &ViewWidget::updateCurrentIndex);
        connect(m_playAllButton, &QPushButton::clicked, this, [this]() {
            Q_EMIT signalPlayAllClicked(m_labelName->text());
        });
        // connect(m_songDelegate, &SongDelegate::signalViewPlayButtonClick, &PlayList::instance(), &PlayList::setCurrentMusicIndex);
        connect(m_viewDelegate, &Panel::ViewDelegate::signalPlayToggle, this, [this]() {
            Q_EMIT signalPlayToggle();
        });
    }

    ViewWidget::ViewWidget(QWidget *parent): QWidget(parent) {
        m_labelName = new QLabel(this);
        m_playAllButton = new Panel::BetterButton(QIcon(SvgRes::PlayIconSVG), this, Panel::BetterButton::WithQss, User::PLAY_ALL_KEY);

        const auto spaceH = new QSpacerItem(-1, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
        const auto layoutH = new QHBoxLayout;
        layoutH->addWidget(m_playAllButton);
        layoutH->addSpacerItem(spaceH);

        m_dataModel = new Panel::DataModel({}, this);

        // init fun
        m_playListView = new QListView(this);
        m_viewDelegate = new Panel::ViewDelegate(m_playListView);
        m_playListView->setModel(m_dataModel);
        m_playListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_playListView->setContextMenuPolicy(Qt::CustomContextMenu);
        m_playListView->setStyleSheet(Tools::readQSS(QssRes::LIST_VIEW_QSS));


        // Delegate Func
        m_playListView->setItemDelegate(m_viewDelegate);

        auto *Layout = new QVBoxLayout;
        Layout->addWidget(m_labelName);
        Layout->addItem(layoutH);
        Layout->addWidget(m_playListView);
        this->setLayout(Layout);
        createConnections();
        // setDefaultList();
    }

    void ViewWidget::viewDoubleClick(const QModelIndex &index) {
        qDebug() << "ViewWidget::viewDoubleClick";
        // if (PlayList::instance().isEmptyPlayList()) {
        //     qDebug() << "List is not loaded, PlayList is empty, load music first";
        //     qDebug() << "Q_EMIT ViewWidget::signalPlayAllClicked to load PlayList";
        //     if (m_labelName) {
        //         Q_EMIT signalPlayAllClicked(m_labelName->text());
        //     } else {
        //         qDebug() << "m_labelName is nullptr!";
        //     }
        // }
        // Then, check music to play this song
        // PlayList::instance().setCurrentMusicIndex(index.row());
    }


    void ViewWidget::updateCurrentIndex(const int row) {
        qDebug() << "ViewWidget::updateCurrentIndex";
        // m_playListView->selectionModel()->select(m_playListModel->index(index, 0), QItemSelectionModel::ClearAndSelect);
        m_playListView->selectionModel()->select(m_dataModel->index(row, 0), QItemSelectionModel::ClearAndSelect);
        m_viewDelegate->setPreviousIndex(row);
    }

    void ViewWidget::showContextMenu(const QPoint &pos) {
        qDebug() << "ViewWidget::showContextMenu";
        const QModelIndex index = m_playListView->indexAt(pos);
        if (!index.isValid()) {
            return;
        }
        auto clickedRow = index.row();
        QMenu *menu = new QMenu(this);
        QAction *action = menu->addAction(tr("Playlist"));
        connect(action, &QAction::triggered, this, [clickedRow, this]() {
            handleAction(clickedRow);
        });
        connect(menu, &QMenu::aboutToHide, menu, &QMenu::deleteLater);
        menu->exec(m_playListView->viewport()->mapToGlobal(pos));
    }

    void ViewWidget::handleAction(const int index) {
        qDebug() << "ViewWidget::handleAction" << index;
    }


    void ViewWidget::showMusicList(const QStringList &nameList) {
        m_dataModel->setMusicList(nameList);
        // const auto songNameList = MusicListCache::instance().getSongNameListByName(listName);
        // if (songNameList.isEmpty()) {
        //     qDebug() << "ViewWidget::showMusicList" << "MusicList is empty(MusicListCache::getSongNameListByName)";
        //     return;
        // }
        //
        // // update the model
        // m_dataModel->setSongs(songNameList);
        // m_labelName->setText(listName);

        // load play list when the list was loaded to the Model-View
        // PlayList::instance().loadMusicByName(listName);
    }

    void ViewWidget::setDefaultList() const {
        // showMusicList(User::LOCAL_LIST_KEY);
    }


    void ViewWidget::refreshForLocalMusic() const {
        if (m_labelName->text() == User::LOCAL_LIST_KEY) {
            // showMusicList(User::LOCAL_LIST_KEY);
            qDebug() << "ViewWidget::refreshForLocalMusic";
        }
    }

    void ViewWidget::playingStatusChange(const bool b) const {
        m_viewDelegate->setPlayStatus(b);
    }

    void ViewWidget::setViewTitle(const QString &title) {
        m_labelName->setText(title);
    }

    void ViewWidget::setModeMusicList(const QStringList &nameList) {
        m_dataModel->setMusicList(nameList);
    }
}
