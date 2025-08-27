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
        static inline const auto MUSIC_PATHS =
                QStringLiteral("LocalMusicPaths");
        static inline const auto USER_LISTS = QStringLiteral("UserLists");
        static inline const auto DEFAULT_VOLUME =
                QStringLiteral("DefaultVolume");
    };

    struct SettingsData
    {
        int defaultVolume;
        QStringList musicPaths;
        QStringList userLists;
    };

#define SETTING_RELEASE 0

    static inline const auto SETTINGS_DIRECTORY = QStringLiteral("settings/");

    static inline const auto SETTINGS_FILE = QStringLiteral("init.json");

    QString m_path;
    SettingsData m_data;
};

void Settings::parseJson()
{
    QFile file(d->m_path);
    if (!file.open(QIODevice::ReadOnly))
    {
        LOG_ERROR("Failed to open file");
        return;
    }
    const QByteArray jsonData = file.readAll();
    file.close();
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (jsonDoc.isNull() || !jsonDoc.isObject())
    {
        LOG_ERROR("Failed to parse JSON");
        return;
    }
    LOG_INFO("Parsed JSON successfully");
    QJsonObject jsonObj = jsonDoc.object();
    d->m_data.musicPaths = jsonObj[SettingsPrivate::Key::MUSIC_PATHS].
                           toVariant().toStringList();
    d->m_data.userLists = jsonObj[SettingsPrivate::Key::USER_LISTS].
                          toVariant().toStringList();
    d->m_data.defaultVolume = jsonObj[SettingsPrivate::Key::DEFAULT_VOLUME].
            toInt();
}

bool Settings::initJsonFile(const QString& filePath) const
{
    if (!QFile::exists(filePath))
    {
        const SettingsPrivate::SettingsData tempData{
            20
            , {}
            , {}
        };
        d->m_data = tempData;
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
    d->m_path = PROJECT_PATH + SettingsPrivate::SETTINGS_DIRECTORY +
            SettingsPrivate::SETTINGS_FILE;
#endif
    if (!initJsonFile(d->m_path))
    {
        LOG_ERROR("Failed to init Json file");
        return;
    }
    parseJson();
}

bool Settings::saveToJson() const
{
    QJsonObject jsonObj;
    jsonObj[SettingsPrivate::Key::MUSIC_PATHS] =
            QJsonArray::fromStringList(d->m_data.musicPaths);
    jsonObj[SettingsPrivate::Key::DEFAULT_VOLUME] =
            QJsonValue(d->m_data.defaultVolume);
    jsonObj[SettingsPrivate::Key::USER_LISTS] =
            QJsonArray::fromStringList(d->m_data.userLists);
    const QJsonDocument doc(jsonObj);
    QFile file(d->m_path);
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
    if (!d->m_data.musicPaths.contains(path))
    {
        d->m_data.musicPaths.append(path);
        if (!saveToJson())
        {
            return;
        }
        Q_EMIT signalLocalDirectoryChanged(d->m_data.musicPaths);
        LOG_INFO(QString("Add path successfully: %1").arg(path));
    }
}

void Settings::removeLocalMusicDirectory(const QString& path)
{
    if (d->m_data.musicPaths.removeOne(path))
    {
        if (!saveToJson())
        {
            return;
        }
        Q_EMIT signalLocalDirectoryChanged(d->m_data.musicPaths);
        LOG_INFO(QString("Remove directory: %1").arg(path));
    }
}

void Settings::addUserPlaylist(const QString& name)
{
    if (!d->m_data.userLists.contains(name))
    {
        d->m_data.userLists.append(name);
        if (!saveToJson())
        {
            return;
        }
        Q_EMIT signalUserKeySetsChanged(d->m_data.userLists);
        LOG_INFO(QString("Added user music: %1").arg(name));
    }
}

void Settings::removeUserPlaylist(const QString& name)
{
    if (d->m_data.userLists.removeOne(name))
    {
        if (!saveToJson())
        {
            return;
        }
        Q_EMIT signalUserKeySetsChanged(d->m_data.userLists);
        LOG_INFO(QString("Removed user music: %1").arg(name));
    }
}

QStringList Settings::getLocalMusicDirectories() const
{
    return d->m_data.musicPaths;
}

QStringList Settings::getKeysUserPlaylist() const
{
    return d->m_data.userLists;
}

unsigned Settings::getDefaultVolume() const
{
    return d->m_data.defaultVolume;
}
}
