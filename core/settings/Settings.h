#pragma once
#include <QObject>
#include <QStringList>
#include <memory>


namespace Tray::Core {
    class SettingsPrivate;

    class Settings final : public QObject {
        Q_OBJECT

    public:
        explicit Settings(const QString &settingsPath, QObject *parent = nullptr);

        Settings(const Settings &) = delete;

        Settings &operator=(const Settings &) = delete;

        Settings(Settings &&) = delete;

        Settings &operator=(Settings &&) = delete;

        ~Settings() override;

        void saveToJson();

        [[nodiscard]] QStringList getLocalMusicDirectories() const;

        [[nodiscard]] QStringList getKeysUserPlaylist() const;

        [[nodiscard]] unsigned getDefaultVolume() const;

    Q_SIGNALS:
        void signalUserPlaylistsChanged(const QStringList &);

    public Q_SLOTS:
        void addLocalMusicDirectory(const QString &path);

        void addUserMusicList(const QString &name);

        void removeUserMusicList(const QString &name);

        void removeLocalMusicDirectory(const QString &path);

    private:
        std::unique_ptr<SettingsPrivate> d;
    };
}
