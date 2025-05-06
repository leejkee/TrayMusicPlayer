#pragma once
#include <QObject>
#include <QStringList>
#include <memory>

namespace Tray::Config {
    class SettingsPrivate;

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

        [[nodiscard]] QStringList getLocalMusicDirectories() const;

        [[nodiscard]] QString getDatabaseDirectory() const;

        [[nodiscard]] QStringList getKeysUserPlaylist() const;

        [[nodiscard]] unsigned getDefaultVolume() const;


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
        std::unique_ptr<SettingsPrivate> d;
        ~Settings() override = default;
    };
}
