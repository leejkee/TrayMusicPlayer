//
// Created by cww on 25-4-10.
//
#pragma once
#include <QWidget>
#include <memory>


namespace Tray::Ui::Panel
{
class StyleButton;
}

class QVBoxLayout;
class QScrollArea;

namespace Tray::Ui
{
class MusicListWidgetPrivate;
class MusicListWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit MusicListWidget(QWidget* parent = nullptr);

public Q_SLOTS:
    // buttons created by user from settings
    void initUserListButtons(const QStringList& playlistNames);

    void removeUserButton(const QString& playlistName);

    void appendUserButton(const QString& playlistName);

Q_SIGNALS:
    void signalMusicListButtonClicked(const QString& playlistName);

    void signalUserPlaylistButtonAdded(const QString& playlistName);

    void signalUserPlaylistButtonRemoved(const QString& text);

private Q_SLOTS:
    // expand icon
    void toggleExpand() const;

    void handleMusicButtonClicked(const QString& name);

    void newUserPlaylist();

    void handleContextMenu(const QPoint& pos);

private:
    std::unique_ptr<MusicListWidgetPrivate> d;

    void createConnections();
};
}
