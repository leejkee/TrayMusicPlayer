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

        void signalViewItemAddToList(const QString &sourceList, const QString &destinationList, int index);

        void signalViewItemDel(const QString &key, const QString &title);

    private Q_SLOTS:
        void showContextMenu(const QPoint &pos);

        void handleViewItemPlayButton(int index);

        void handleMenuPop(const QPoint &pos, const QModelIndex &index);

    public Q_SLOTS:
        /// update the current selection when the current music changed
        /// keep the current item is shown in the visible area
        void updateCurrentIndex(int);

        /// update the view when a music list button is pressed to check the current list
        void showMusicList(const QString &name, const QStringList &nameList);

        /// @brief Refreshes the view widget to reflect changes in the local music path.
        ///
        /// This function is intended to be called when the local music path has been updated
        /// while the local music is currently being displayed by the view widget. It ensures
        /// that the view widget is updated to reflect the new path, such as updating the displayed
        /// information or reloading the music file.
        ///
        /// @note This function should only be called when the view widget is actively displaying
        ///       local music.
        void refreshForLocalMusic();

        void updatePlayingStatus(bool b);

    private:
        QLabel *m_labelName;
        QListView *m_playListView;
        Panel::BetterButton *m_playAllButton;
        DataModel *m_dataModel;
        ViewDelegate *m_viewDelegate;
        QStringList m_userPlaylistKeys;

        constexpr static int SIZE_TITLE_FONT = 14;

        void createConnections();

        void setListTitle(const QString &title);
    };
}
