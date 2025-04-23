//
// Created by cww on 25-4-9.
//
#pragma once

#include <QWidget>

class QLabel;
class QListView;


namespace UI::Panel {
    class BetterButton;
    class DataModel;
    class ViewDelegate;
}


namespace UI::ViewWidget {
    class ViewWidget final : public QWidget {
        Q_OBJECT

    public:
        explicit ViewWidget(QWidget *parent = nullptr);

    Q_SIGNALS:
        void signalViewItemPlayButtonClicked(const QString &, int);

    private Q_SLOTS:
        void showContextMenu(const QPoint &pos);

        void handleAction(int);

        void handleViewItemPlayButton(int index);

        void handleViewItemAddToList(int index);

    public Q_SLOTS:

        /// update the current selection when the current music changed
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
        void refreshForLocalMusic() const;

        void updatePlayingStatus(bool b);

    private:
        QLabel *m_labelName;
        QListView *m_playListView;
        Panel::BetterButton *m_playAllButton;
        Panel::DataModel *m_dataModel;
        Panel::ViewDelegate *m_viewDelegate;

        void createConnections();

    };
}
