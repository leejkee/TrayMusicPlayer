#include <settings/settings.h>
#include <log/log.h>
#include <utils/jsonutils.h>
#include <QFile>
#include <QDir>
#include <QJsonArray>
#include <QtWidgets/QApplication>
#include <QJsonDocument>
#include <QJsonObject>

namespace Tray::Core
{
Settings::Settings(QObject* parent)
    : Settings({}, parent)
{
}

Settings::Settings(const QString& configFilePath, QObject* parent)
    : QObject(parent)
{
    if (configFilePath.isEmpty())
    {// default path
        m_settingFilePath = QDir(QApplication::applicationDirPath()).
                absoluteFilePath("../settings/init.json");
    }
    else
    {
        m_settingFilePath = configFilePath;
    }
    LOG_INFO(QString("Init.json: %1").arg(m_settingFilePath));
    if (!initJsonFile(m_settingFilePath))
    {
        LOG_ERROR("Failed to init Json file");
        return;
    }
    LOG_INFO("[init.json] exists, parsing it.");
    parseJson();
}

void Settings::parseJson()
{
    QFile file(m_settingFilePath);
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

    if (jsonObj.contains(Key::PLAYLIST_KEY))
    {
        const QJsonObject obj = jsonObj.
                                value(Key::PLAYLIST_KEY).
                                toObject();
        m_data.playlist.musicPaths = Utils::getJsonValue<QStringList>(obj
            , Key::Playlist::MUSIC_PATHS);
        m_data.playlist.userLists = Utils::getJsonValue<QStringList>(obj
            , Key::Playlist::USER_LISTS);
        m_data.playlist.preloadListKey = Utils::getJsonValue<QString>(obj
            , Key::Playlist::PRELOAD_LIST);
    }
    else
    {
        LOG_ERROR("The Key 'Music' is missing in init.json, using default value")
        ;
    }

    if (jsonObj.contains(Key::AUDIO_KEY))
    {
        const QJsonObject obj = jsonObj.value(Key::AUDIO_KEY).
                                        toObject();
        m_data.audio.defaultVolume = Utils::getJsonValue<int>(obj
            , Key::Audio::DEFAULT_VOLUME);
    }
    else
    {
        LOG_ERROR("The Key 'Audio' is missing in init.json, using default value")
        ;
    }
}


bool Settings::initJsonFile(const QString& filePath)
{
    if (!QFile::exists(filePath))
    {
        m_data = DEFAULT_SETTINGS;
        return saveToJson();
    }
    return true;
}

Settings::~Settings() = default;


bool Settings::saveToJson() const
{
    QJsonObject listJson;
    QJsonObject audioJson;
    listJson[Key::Playlist::MUSIC_PATHS] =
            QJsonArray::fromStringList(m_data.playlist.musicPaths);
    listJson[Key::Playlist::USER_LISTS] =
            QJsonArray::fromStringList(m_data.playlist.userLists);
    listJson[Key::Playlist::PRELOAD_LIST] =
            QJsonValue(m_data.playlist.preloadListKey);
    audioJson[Key::Audio::DEFAULT_VOLUME] =
            QJsonValue(m_data.audio.defaultVolume);

    QJsonObject jsonObj;
    jsonObj[Key::PLAYLIST_KEY] = listJson;
    jsonObj[Key::AUDIO_KEY] = audioJson;

    const QJsonDocument doc(jsonObj);
    QFile file(m_settingFilePath);
    const QFileInfo info(file);
    if (const QDir dir; !dir.mkpath(info.absolutePath())) {
        LOG_ERROR(QString("Could not create directory: %1").arg(info.absolutePath()));
        return false;
    }
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
    if (!m_data.playlist.musicPaths.contains(path))
    {
        m_data.playlist.musicPaths.append(path);
        if (!saveToJson())
        {
            return;
        }
        Q_EMIT signalLocalDirectoryChanged(m_data.playlist.musicPaths);
        LOG_INFO(QString("Add path successfully: %1").arg(path));
    }
}

void Settings::removeLocalMusicDirectory(const QString& path)
{
    if (m_data.playlist.musicPaths.removeOne(path))
    {
        if (!saveToJson())
        {
            return;
        }
        Q_EMIT signalLocalDirectoryChanged(m_data.playlist.musicPaths);
        LOG_INFO(QString("Remove directory: %1").arg(path));
    }
}

void Settings::addUserPlaylist(const QString& name)
{
    if (!m_data.playlist.userLists.contains(name))
    {
        m_data.playlist.userLists.append(name);
        if (!saveToJson())
        {
            return;
        }
        Q_EMIT signalUserKeySetsChanged(m_data.playlist.userLists);
        LOG_INFO(QString("Added user music: %1").arg(name));
    }
}

void Settings::removeUserPlaylist(const QString& name)
{
    if (m_data.playlist.userLists.removeOne(name))
    {
        if (!saveToJson())
        {
            return;
        }
        Q_EMIT signalUserKeySetsChanged(m_data.playlist.userLists);
        LOG_INFO(QString("Removed user music: %1").arg(name));
    }
}

QStringList Settings::getLocalMusicDirectories() const
{
    return m_data.playlist.musicPaths;
}

QStringList Settings::getKeysUserPlaylist() const
{
    return m_data.playlist.userLists;
}

int Settings::getDefaultVolume() const
{
    return m_data.audio.defaultVolume;
}

QString Settings::getPreloadKey() const
{
    return m_data.playlist.preloadListKey;
}

void Settings::changeDefaultVolume(const int volume)
{
    if (m_data.audio.defaultVolume != volume)
    {
        m_data.audio.defaultVolume = volume;
        if (saveToJson())
        {
            LOG_INFO(QString("Changed default volume: %1").arg(volume));
        }
        else
        {
            LOG_ERROR(QString("Could not change default volume: %1").arg(volume
                      ));
        }
    }
}

void Settings::changePreloadKey(const QString& key)
{
    if (m_data.playlist.preloadListKey != key)
    {
        m_data.playlist.preloadListKey = key;
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
