//
// Created by cww on 25-4-10.
//
#pragma once
#include <QWidget>


namespace UI::Panel {
    class BetterButton;
}


class QVBoxLayout;
class QPushButton;
class QScrollArea;


namespace UI::MusicListWidget {
    class MusicListWidget final : public QWidget {
        Q_OBJECT

    public:
        explicit MusicListWidget(QWidget *parent = nullptr);

        // buttons created by user from settings
        void initUserListButtons(const QStringList &playlistNames);

    Q_SIGNALS:
        void signalMusicListButtonClicked(const QString &playlistName);

        void signalMusicListButtonAdded(const QString &playlistName);

    private Q_SLOTS:
        // expand icon
        void toggleExpand() const;

        // void removeButton(const QString &name);

    private:
        Panel::BetterButton *m_expandButton;
        Panel::BetterButton *m_addButton;
        QScrollArea *m_scrollArea;
        QWidget *m_buttonWidget;
        QVBoxLayout *m_buttonLayout;
        Panel::BetterButton *m_localListButton;

        QStringList m_userPlaylistKeys;

        void createConnections();

        void handleMusicButtonClicked(const QString &name);

        // button from new
        void addButton();

        void newButton(const QString &playlistName);

        void deleteButton(const QString &playlistName);

    };
}
