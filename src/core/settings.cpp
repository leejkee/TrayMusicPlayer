#include <QCoreApplication>
#include <settings/settings.h>
#include <trayconfig.h>
#include <log/log.h>
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
    SettingsPrivate();

    struct Key
    {
        static inline const auto MUSIC_KEY = QStringLiteral("Music");

        static inline const auto AUDIO_KEY = QStringLiteral("Audio");

        struct Music
        {
            static inline const auto MUSIC_PATHS = QStringLiteral("MusicPaths");

            static inline const auto USER_LISTS = QStringLiteral("UserLists");
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

        struct Music
        {
            QStringList musicPaths;
            QStringList userLists;
        };

        // struct RememberSet
        // {
        //     QString theLastListKey;
        //     QString theLastMusic;
        // };

        Audio audio;
        Music music;
    };

#define SETTING_RELEASE 0

    static inline const auto SETTINGS_DIRECTORY = QStringLiteral("settings/");

    static inline const auto SETTINGS_FILE = QStringLiteral("init.json");

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

    auto parseContent = [](const QJsonObject& json, const QString& key)
    {
        if (json.contains(key))
        {
            if (const QJsonValue value = json.value(key); value.isArray())
            {
                QStringList list;
                for (const auto& item : value.toArray())
                {
                    if (item.isString())
                    {
                        list.append(item.toString());
                    }
                }
                return list;
            }
        }
        return {};
    };

    if (jsonObj.contains(SettingsPrivate::Key::MUSIC_KEY))
    {
        const QJsonObject obj = jsonObj.value(SettingsPrivate::Key::MUSIC_KEY).
                                        toObject();
        d->m_data.music.musicPaths = parseContent(obj
                                                  , SettingsPrivate::Key::Music::MUSIC_PATHS);
        d->m_data.music.userLists = parseContent(obj
                                                 , SettingsPrivate::Key::Music::USER_LISTS);
    }
    else
    {
        LOG_ERROR("The Key 'Music' is missing in init.json, using default values")
        ;
    }

    if (jsonObj.contains(SettingsPrivate::Key::AUDIO_KEY))
    {
        const QJsonObject obj = jsonObj.value(SettingsPrivate::Key::AUDIO_KEY).
                                        toObject();
        if (const auto value = obj.
                    value(SettingsPrivate::Key::Audio::DEFAULT_VOLUME); value.
            isDouble())
        {
            d->m_data.audio.defaultVolume = value.toInt();
        }
        else
        {
            LOG_ERROR("The Key 'DefaultVolume' is missing in 'Audio' or not a number, using default value")
            ;
            d->m_data.audio.defaultVolume = 20;
        }
    }
    else
    {
        LOG_ERROR("The Key 'Audio' is missing in init.json, using default values")
        ;
    }
}

bool Settings::initJsonFile(const QString& filePath) const
{
    if (!QFile::exists(filePath))
    {
        d->m_data.audio.defaultVolume = 20;
        d->m_data.music.musicPaths.clear();
        d->m_data.music.userLists.clear();
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
    parseJson();
}

bool Settings::saveToJson() const
{
    QJsonObject musicJson;
    QJsonObject audioJson;
    musicJson[SettingsPrivate::Key::Music::MUSIC_PATHS] =
            QJsonArray::fromStringList(d->m_data.music.musicPaths);
    musicJson[SettingsPrivate::Key::Music::USER_LISTS] =
            QJsonArray::fromStringList(d->m_data.music.userLists);
    audioJson[SettingsPrivate::Key::Audio::DEFAULT_VOLUME] =
            QJsonValue(d->m_data.audio.defaultVolume);

    QJsonObject jsonObj;
    jsonObj[SettingsPrivate::Key::MUSIC_KEY] = musicJson;
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
    if (!d->m_data.music.musicPaths.contains(path))
    {
        d->m_data.music.musicPaths.append(path);
        if (!saveToJson())
        {
            return;
        }
        Q_EMIT signalLocalDirectoryChanged(d->m_data.music.musicPaths);
        LOG_INFO(QString("Add path successfully: %1").arg(path));
    }
}

void Settings::removeLocalMusicDirectory(const QString& path)
{
    if (d->m_data.music.musicPaths.removeOne(path))
    {
        if (!saveToJson())
        {
            return;
        }
        Q_EMIT signalLocalDirectoryChanged(d->m_data.music.musicPaths);
        LOG_INFO(QString("Remove directory: %1").arg(path));
    }
}

void Settings::addUserPlaylist(const QString& name)
{
    if (!d->m_data.music.userLists.contains(name))
    {
        d->m_data.music.userLists.append(name);
        if (!saveToJson())
        {
            return;
        }
        Q_EMIT signalUserKeySetsChanged(d->m_data.music.userLists);
        LOG_INFO(QString("Added user music: %1").arg(name));
    }
}

void Settings::removeUserPlaylist(const QString& name)
{
    if (d->m_data.music.userLists.removeOne(name))
    {
        if (!saveToJson())
        {
            return;
        }
        Q_EMIT signalUserKeySetsChanged(d->m_data.music.userLists);
        LOG_INFO(QString("Removed user music: %1").arg(name));
    }
}

QStringList Settings::getLocalMusicDirectories() const
{
    return d->m_data.music.musicPaths;
}

QStringList Settings::getKeysUserPlaylist() const
{
    return d->m_data.music.userLists;
}

unsigned Settings::getDefaultVolume() const
{
    return d->m_data.audio.defaultVolume;
}
}
