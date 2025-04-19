#include <Logger_qt.h>
#include <Settings.h>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>


namespace Core::Service {
    Settings::Settings(const QString &settingsPath, QObject *parent): QObject(parent), m_settingsPath(settingsPath) {
        setObjectName(QStringLiteral("Settings"));
        Log = Logger_QT(objectName());
        if (m_settingsPath.isEmpty()) {
            Log.log(Logger_QT::LogLevel::Error, "the path of settings is empty");
            return;
        }
        loadFromJson();
    }

    void Settings::loadFromJson() {
        QFile file(m_settingsPath);
        if (!file.open(QIODevice::ReadOnly)) {
            Log.log(Logger_QT::LogLevel::Error, "Failed to open file");
            return;
        }
        const QByteArray jsonData = file.readAll();
        file.close();
        const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        if (jsonDoc.isNull() || !jsonDoc.isObject()) {
            Log.log(Logger_QT::LogLevel::Error, "Failed to parse JSON");
            return;
        }
        QJsonObject json = jsonDoc.object();
        m_localMusicList = json["MusicDirectory"].toVariant().toStringList();
        m_dbPath = json["DatabaseDirectory"].toString();
        m_userList = json["UserLists"].toVariant().toStringList();
        m_volume = json["DefaultVolume"].toInt();
    };


    void Settings::saveToJson() {
        QJsonObject jsonObj;
        jsonObj["MusicDirectory"] = QJsonArray::fromStringList(m_localMusicList);
        jsonObj["DatabaseDirectory"] = QJsonValue(m_dbPath);
        jsonObj["DefaultVolume"] = QJsonValue(m_volume);
        jsonObj["UserLists"] = QJsonArray::fromStringList(m_userList);
        const QJsonDocument doc(jsonObj);
        QFile file(m_settingsPath);
        if (!file.open(QIODevice::WriteOnly)) {
            Log.log(Logger_QT::LogLevel::Error, "Could not write to JSON file");
            return;
        }
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        Log.log(Logger_QT::LogLevel::Info, "Saved JSON successfully");
    }

    void Settings::addLocalMusicDirectory(const QString &path) {
        if (!m_localMusicList.contains(path)) {
            m_localMusicList.append(path);
            saveToJson();
            Log.log(Logger_QT::LogLevel::Info, "Added local music: " + path);
            Q_EMIT signalLocalSettingsChanged();
        }
    }

    void Settings::removeLocalMusicDirectory(const QString &path) {
        if (m_localMusicList.removeOne(path)) {
            saveToJson();
            Q_EMIT signalLocalSettingsChanged();
        }
    }

    void Settings::addUserMusicList(const QString &path) {
        if (!m_userList.contains(path)) {
            m_userList.append(path);
            saveToJson();
            Log.log(Logger_QT::LogLevel::Info, "Added user music: " + path);
            Q_EMIT signalUserListAdded(path);
        }
    }

    void Settings::removeUserMusicList(const QString &path) {
        if (!m_userList.removeOne(path)) {
            saveToJson();
            Log.log(Logger_QT::LogLevel::Info, "Removed user music: " + path);
            Q_EMIT signalUserListRemoved(path);
        }
    }

}
