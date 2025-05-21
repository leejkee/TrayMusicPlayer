//
// Created by cww on 25-4-9.
//
#pragma once

#include <QWidget>

class QLabel;
class QListView;


namespace Tray::Ui::Panel {
    class BetterButton;
}


namespace Tray::Ui {
    class DataModel;
    class ViewDelegate;

    class ViewWidget final : public QWidget {
        Q_OBJECT

    public:
        explicit ViewWidget(QWidget *parent = nullptr);

        void setUserPlaylistKeys(const QStringList &keys);

    Q_SIGNALS:
        void signalViewItemPlayButtonClicked(const QString &key, int index);

        void signalMusicAddedToList(const QString &sourceList, const QString &destinationList, int index);

        void signalMusicRemovedFromList(const QString &key, const QString &title);

    private Q_SLOTS:
        // handle the context menu pop for right-click event
        void showContextMenu(const QPoint &pos);

        // handle the options menu pop
        void handleMenuPop(const QPoint &pos, const QModelIndex &index);

        void syncRenderMatchStatus();

    public Q_SLOTS:
        /// @brief update the current selection when the current music changed
        /// Also, keep the current item is shown in the visible area is supported
        void updateCurrentIndex(int);

        // update the view when a music list button is pressed
        void showCurrentTitleListToView(const QString &name, const QStringList &nameList);

        // update the view when the current list is updated
        void refreshCurrentMusicList(const QString &key, const QStringList &nameList);

        /// @brief update the status of playing on ListView
        /// @param b the bool value
        void updatePlayingStatus(bool b);

        void syncRenderWithCurrentPlaylist(const QString &key);

    private:
        QLabel *m_labelName;
        QListView *m_playListView;
        Panel::BetterButton *m_playAllButton;
        DataModel *m_dataModel;
        ViewDelegate *m_viewDelegate;
        QStringList m_userPlaylistKeys;
        QString m_currentPlaylistKey;

        constexpr static int SIZE_TITLE_FONT = 14;

        void createConnections();

        void setListTitle(const QString &title);
    };
}
