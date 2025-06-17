//
// Created by cww on 25-4-10.
//
#pragma once
#include <QWidget>


namespace Tray::Ui::Panel {
    class BetterButton;
}

class QVBoxLayout;
class QScrollArea;

namespace Tray::Ui {
    class MusicListWidget final : public QWidget {
        Q_OBJECT

    public:
        explicit MusicListWidget(QWidget *parent = nullptr);

    public Q_SLOTS:
        // buttons created by user from settings
        void initUserListButtons(const QStringList &playlistNames);

        void removeUserButton(const QString &playlistName);

        void appendUserButton(const QString &playlistName);

    Q_SIGNALS:
        void signalMusicListButtonClicked(const QString &playlistName);

        void signalUserPlaylistButtonAdded(const QString &playlistName);

        void signalUserPlaylistButtonRemoved(const QString &text);

    private Q_SLOTS:
        // expand icon
        void toggleExpand() const;

        void handleMusicButtonClicked(const QString &name);

        void newUserPlaylist();

        void handleContextMenu(const QPoint &pos);

    private:
        Panel::BetterButton *m_expandButton;
        Panel::BetterButton *m_addButton;
        QScrollArea *m_scrollArea;
        QWidget *m_buttonWidget;
        QVBoxLayout *m_buttonLayout;
        Panel::BetterButton *m_localListButton;
        QHash<QString, Panel::BetterButton *> m_UserButtonHash;

        static inline const auto LOCAL_LIST_KEY = QStringLiteral("Local");
        static inline const auto EXPAND_BTN_TEXT = QStringLiteral("User's");

        void createConnections();
    };
}
