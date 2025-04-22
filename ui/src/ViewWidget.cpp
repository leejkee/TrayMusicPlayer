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
    ViewWidget::ViewWidget(QWidget *parent): QWidget(parent) {
        m_labelName = new QLabel(this);
        m_playAllButton = new Panel::BetterButton(QIcon(SvgRes::PlayIconSVG), this, Panel::BetterButton::WithQss);
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

        connect(m_viewDelegate, &Panel::ViewDelegate::signalViewItemPlayButtonClicked,
                this, &ViewWidget::handleViewItemPlayButton);
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
        auto clickedRow = index.row();
        auto *menu = new QMenu(this);
        QAction *action = menu->addAction(tr("Playlist"));
        connect(action, &QAction::triggered, this, [clickedRow, this]() {
            handleAction(clickedRow);
        });
        connect(menu, &QMenu::aboutToHide, menu, &QMenu::deleteLater);
        menu->exec(m_playListView->viewport()->mapToGlobal(pos));
    }

    void ViewWidget::handleAction(const int index) {
    }


    void ViewWidget::showMusicList(const QString &name, const QStringList &nameList) {
        m_labelName->setText(name);
        m_dataModel->setMusicList(nameList);
    }


    void ViewWidget::refreshForLocalMusic() const {
        if (m_labelName->text() == User::LOCAL_LIST_KEY) {
            // showMusicList(User::LOCAL_LIST_KEY);
        }
    }

    void ViewWidget::updatePlayingStatus(const bool b) {
        m_viewDelegate->updatePlayingStatus(b);
    }
}
