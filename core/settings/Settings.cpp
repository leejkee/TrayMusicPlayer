#include "Settings.h"
#include <QLogger.h>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>


namespace Tray::Core {
    class SettingsPrivate {
    public:
        static inline auto KEY_MUSIC_DIRECTORY = QStringLiteral("MusicDirectory");
        static inline auto KEY_DATABASE_DIRECTORY = QStringLiteral("DatabaseDirectory");
        static inline auto KEY_USER_LISTS = QStringLiteral("UserLists");
        static inline auto KEY_DEFAULT_VOLUME = QStringLiteral("DefaultVolume");

        static inline auto PROJECT_PATH = QStringLiteral("C:/Users/cww/Documents/Workspace/TrayMusicPlayer/");

        QString m_settingsPath;
        QString m_dbPath;
        QStringList m_localMusicList;
        QStringList m_userListKeys;
        int m_volume;

        Log::QLogger Log;
    };

    Settings::Settings(const QString &settingsPath, QObject *parent): QObject(parent),
                                                                      d(std::make_unique<SettingsPrivate>()) {
        this->setObjectName(QStringLiteral("Settings"));
        d->Log = Log::QLogger(objectName());
        d->m_settingsPath = SettingsPrivate::PROJECT_PATH + settingsPath;
        if (d->m_settingsPath.isEmpty()) {
            d->Log.log(Log::QLogger::LogLevel::Error, "the path of settings is empty");
            return;
        }
        QFile file(d->m_settingsPath);
        if (!file.open(QIODevice::ReadOnly)) {
            d->Log.log(Log::QLogger::LogLevel::Error, "Failed to open file");
            return;
        }
        const QByteArray jsonData = file.readAll();
        file.close();
        const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        if (jsonDoc.isNull() || !jsonDoc.isObject()) {
            d->Log.log(Log::QLogger::LogLevel::Error, "Failed to parse JSON");
            return;
        }
        QJsonObject jsonObj = jsonDoc.object();
        d->m_localMusicList = jsonObj[SettingsPrivate::KEY_MUSIC_DIRECTORY].toVariant().toStringList();
        d->m_dbPath = jsonObj[SettingsPrivate::KEY_DATABASE_DIRECTORY].toString();
        d->m_userListKeys = jsonObj[SettingsPrivate::KEY_USER_LISTS].toVariant().toStringList();
        d->m_volume = jsonObj[SettingsPrivate::KEY_DEFAULT_VOLUME].toInt();
    }


    void Settings::saveToJson() {
        QJsonObject jsonObj;
        jsonObj[SettingsPrivate::KEY_MUSIC_DIRECTORY] = QJsonArray::fromStringList(d->m_localMusicList);
        jsonObj[SettingsPrivate::KEY_DATABASE_DIRECTORY] = QJsonValue(d->m_dbPath);
        jsonObj[SettingsPrivate::KEY_DEFAULT_VOLUME] = QJsonValue(d->m_volume);
        jsonObj[SettingsPrivate::KEY_USER_LISTS] = QJsonArray::fromStringList(d->m_userListKeys);
        const QJsonDocument doc(jsonObj);
        QFile file(d->m_settingsPath);
        if (!file.open(QIODevice::WriteOnly)) {
            d->Log.log(Log::QLogger::LogLevel::Error, "Could not write to JSON file");
            return;
        }
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        d->Log.log(Log::QLogger::LogLevel::Info, "Saved JSON successfully");
    }

    void Settings::addLocalMusicDirectory(const QString &path) {
        if (!d->m_localMusicList.contains(path)) {
            d->m_localMusicList.append(path);
            saveToJson();
            d->Log.log(Log::QLogger::LogLevel::Info, "Added local music: " + path);
        }
    }

    void Settings::removeLocalMusicDirectory(const QString &path) {
        if (d->m_localMusicList.removeOne(path)) {
            saveToJson();
        }
    }

    void Settings::addUserMusicList(const QString &name) {
        if (!d->m_userListKeys.contains(name)) {
            d->m_userListKeys.append(name);
            saveToJson();
            d->Log.log(Log::QLogger::LogLevel::Info, "Added user music: " + name);
        }
    }

    void Settings::removeUserMusicList(const QString &name) {
        if (!d->m_userListKeys.removeOne(name)) {
            saveToJson();
            d->Log.log(Log::QLogger::LogLevel::Info, "Removed user music: " + name);
        }
    }

    QStringList Settings::getLocalMusicDirectories() const {
        return d->m_localMusicList;
    }

    QString Settings::getDatabaseDirectory() const {
        return d->m_dbPath;
    }

    QStringList Settings::getKeysUserPlaylist() const {
        return d->m_userListKeys;
    }

    unsigned Settings::getDefaultVolume() const {
        return d->m_volume;
    }
}
