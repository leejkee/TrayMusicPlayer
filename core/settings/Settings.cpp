#include "Settings.h"
#include <TrayConfig.h>
#include <QLogger.h>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>


namespace Tray::Core {
    class SettingsPrivate {
    public:
        explicit SettingsPrivate(const QString &objName);

        static inline const auto KEY_MUSIC_DIRECTORY = QStringLiteral("MusicDirectory");
        static inline const auto KEY_USER_LISTS = QStringLiteral("UserLists");
        static inline const auto KEY_DEFAULT_VOLUME = QStringLiteral("DefaultVolume");
        static inline const auto SETTINGS_DIRECTORY = QStringLiteral("settings/");
        static inline const auto SETTINGS_FILE = QStringLiteral("init.json");

#if defined(Q_OS_WIN32)
        static inline const auto SETTINGS_TEMPLATE_FILE = QStringLiteral("win32/init_win32.json");
#elif defined(Q_OS_LINUX)
        static inline const auto SETTINGS_TEMPLATE_FILE = QStringLiteral("linux/init_linux.json");
#endif

        Log::QLogger Log;
        QString m_settingsPath;
        QStringList m_localMusicList;
        QStringList m_userListKeys;
        int m_volume;

        [[nodiscard]] bool newSettingsFile(const QString &file) const;

        void parseJson();

        void saveToJson() const;
    };

    SettingsPrivate::SettingsPrivate(const QString &objName) : Log(Log::QLogger(objName)),
                                                               m_settingsPath(
                                                                   PROJECT_PATH + SETTINGS_DIRECTORY + SETTINGS_FILE)
                                                               , m_volume(0) {
        parseJson();
    }

    void SettingsPrivate::parseJson() {
        if (newSettingsFile(m_settingsPath)) {
            QFile file(m_settingsPath);
            if (!file.open(QIODevice::ReadOnly)) {
                Log.log(Log::QLogger::LogLevel::Error, "Failed to open file");
                return;
            }
            const QByteArray jsonData = file.readAll();
            file.close();
            const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
            if (jsonDoc.isNull() || !jsonDoc.isObject()) {
                Log.log(Log::QLogger::LogLevel::Error, "Failed to parse JSON");
                return;
            }
            Log.log(Log::QLogger::LogLevel::Info, "Parsed JSON successfully");
            QJsonObject jsonObj = jsonDoc.object();
            m_localMusicList = jsonObj[KEY_MUSIC_DIRECTORY].toVariant().toStringList();
            m_userListKeys = jsonObj[KEY_USER_LISTS].toVariant().toStringList();
            m_volume = jsonObj[KEY_DEFAULT_VOLUME].toInt();
        } else {
            Log.log(Log::QLogger::LogLevel::Error,
                    "Parse init.json failed, no settings file or cannot create the json file");
        }
    }

    bool SettingsPrivate::newSettingsFile(const QString &file) const {
        if (!QFile::exists(file)) {
            if (const QString source = PROJECT_PATH + SETTINGS_DIRECTORY + SETTINGS_TEMPLATE_FILE; !QFile::copy(
                source, file)) {
                Log.log(Log::QLogger::LogLevel::Error, "Failed to copy file: " + file);
                return false;
            } else {
                Log.log(Log::QLogger::LogLevel::Info,
                        "Successfully copy from template [" + source + "] as settings file: " + file);
            }
        }
        else {
            Log.log(Log::QLogger::LogLevel::Info, "Settings file already exists, read it: " + file);
        }
        return true;
    }

    Settings::~Settings() = default;

    Settings::Settings(QObject *parent): QObject(parent) {
        this->setObjectName(QStringLiteral("Settings"));
        d = std::make_unique<SettingsPrivate>(objectName());
    }

    void SettingsPrivate::saveToJson() const {
        QJsonObject jsonObj;
        jsonObj[KEY_MUSIC_DIRECTORY] = QJsonArray::fromStringList(m_localMusicList);
        jsonObj[KEY_DEFAULT_VOLUME] = QJsonValue(m_volume);
        jsonObj[KEY_USER_LISTS] = QJsonArray::fromStringList(m_userListKeys);
        const QJsonDocument doc(jsonObj);
        QFile file(m_settingsPath);
        if (!file.open(QIODevice::WriteOnly)) {
            Log.log(Log::QLogger::LogLevel::Error, "Could not write to JSON file");
            return;
        }
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        Log.log(Log::QLogger::LogLevel::Info, "Saved JSON successfully");
    }

    void Settings::addLocalMusicDirectory(const QString &path) {
        if (!d->m_localMusicList.contains(path)) {
            d->m_localMusicList.append(path);
            d->saveToJson();
            d->Log.log(Log::QLogger::LogLevel::Info, "Added local music: " + path);
        }
    }

    void Settings::removeLocalMusicDirectory(const QString &path) {
        if (d->m_localMusicList.removeOne(path)) {
            d->saveToJson();
        }
    }

    void Settings::addUserPlaylist(const QString &name) {
        if (!d->m_userListKeys.contains(name)) {
            d->m_userListKeys.append(name);
            d->saveToJson();
            Q_EMIT signalUserPlaylistsChanged(getKeysUserPlaylist());
            d->Log.log(Log::QLogger::LogLevel::Info, "Added user music: " + name);
        }
    }

    void Settings::removeUserPlaylist(const QString &name) {
        if (d->m_userListKeys.removeOne(name)) {
            d->saveToJson();
            Q_EMIT signalUserPlaylistsChanged(getKeysUserPlaylist());
            d->Log.log(Log::QLogger::LogLevel::Info, "Removed user music: " + name);
        }
    }

    QStringList Settings::getLocalMusicDirectories() const {
        return d->m_localMusicList;
    }

    QStringList Settings::getKeysUserPlaylist() const {
        return d->m_userListKeys;
    }

    unsigned Settings::getDefaultVolume() const {
        return d->m_volume;
    }
}
