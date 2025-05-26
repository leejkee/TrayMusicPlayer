#pragma once

#include "include/coreinterface.h"
#include <QLogger.h>
#include <QObject>
#include <memory>


namespace Tray::Core {
    class Player;
    class PlayList;
    class ListCache;
    class CorePrivate;

    class Core final : public CoreInterface {
        Q_OBJECT

    public:
        explicit Core(QObject *parent = nullptr);

        ~Core() override;

        void initWork() override;

    public Q_SLOTS:
        void setVolume(unsigned int volume) override;

        void playToggle() override;

        void playToggleWithListAndIndex(const QString &listKey, int) override;

        void nextMusic() override;

        void preMusic() override;

        void setPlayerPosition(qint64 position) override;

        void changePlayMode() override;

        void requestPlaylist(const QString &) override;

        QStringList getKeysOfUserPlaylist() override;

        void addUserPlaylist(const QString &) override;

        void removeUserPlaylist(const QString &) override;

        QStringList getLocalMusicPaths() override;

        void appendLocalMusicPath(const QString &) override;

        void removeLocalMusicPath(const QString &) override;

        void addMusicToList(const QString &sourceListKey, const QString &destinationListKey, int index) override;

        void removeMusicFromList(const QString &key, const QString &songTitle) override;

    private:
        std::unique_ptr<CorePrivate> d;

        void createConnections();
    };
} // namespace Core
