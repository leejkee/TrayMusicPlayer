#pragma once
#include <Logger_qt.h>
#include <QObject>
#include <QStringList>




namespace Core::Service {
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


        [[nodiscard]] QStringList getLocalMusicDirectories() const { return m_localMusicPaths; }


        [[nodiscard]] QString getDatabaseDirectory() const { return m_dbPath; }


        [[nodiscard]] QStringList getUserMusicList() const { return m_userMusicList; }


        [[nodiscard]] float getDefaultVolume() const { return m_volume; }


    Q_SIGNALS:
        void signalLocalSettingsChanged();

        void signalUserListAdded(const QString &);

        void signalUserListRemoved(const QString &);

    public Q_SLOTS:
        void addLocalMusicDirectory(const QString &path);

        void addUserMusicList(const QString &path);

        void removeMusicDirectory(const QString &path);

    private:
        QString m_settingsPath;
        QString m_dbPath;
        QStringList m_localMusicPaths;
        QStringList m_userMusicList;
        float m_volume{};
        Logger_QT Log;

        ~Settings() override = default;
    };
}
