#pragma once
#include "QLogger.h"
#include <QObject>
#include <QStringList>

namespace Tray::Config {
    class Settings final : public QObject {
        Q_OBJECT

    public:
        explicit Settings(const QString &settingsPath, QObject *parent = nullptr);

        Settings(const Settings &) = delete;

        Settings &operator=(const Settings &) = delete;

        Settings(Settings &&) = delete;

        Settings &operator=(Settings &&) = delete;

        void loadFromJson();

        void saveToJson();

        [[nodiscard]] QStringList getLocalMusicDirectories() const { return m_localMusicList; }

        [[nodiscard]] QString getDatabaseDirectory() const { return m_dbPath; }

        [[nodiscard]] QStringList getKeysUserPlaylist() const { return m_userList; }

        [[nodiscard]] unsigned getDefaultVolume() const { return m_volume; }


    Q_SIGNALS:
        void signalLocalSettingsChanged();

        void signalUserListAdded(const QString &);

        void signalUserListRemoved(const QString &);

    public Q_SLOTS:
        void addLocalMusicDirectory(const QString &path);

        void addUserMusicList(const QString &name);

        void removeUserMusicList(const QString &name);

        void removeLocalMusicDirectory(const QString &path);

    private:
        QString m_settingsPath;
        QString m_dbPath;
        QStringList m_localMusicList;
        QStringList m_userList;
        int m_volume;
        Log::QLogger Log;

        ~Settings() override = default;
    };
}
