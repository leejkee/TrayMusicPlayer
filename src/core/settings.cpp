#include <QCoreApplication>
#include <settings/settings.h>
#include <include/common/trayconfig.h>
#include <log/log.h>
#include <utils/jsonutils.h>
#include <QFile>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace Tray::Core
{
class SettingsPrivate
{
public:
    struct Key
    {
        static inline const auto PLAYLIST_KEY = QStringLiteral("Playlist");

        static inline const auto AUDIO_KEY = QStringLiteral("Audio");

        struct Playlist
        {
            static inline const auto MUSIC_PATHS = QStringLiteral("MusicPaths");

            static inline const auto USER_LISTS = QStringLiteral("UserLists");

            static inline const auto PRELOAD_LIST = QStringLiteral("PreloadList");
        };

        struct Audio
        {
            static inline const auto DEFAULT_VOLUME =
                    QStringLiteral("DefaultVolume");
        };
    };

    struct SettingsData
    {
        struct Audio
        {
            int defaultVolume;
        };

        struct Playlist
        {
            QString preloadList;
            QStringList musicPaths;
            QStringList userLists;
        };

        // struct RememberSet
        // {
        //     QString theLastListKey;
        //     QString theLastMusic;
        // };

        Audio audio;
        Playlist playlist;
    };

#define SETTING_RELEASE 0

    static inline const auto SETTINGS_DIRECTORY = QStringLiteral("settings/");

    static inline const auto SETTINGS_FILE = QStringLiteral("init.json");

    static inline const auto LOCAL_KEY = QStringLiteral("Local");

    QString m_settingFilePath;

    SettingsData m_data;
};


void Settings::parseJson()
{
    QFile file(d->m_settingFilePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        LOG_ERROR("Failed to open file");
        return;
    }
    const QByteArray jsonData = file.readAll();
    file.close();
    QJsonParseError parseError;
    const QJsonDocument jsonDoc =
            QJsonDocument::fromJson(jsonData, &parseError);
    if (jsonDoc.isNull() || !jsonDoc.isObject())
    {
        LOG_ERROR("Failed to parse JSON" + parseError.errorString());
        return;
    }
    LOG_INFO("Parse JSON successfully");

    const QJsonObject jsonObj = jsonDoc.object();

    if (jsonObj.contains(SettingsPrivate::Key::PLAYLIST_KEY))
    {
        const QJsonObject obj = jsonObj.value(SettingsPrivate::Key::PLAYLIST_KEY).
                                        toObject();
        d->m_data.playlist.musicPaths = Utils::getJsonValue<QStringList>(obj
                                                  , SettingsPrivate::Key::Playlist::MUSIC_PATHS);
        d->m_data.playlist.userLists = Utils::getJsonValue<QStringList>(obj
                                                 , SettingsPrivate::Key::Playlist::USER_LISTS);
        d->m_data.playlist.preloadList = Utils::getJsonValue<QString>(obj
                                                 , SettingsPrivate::Key::Playlist::PRELOAD_LIST);
    }
    else
    {
        LOG_ERROR("The Key 'Music' is missing in init.json, using default value");
    }

    if (jsonObj.contains(SettingsPrivate::Key::AUDIO_KEY))
    {
        const QJsonObject obj = jsonObj.value(SettingsPrivate::Key::AUDIO_KEY).
                                        toObject();
        d->m_data.audio.defaultVolume = Utils::getJsonValue<int>(obj, SettingsPrivate::Key::Audio::DEFAULT_VOLUME);
    }
    else
    {
        LOG_ERROR("The Key 'Audio' is missing in init.json, using default value");
    }
}


bool Settings::initJsonFile(const QString& filePath) const
{
    if (!QFile::exists(filePath))
    {
        d->m_data.audio.defaultVolume = 20;
        d->m_data.playlist.musicPaths.clear();
        d->m_data.playlist.userLists.clear();
        d->m_data.playlist.preloadList = SettingsPrivate::LOCAL_KEY;
        return saveToJson();
    }
    return true;
}

Settings::~Settings() = default;

Settings::Settings(QObject* parent)
    : QObject(parent),
      d(std::make_unique<SettingsPrivate>())
{
#if SETTING_RELEASE
    d->m_path = QCoreApplication::applicationDirPath() +
            SettingsPrivate::SETTINGS_DIRECTORY +
            SettingsPrivate::SETTINGS_FILE;
#else
    d->m_settingFilePath = PROJECT_PATH + SettingsPrivate::SETTINGS_DIRECTORY +
            SettingsPrivate::SETTINGS_FILE;
#endif
    if (!initJsonFile(d->m_settingFilePath))
    {
        LOG_ERROR("Failed to init Json file");
        return;
    }
    LOG_INFO("[init.json] exists, parsing it.");
    parseJson();
}

bool Settings::saveToJson() const
{
    QJsonObject listJson;
    QJsonObject audioJson;
    listJson[SettingsPrivate::Key::Playlist::MUSIC_PATHS] =
            QJsonArray::fromStringList(d->m_data.playlist.musicPaths);
    listJson[SettingsPrivate::Key::Playlist::USER_LISTS] =
            QJsonArray::fromStringList(d->m_data.playlist.userLists);
    listJson[SettingsPrivate::Key::Playlist::PRELOAD_LIST] = QJsonValue(d->m_data.playlist.preloadList);
    audioJson[SettingsPrivate::Key::Audio::DEFAULT_VOLUME] =
            QJsonValue(d->m_data.audio.defaultVolume);

    QJsonObject jsonObj;
    jsonObj[SettingsPrivate::Key::PLAYLIST_KEY] = listJson;
    jsonObj[SettingsPrivate::Key::AUDIO_KEY] = audioJson;

    const QJsonDocument doc(jsonObj);
    QFile file(d->m_settingFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        LOG_ERROR("Could not write to Json file");
        return false;
    }
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    LOG_INFO("Saved to Json file successfully");
    return true;
}

void Settings::addLocalMusicDirectory(const QString& path)
{
    if (!QDir(path).exists())
    {
        LOG_ERROR(QString(
                      "Attempted to add an empty or non-existent local directory: %1"
                  ).arg(path));
        return;
    }
    if (!d->m_data.playlist.musicPaths.contains(path))
    {
        d->m_data.playlist.musicPaths.append(path);
        if (!saveToJson())
        {
            return;
        }
        Q_EMIT signalLocalDirectoryChanged(d->m_data.playlist.musicPaths);
        LOG_INFO(QString("Add path successfully: %1").arg(path));
    }
}

void Settings::removeLocalMusicDirectory(const QString& path)
{
    if (d->m_data.playlist.musicPaths.removeOne(path))
    {
        if (!saveToJson())
        {
            return;
        }
        Q_EMIT signalLocalDirectoryChanged(d->m_data.playlist.musicPaths);
        LOG_INFO(QString("Remove directory: %1").arg(path));
    }
}

void Settings::addUserPlaylist(const QString& name)
{
    if (!d->m_data.playlist.userLists.contains(name))
    {
        d->m_data.playlist.userLists.append(name);
        if (!saveToJson())
        {
            return;
        }
        Q_EMIT signalUserKeySetsChanged(d->m_data.playlist.userLists);
        LOG_INFO(QString("Added user music: %1").arg(name));
    }
}

void Settings::removeUserPlaylist(const QString& name)
{
    if (d->m_data.playlist.userLists.removeOne(name))
    {
        if (!saveToJson())
        {
            return;
        }
        Q_EMIT signalUserKeySetsChanged(d->m_data.playlist.userLists);
        LOG_INFO(QString("Removed user music: %1").arg(name));
    }
}

QStringList Settings::getLocalMusicDirectories() const
{
    return d->m_data.playlist.musicPaths;
}

QStringList Settings::getKeysUserPlaylist() const
{
    return d->m_data.playlist.userLists;
}

unsigned Settings::getDefaultVolume() const
{
    return d->m_data.audio.defaultVolume;
}

QString Settings::getPreloadKey() const
{
    return d->m_data.playlist.preloadList;
}

void Settings::changeDefaultVolume(const int volume)
{
    if (d->m_data.audio.defaultVolume != volume)
    {
        d->m_data.audio.defaultVolume = volume;
        if (saveToJson())
        {
            LOG_INFO(QString("Changed default volume: %1").arg(volume));
        }
        else
        {
            LOG_ERROR(QString("Could not change default volume: %1").arg(volume));
        }
    }
}

void Settings::changePreloadKey(const QString& key)
{
    if (d->m_data.playlist.preloadList != key)
    {
        d->m_data.playlist.preloadList = key;
        if (saveToJson())
        {
            LOG_INFO(QString("Changed preload key: %1").arg(key));
        }
        else
        {
            LOG_ERROR(QString("Could not change preload key: %1").arg(key));
        }
    }
}

}
