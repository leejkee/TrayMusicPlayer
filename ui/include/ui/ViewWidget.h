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
        void signalViewDoubleClicked(int);

        void signalPlayAllClicked(const QString &name);

        void signalPlayToggle();

    private Q_SLOTS:
        void showContextMenu(const QPoint &pos);

        void handleAction(int);

    public Q_SLOTS:
        void viewDoubleClick(const QModelIndex &index);

        void updateCurrentIndex(int);

        void showMusicList(const QString &listName) const;

        ///
        /// init function
        /// This function will set the default string list (Local Music List) for ViewWidget
        void setDefaultList() const;

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

        void playingStatusChange(bool b) const;

    private:
        QLabel *m_labelName;
        QListView *m_playListView;
        Panel::BetterButton *m_playAllButton;
        Panel::DataModel *m_dataModel;
        Panel::ViewDelegate *m_viewDelegate;

        void createConnections();
    };
}
