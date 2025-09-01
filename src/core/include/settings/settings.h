#pragma once
#include <QObject>
#include <QStringList>
#include <memory>


namespace Tray::Core
{
class SettingsPrivate;

class Settings final : public QObject
{
    Q_OBJECT

public:
    explicit Settings(QObject* parent = nullptr);

    Settings(const Settings&) = delete;

    Settings& operator=(const Settings&) = delete;

    Settings(Settings&&) = delete;

    Settings& operator=(Settings&&) = delete;

    ~Settings() override;

    [[nodiscard]] QStringList getLocalMusicDirectories() const;

    [[nodiscard]] QStringList getKeysUserPlaylist() const;

    [[nodiscard]] unsigned getDefaultVolume() const;

    QString getPreloadKey() const;

Q_SIGNALS:
    void signalUserKeySetsChanged(const QStringList&);

    void signalLocalDirectoryChanged(const QStringList&);

public Q_SLOTS:
    void addLocalMusicDirectory(const QString& path);

    void addUserPlaylist(const QString& name);

    void removeUserPlaylist(const QString& name);

    void removeLocalMusicDirectory(const QString& path);

    void changeDefaultVolume(int volume);

    void changePreloadKey(const QString& key);

private:
    std::unique_ptr<SettingsPrivate> d;

    /// @brief Initializes the JSON settings file if it does not exist.
    ///
    /// This function checks for the existence of the settings file at the provided
    /// path. If the file is not found, it creates a new file with default settings
    /// and saves it. If the file already exists, no action is taken.
    ///
    /// @param filePath The absolute path to the settings file.
    /// @return `true` if the file exists or was successfully created; otherwise, `false`.
    [[nodiscard]] bool initJsonFile(const QString& filePath) const;

    /// @brief Parses the application's settings from a JSON file.
    ///
    /// This function reads the content of the JSON file specified by `m_path`,
    /// parses it into a QJsonDocument, and then populates the member variables
    /// of the SettingsData struct with the values from the JSON object.
    ///
    /// The function assumes the file at `m_path` exists and is accessible.
    /// If the file cannot be opened or the JSON is invalid, an error is logged
    /// and the function returns without modifying the current settings.
    void parseJson();

    [[nodiscard]] bool saveToJson() const;

};
}
